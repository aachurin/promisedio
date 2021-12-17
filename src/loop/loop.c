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
    PyObject *signal_trap;
    volatile int signal_flag;
    uv_async_t wakeup_event;
    uv_async_t *wakeup_event_ptr;
    sigevent signal_event;
    sigevent *signal_event_ptr;
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
    _CTX_set((sigevent *) handle);
    S(signal_flag) = 1;
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
    }
    // all non-user handles SHOULD be closed manually
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
        uv_async_init(S(loop_ptr), (uv_async_t *) &S(signal_event), sig_event_callback);
        LOG("(%p): sig_ev", &S(signal_event));
        S(signal_event_ptr) = &S(signal_event);
        S(signal_event_ptr)->base.data = NULL;
        _CTX_save(S(signal_event_ptr));
        uv_unref((uv_handle_t *) S(signal_event_ptr));
        atomic_lock(&global_state.lock);
        global_state.sig_ev = S(signal_event_ptr);
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

static void
loop_unlockloop(_ctx_var)
{
    if (S(wakeup_event_ptr)) {
        uv_async_send(S(wakeup_event_ptr));
    }
}

/*[clinic input]
loop.run_forever
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop_run_forever_impl(PyObject *module)
/*[clinic end generated code: output=b743acb40530fbba input=4dc2387b81a4d52f]*/
{
    _CTX_set_module(module);
    if (Promise_StartLoop((unlockloop) loop_unlockloop, _ctx))
        return NULL;

    uv_loop_t *loop = Loop_Get(_ctx);
    uv_async_init(loop, &S(wakeup_event), NULL);
    S(wakeup_event_ptr) = &S(wakeup_event);
    S(signal_flag) = 0;

    LOG("(%p): start", loop);

    Py_BEGIN_ALLOW_THREADS
        while (1) {
            Py_ssize_t process_result;
            ACQUIRE_GIL
                if (PyErr_Occurred()) {
                    process_result = -1;
                } else {
                    process_result = Promise_ProcessChain();
                }
            RELEASE_GIL
            if (process_result <= 0)
                break;
            if (S(signal_flag)) {
                int err = 0;
                S(signal_flag) = 0;
                ACQUIRE_GIL
                    LOG("signal received");
                    // call python function to catch signals
                    PyObject *ret = PyObject_CallNoArgs(S(signal_trap));
                    if (ret != NULL) {
                        Py_DECREF(ret);
                    } else {
                        err = -1;
                    }
                RELEASE_GIL
                if (err) {
                    break;
                }
            }
            uv_run(loop, UV_RUN_ONCE);
        }
    Py_END_ALLOW_THREADS

    uv_close((uv_handle_t* ) &S(wakeup_event), NULL);
    S(wakeup_event_ptr) = NULL;

    LOG("(%p): stop", loop);
    loop_stop(_ctx);

#ifdef BUILD_LOG_DEBUG
    uv_print_all_handles(loop, stderr);
#endif

    Promise_StopLoop((unlockloop) loop_unlockloop, _ctx);

    if (PyErr_Occurred()) {
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
    _CTX_set_module(module);
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
    _CTX_set_module(module);
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
    _CTX_set_module(module);
    LOG("(%p)", module);
    PyObject *mod = PyImport_ImportModule("promisedio._internal");
    if (!mod)
        return -1;
    PyObject *trap = PyObject_GetAttrString(mod, "__sigtrap");
    if (trap) {
        S(signal_trap) = trap;
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
    _CTX_set_module(module);
    Capsule_VISIT(PROMISE_API);
    Py_VISIT(S(signal_trap));
    return 0;
}

static int
loopmodule_clear(PyObject *module)
{
    _CTX_set_module(module);
    Py_CLEAR(S(signal_trap));
    return 0;
}

static void
loopmodule_free(void *module)
{
    _CTX_set_module(module);
    LOG("(%p)", module);
    if (S(is_main_interp)) {
        if (S(signal_event_ptr)) {
            atomic_lock(&global_state.lock);
            if (global_state.sig_ev == S(signal_event_ptr)) {
                global_state.sig_ev = NULL;
            }
            atomic_unlock(&global_state.lock);
            uv_close((uv_handle_t *) S(signal_event_ptr), NULL);
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
    LOOP_RUN_FOREVER_METHODDEF
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
