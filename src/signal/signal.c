// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "core/base.h"
#include "core/module.h"
#include "core/capsule.h"
#include "loop/loop_api.h"
#include "clinic/signal.c.h"
#include "internal/pycore_atomic.h"

typedef struct {
    uv_loop_t *loop;
    int is_main_interp;
    PyObject *signal_func;
} _modulestate;

static struct {
    _Py_atomic_int initialized;
    void (*set_sig)();
} global_state = {0};

/*[clinic input]
module signal
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=b0301a3bde5fe9d3]*/

static void
signal_handler(int sig_num)
{
    int save_errno = errno;

    PyErr_SetInterruptEx(sig_num);

#ifndef HAVE_SIGACTION
#ifdef SIGCHLD
    /* To avoid infinite recursion, this signal remains
       reset until explicit re-instated.
       Don't clear the 'func' field as it is our pointer
       to the Python handler... */
    if (sig_num != SIGCHLD)
#endif
        /* If the handler was not set up with sigaction, reinstall it.  See
         * Python/pylifecycle.c for the implementation of PyOS_setsig which
         * makes this true.  See also issue8354. */
        PyOS_setsig(sig_num, signal_handler);
#endif

    /* Issue #10311: asynchronously executing signal handlers should not
       mutate errno under the feet of unsuspecting C code. */
    errno = save_errno;

#ifdef MS_WINDOWS
    if (sig_num == SIGINT) {
        SetEvent(_PyOS_SigintEvent());
    }
#endif
    if (_Py_atomic_load(&global_state.initialized)) {
        global_state.set_sig();
    }
}

/*[clinic input]
signal.signal
    signalnum: object
    handler: object
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
signal_signal_impl(PyObject *module, PyObject *signalnum, PyObject *handler)
/*[clinic end generated code: output=d6215a4c80f4273b input=3009c20a709e2f7e]*/
{
    _STATE_setmodule(module)

    PyObject *ret = PyObject_CallFunctionObjArgs(_state->signal_func, signalnum, handler);
    if (!ret)
        return NULL;
    int signum = _PyLong_AsInt(signalnum);
    void (*c_handler)(int) = PyOS_getsig(signum);
    if (c_handler == SIG_DFL || c_handler == SIG_IGN) {
        return ret;
    }
    PyOS_setsig(signum, signal_handler);
    return ret;
}

static int
signalmodule_init(PyObject *module)
{
    LOG("signalmodule_init(%p)", module);

    _STATE_setmodule(module)

    if (_PyOS_IsMainThread()) {
        _state->is_main_interp = 1;
        _Py_atomic_store(&global_state.initialized, 0);
    }
    return 0;
}

static int
signalmodule_create_api(PyObject *module)
{
    LOG("signalmodule_create_api(%p)", module);
    static void *c_api[] = {};
    Capsule_CREATE(module, SIGNAL_API_ID, c_api);
    return 0;
}

static int
signalmodule_install_signals(PyObject *module)
{
    _STATE_setmodule(module)

    if (_state->is_main_interp) {
        LOG("signalmodule_install_signals(%p)", module);
        int err = -1;
        PyObject *signal_mod = NULL, *my_signal_func = NULL;
        my_signal_func = PyObject_GetAttrString(module, "signal");
        if (!my_signal_func)
            goto finally;
        signal_mod = PyImport_ImportModule("_signal");
        if (!signal_mod)
            goto finally;
        if (PyObject_HasAttrString(signal_mod, "__promisedio.signal_orig")) {
            _state->signal_func = PyObject_GetAttrString(signal_mod, "__promisedio.signal_orig");
        } else {
            _state->signal_func = PyObject_GetAttrString(signal_mod, "signal");
            if (!_state->signal_func)
                goto finally;
            if (PyObject_SetAttrString(signal_mod, "__promisedio.signal_orig", _state->signal_func))
                goto finally;
            if (PyObject_SetAttrString(signal_mod, "signal", my_signal_func))
                goto finally;
        }
        for (int signum = 1; signum < NSIG; signum++) {
            void (*c_handler)(int) = PyOS_getsig(signum);
            if (c_handler == SIG_DFL || c_handler == SIG_IGN || c_handler == SIG_ERR || c_handler == signal_handler)
                continue;
            PyOS_setsig(signum, signal_handler);
        }
        err = 0;
        finally:
        Py_XDECREF(signal_mod);
        Py_XDECREF(my_signal_func);
        return err;
    }
    return 0;
}

static int
signalmodule_init_complete(PyObject *module)
{
    _STATE_setmodule(module)

    LOG("signalmodule_init_complete(%p)", module);
    if (_state->is_main_interp) {
        global_state.set_sig = (void (*)()) Capsule_LOAD_FUNC(
            "promisedio.loop", LOOP_API_ID, LOOP_SETSIG_API_ID);
        if (!global_state.set_sig)
            return -1;
        _Py_atomic_store(&global_state.initialized, 1);
    }
    return 0;
}

static int
signalmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _STATE_setmodule(module)

    Py_VISIT(_state->signal_func);
    return 0;
}

static int
signalmodule_clear(PyObject *module)
{
    _STATE_setmodule(module)

    Py_CLEAR(_state->signal_func);
    return 0;
}

static void
signalmodule_free(void *module)
{
    LOG("signalmodule_free(%p)", module);
    signalmodule_clear(module);
}

static PyMethodDef signalmodule_methods[] = {
    SIGNAL_SIGNAL_METHODDEF
    {NULL},
};

static PyModuleDef_Slot signalmodule_slots[] = {
    {Py_mod_exec, signalmodule_init},
    {Py_mod_exec, signalmodule_create_api},
    {Py_mod_exec, signalmodule_install_signals},
    {Py_mod_exec, signalmodule_init_complete},
    {0, NULL},
};

static struct PyModuleDef signalmodule_def = {
    PyModuleDef_HEAD_INIT,
    "promisedio.signal",
    NULL,
    sizeof(_modulestate),
    signalmodule_methods,
    signalmodule_slots,
    signalmodule_traverse,
    signalmodule_clear,
    signalmodule_free,
};

PyMODINIT_FUNC
PyInit_signal(void)
{
    return PyModuleDef_Init(&signalmodule_def);
}