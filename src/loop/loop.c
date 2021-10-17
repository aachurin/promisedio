// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "internal/pycore_atomic.h"
#include "core/base.h"
#include "core/handle.h"
#include "core/module.h"
#include "core/capsule.h"
#include "core/chain.h"
#include "promise/promise_api.h"
#include "loop.h"
#include "clinic/loop.c.h"

typedef struct modulestate_s _modulestate;

typedef struct {
    uv_async_t base;
    _STATE_var;
} SignalEvent;

typedef struct modulestate_s {
    int is_main_interp;
    uv_loop_t loop_obj;
    uv_loop_t *loop;
    volatile int running;
    PyObject *sig_trap;
    volatile int sig_flag;
    SignalEvent sig_event_obj;
    SignalEvent *sig_event;
    Capsule_MOUNT_POINT(PROMISE_API_ID)
} _modulestate;

static struct {
    _Py_atomic_address initialized;
    SignalEvent *sig_event;
} global_state = {0};

/*[clinic input]
module loop
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=d391b010e8567f5d]*/

static void
sig_event_callback(uv_async_t *handle)
{
    _STATE_set((SignalEvent *) handle);
    S(sig_flag) = 1;
}

static void
Loop_SetSig()
{
    // for signal handler only!
    if (_Py_atomic_load(&global_state.initialized)) {
        uv_async_send((uv_async_t *) global_state.sig_event);
    }
}

static void
loop_close_handle(uv_handle_t *handle, _STATE_var)
{
    if (handle->data) {
        Handle_Close_UV(handle);
    } // else non-user handles
}

static void
loop_stop(_STATE_var)
{
    uv_walk(S(loop), (uv_walk_cb) loop_close_handle, _state);
    uv_run(S(loop), UV_RUN_DEFAULT);
    Promise_ClearChain();
}

static int
run_until_complete(_STATE_var)
{
    if (S(running)) {
        PyErr_SetString(PyExc_RuntimeError, "The loop is already running.");
        return -1;
    }

    uv_loop_t *loop = S(loop);
    S(running) = 1;
    S(sig_flag) = 0;

    LOG("run_until_complete(%p): start", loop);

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

    LOG("run_until_complete(%p): stop", loop);

    loop_stop(_state);
#ifdef DEBUG_LOG
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
    _STATE_setmodule(module)

    if (run_until_complete(_state)) {
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
    _STATE_setmodule(module)

    return PyLong_FromUint64_t(uv_now(S(loop)));
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
create_loop(_STATE_var)
{
    if (uv_loop_init(&S(loop_obj)))
        return -1;
    S(loop) = &S(loop_obj);
    if (S(is_main_interp)) {
        uv_async_init(S(loop), (uv_async_t *) &S(sig_event_obj), sig_event_callback);
        S(sig_event) = &S(sig_event_obj);
        S(sig_event)->base.data = NULL;
        _STATE_save(S(sig_event));
        uv_unref((uv_handle_t *) S(sig_event));
        global_state.sig_event = S(sig_event);
    }
    return 0;
}

/*[clinic input]
loop.__after_fork
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop___after_fork_impl(PyObject *module)
/*[clinic end generated code: output=0ff07c3200581fdd input=c9000c7c07e9c627]*/
{
    _STATE_setmodule(module)

    LOG("loop___after_fork");
    _Py_atomic_store(&global_state.initialized, 0);
    Promise_ClearChain();
    if (create_loop(_state)) {
        PyErr_SetString(PyExc_RuntimeError, "Could not create new loop after fork");
        return NULL;
    }
    _Py_atomic_store(&global_state.initialized, (uintptr_t) module);
    Py_RETURN_NONE;
}

/*[clinic input]
loop.__before_fork
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
loop___before_fork_impl(PyObject *module)
/*[clinic end generated code: output=8ed0df040c809ed9 input=1d1231f46e289e9c]*/
{
    PyErr_WarnFormat(PyExc_RuntimeWarning, 1, "IT'S UNSAFE TO USE FORK.");
    Py_RETURN_NONE;
}

static uv_loop_t *
Loop_Get(_STATE_var)
{
    return S(loop);
}

static int
loopmodule_init(PyObject *module)
{
    LOG("loopmodule_init(%p)", module);

    _STATE_setmodule(module)

    Capsule_LOAD("promisedio.promise", PROMISE_API_ID);
    if (_PyOS_IsMainThread()) {
        S(is_main_interp) = 1;
        _Py_atomic_store(&global_state.initialized, 0);
    }
    return 0;
}

static int
loopmodule_create_api(PyObject *module)
{
    LOG("loopmodule_create_api(%p)", module);
    static void *c_api[] = {
        [LOOP_GET_API_ID] = Loop_Get,
        [LOOP_SETSIG_API_ID] = Loop_SetSig
    };
    Capsule_CREATE(module, LOOP_API_ID, c_api);
    return 0;
}

static int
loopmodule_loop_init(PyObject *module)
{
    LOG("loopmodule_loop_init(%p)", module);

    _STATE_setmodule(module)

    if (S(is_main_interp)) {
        PyObject *mod = PyImport_ImportModule("promisedio");
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
    }
    return create_loop(_state);
}

static int
loopmodule_register_at_fork(PyObject *module)
{
    _STATE_setmodule(module)

    if (!S(is_main_interp))
        return 0;
    LOG("loopmodule_register_at_fork(%p)", module);
    PyObject *after_fork = NULL,
        *before_fork = NULL,
        *os = NULL,
        *register_at_fork = NULL,
        *kwargs = NULL,
        *obj = NULL;
    int err = -1;
    after_fork = PyObject_GetAttrString(module, "__after_fork");
    if (!after_fork)
        goto finally;
    before_fork = PyObject_GetAttrString(module, "__before_fork");
    if (!before_fork)
        goto finally;
    os = PyImport_ImportModule("os");
    if (!os)
        goto finally;
    register_at_fork = PyObject_GetAttrString(os, "register_at_fork");
    if (!register_at_fork)
        goto finally;
    kwargs = Py_BuildValue("{s:O,s:O}", "before", before_fork, "after_in_child", after_fork);
    if (!kwargs)
        goto finally;
    obj = PyObject_VectorcallDict(register_at_fork, NULL, 0, kwargs);
    if (!obj)
        goto finally;
    err = 0;
    finally:
    Py_XDECREF(after_fork);
    Py_XDECREF(before_fork);
    Py_XDECREF(os);
    Py_XDECREF(register_at_fork);
    Py_XDECREF(kwargs);
    Py_XDECREF(obj);
    return err;
}

static int
loopmodule_init_signals(PyObject *module)
{
    _STATE_setmodule(module)

    if (S(is_main_interp)) {
        PyObject *mod = PyImport_ImportModule("promisedio.signal");
        if (!mod)
            return -1;
        Py_DECREF(mod);
    }
    return 0;
}

static int
loopmodule_init_complete(PyObject *module)
{
    _STATE_setmodule(module)

    if (S(is_main_interp)) {
        _Py_atomic_store(&global_state.initialized, (uintptr_t) module);
    }
    return 0;
}

static int
loopmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _STATE_setmodule(module)

    Capsule_TRAVERSE(PROMISE_API_ID);
    Py_VISIT(S(sig_trap));
    return 0;
}

static int
loopmodule_clear(PyObject *module)
{
    _STATE_setmodule(module)

    Capsule_CLEAR(PROMISE_API_ID);
    Py_CLEAR(S(sig_trap));
    return 0;
}

static void
loopmodule_free(void *module)
{
    LOG("loopmodule_free(%p)", module);
    loopmodule_clear(module);

    _STATE_setmodule(module)

    if (S(is_main_interp)) {
        if (_Py_atomic_load(&global_state.initialized) == (uintptr_t) module) {
            _Py_atomic_store(&global_state.initialized, 0);
            global_state.sig_event = NULL;
        }
        if (S(sig_event)) {
            uv_close((uv_handle_t *) S(sig_event), NULL);
        }
    }
    if (S(loop)) {
        loop_stop(_state);
        uv_loop_close(S(loop));
    }
}

static PyMethodDef loopmodule_methods[] = {
    LOOP_RUN_UNTIL_COMPLETE_METHODDEF
    LOOP_TIME_METHODDEF
    LOOP_HRTIME_METHODDEF
    LOOP___AFTER_FORK_METHODDEF
    LOOP___BEFORE_FORK_METHODDEF
    {NULL}
};

static PyModuleDef_Slot loopmodule_slots[] = {
    {Py_mod_exec, loopmodule_init},
    {Py_mod_exec, loopmodule_create_api},
    {Py_mod_exec, loopmodule_loop_init},
    {Py_mod_exec, loopmodule_register_at_fork},
    {Py_mod_exec, loopmodule_init_signals},
    {Py_mod_exec, loopmodule_init_complete},
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
