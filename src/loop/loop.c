// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "core/atomic.h"
#include "core/base.h"
#include "core/handle.h"
#include "core/module.h"
#include "core/capsule.h"
#include "core/chain.h"
#include "clinic/loop.c.h"
#include "capsule/signal.h"
#include "loop.h"

typedef struct modulestate_s _modulestate;

typedef struct {
    uv_async_t base;
    _ctx_var;
} sigevent;

typedef struct modulestate_s {
    int is_main_interp;
    uv_loop_t loop;
    uv_loop_t *loop_ptr;
    volatile int running;
    PyObject *sig_trap;
    volatile int sig_flag;
    sigevent sig_ev;
    sigevent *sig_ev_ptr;
    Capsule_MOUNT(PROMISE_API)
} _modulestate;

static struct {
    _Py_atomic_lock lock;
    sigevent *sig_ev;
} global_state = {0};

/*[clinic input]
module loop
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=d391b010e8567f5d]*/

static void
sig_event_callback(uv_async_t *handle)
{
    _CTX_setstored((sigevent *) handle);
    S(sig_flag) = 1;
}

static void
loop_sigevent()
{
    if (atomic_try_lock(&global_state.lock)) {
        if (global_state.sig_ev) {
            uv_async_send((uv_async_t *) global_state.sig_ev);
        }
        atomic_unlock(&global_state.lock);
    }
}

static void
loop_close_handle(uv_handle_t *handle, _ctx_var)
{
    if (handle->data) {
        Handle_Close_UV(handle);
    } // else non-user handles
}

static void
loop_stop(_ctx_var)
{
    uv_walk(S(loop_ptr), (uv_walk_cb) loop_close_handle, _ctx);
    uv_run(S(loop_ptr), UV_RUN_DEFAULT);
    Promise_ClearChain();
}

Py_LOCAL_INLINE(int)
create_loop(_ctx_var)
{
    int err = uv_loop_init(&S(loop));
    if (err)
        return err;
    S(loop_ptr) = &S(loop);
    if (S(is_main_interp)) {
        uv_async_init(S(loop_ptr), (uv_async_t *) &S(sig_ev), sig_event_callback);
        S(sig_ev_ptr) = &S(sig_ev);
        S(sig_ev_ptr)->base.data = NULL;
        S(sig_ev_ptr)->_ctx = _ctx;
        uv_unref((uv_handle_t *) S(sig_ev_ptr));
        atomic_lock(&global_state.lock);
        global_state.sig_ev = S(sig_ev_ptr);
        atomic_unlock(&global_state.lock);
    }
    return 0;
}

CAPSULE_API(LOOP_API, uv_loop_t *)
Loop_Get(_ctx_var)
{
    if (!S(loop_ptr)) {
        int err = create_loop(_ctx);
        if (err) {
            Py_SetUVError(PyExc_OSError, err);
            return NULL;
        }
    }
    return S(loop_ptr);
}

Py_LOCAL_INLINE(int)
run_until_complete(_ctx_var)
{
    if (S(running)) {
        PyErr_SetString(PyExc_RuntimeError, "The loop is already running.");
        return -1;
    }

    uv_loop_t *loop = Loop_Get(_ctx);
    S(running) = 1;
    S(sig_flag) = 0;
    LOG("(%p): start", loop);
    int promise_do_more, uv_do_more = 1;
    Py_BEGIN_ALLOW_THREADS
        while (1) {
            ACQUIRE_GIL
                promise_do_more = Promise_ProcessChain();
            RELEASE_GIL
            if (S(sig_flag)) {
                S(sig_flag) = 0;
                if (promise_do_more == 0) {
                    ACQUIRE_GIL
                        PyObject *ret = PyObject_CallNoArgs(S(sig_trap));
                        if (ret != NULL) {
                            Py_DECREF(ret);
                        } else {
                            promise_do_more = -1;
                        }
                    RELEASE_GIL
                }
            }
            if (promise_do_more < 0 || (promise_do_more == 0 && uv_do_more == 0)) {
                break;
            }
            uv_do_more = uv_run(loop, UV_RUN_ONCE);
        }
    Py_END_ALLOW_THREADS
    LOG("(%p): stop", loop);
    loop_stop(_ctx);

#ifdef BUILD_LOG_DEBUG
    uv_print_all_handles(loop, stderr);
#endif

    S(running) = 0;
    if (PyErr_Occurred()) {
        return -1;
    }
    return 0;
}

/*[clinic input]
loop.run_until_complete
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop_run_until_complete_impl(PyObject *module)
/*[clinic end generated code: output=a7dc5a7be12de46e input=3ecb0123b16c5bb0]*/
{
    _CTX_setmodule(module);
    if (run_until_complete(_ctx)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
loop.time
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop_time_impl(PyObject *module)
/*[clinic end generated code: output=a19f939adde02bd3 input=02332386bb9543a3]*/
{
    _CTX_setmodule(module);
    return PyLong_FromUint64_t(uv_now(S(loop_ptr)));
}

/*[clinic input]
loop.hrtime
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop_hrtime_impl(PyObject *module)
/*[clinic end generated code: output=7c4b876ffef14198 input=d5db8b582a44c5d8]*/
{
    return PyLong_FromUint64_t(uv_hrtime());
}

static int
loopmodule_init(PyObject *module)
{
    _CTX_setmodule(module);
    LOG("(%p)", module);
    S(is_main_interp) = Py_IsMainInterp();
    Capsule_LOAD("promisedio.promise", PROMISE_API);
    return 0;
}

#include "capsule.h"

static int
loopmodule_create_api(PyObject *module)
{
    LOG("(%p)", module);
    Capsule_CREATE(module, LOOP_API);
    return 0;
}

static int
loopmodule_exec(PyObject *module)
{
    _CTX_setmodule(module);
    LOG("(%p)", module);
    PyObject *mod = PyImport_ImportModule("promisedio._internal");
    if (!mod)
        return -1;
    PyObject *trap = PyObject_GetAttrString(mod, "__sigtrap");
    if (trap) {
        S(sig_trap) = trap;
    }
    Py_DECREF(mod);
    if (!trap) {
        return -1;
    }
    if (S(is_main_interp)) {
        void (*setsig)(void *) = Capsule_GetFunc("promisedio.signal", SIGNAL_API, SIGNAL_SETSIGEVENT_ID);
        setsig(loop_sigevent);
    }
    return 0;
}

static int
loopmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _CTX_setmodule(module);
    Capsule_VISIT(PROMISE_API);
    Py_VISIT(S(sig_trap));
    return 0;
}

static int
loopmodule_clear(PyObject *module)
{
    _CTX_setmodule(module);
    Py_CLEAR(S(sig_trap));
    return 0;
}

static void
loopmodule_free(void *module)
{
    _CTX_setmodule(module);
    LOG("(%p)", module);
    if (S(is_main_interp)) {
        if (S(sig_ev_ptr)) {
            atomic_lock(&global_state.lock);
            if (global_state.sig_ev == S(sig_ev_ptr)) {
                global_state.sig_ev = NULL;
            }
            atomic_unlock(&global_state.lock);
            uv_close((uv_handle_t *) S(sig_ev_ptr), NULL);
        }
    }
    if (S(loop_ptr)) {
        loop_stop(_ctx);
        uv_loop_close(S(loop_ptr));
    }
    Capsule_CLEAR(PROMISE_API);
    loopmodule_clear(module);
}

static PyMethodDef loopmodule_methods[] = {
    LOOP_RUN_UNTIL_COMPLETE_METHODDEF
    LOOP_TIME_METHODDEF
    LOOP_HRTIME_METHODDEF
    {NULL}
};

static PyModuleDef_Slot loopmodule_slots[] = {
    {Py_mod_exec, loopmodule_init},
    {Py_mod_exec, loopmodule_create_api},
    {Py_mod_exec, loopmodule_exec},
    {0, NULL}
};

static struct PyModuleDef loopmodule_def = {
    PyModuleDef_HEAD_INIT,
    "promisedio.loop",
    NULL,
    sizeof(_modulestate),
    loopmodule_methods,
    loopmodule_slots,
    loopmodule_traverse,
    loopmodule_clear,
    loopmodule_free
};

PyMODINIT_FUNC
PyInit_loop(void)
{
    return PyModuleDef_Init(&loopmodule_def);
}
