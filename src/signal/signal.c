// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "core/base.h"
#include "core/module.h"
#include "core/capsule.h"
#include "core/atomic.h"
#include "clinic/signal.c.h"


typedef struct {
    int is_main_interp;
    PyObject *signal_func;
} _modulestate;

typedef void (*sigevent)();

static struct {
    _Py_atomic_address sig_ev;
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
    sigevent sig_func = (sigevent)_Py_atomic_load(&global_state.sig_ev);
    if (sig_func) {
        sig_func();
    }
}

CAPSULE_API(SIGNAL_API, void)
Signal_SetSigEvent(sigevent cb)
{
    _Py_atomic_store(&global_state.sig_ev, (uintptr_t) cb);
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
    _CTX_set_module(module);
    if (S(is_main_interp)) {
        PyObject *ret = PyObject_CallFunctionObjArgs(S(signal_func), signalnum, handler);
        if (!ret)
            return NULL;
        int signum = _PyLong_AsInt(signalnum);
        void (*c_handler)(int) = PyOS_getsig(signum);
        if (c_handler == SIG_DFL || c_handler == SIG_IGN) {
            return ret;
        }
        PyOS_setsig(signum, signal_handler);
        return ret;
    } else {
        // TODO:
        Py_RETURN_NONE;
    }
}

#define SIGNAL_KEY "__promisedio.a1d0c6e83f027327d8461063f4ac58a6"

static int
signalmodule_exec(PyObject *module)
{
    _CTX_set_module(module);
    LOG("(%p)", module);
    S(is_main_interp) = Py_IsMainInterp();
    if (S(is_main_interp)) {
        int err = -1;
        PyObject *signal_mod = NULL;
        PyObject *signal_func = NULL;
        signal_func = PyObject_GetAttrString(module, "signal");
        if (!signal_func)
            goto finally;
        signal_mod = PyImport_ImportModule("_signal");
        if (!signal_mod)
            goto finally;
        if (PyObject_HasAttrString(signal_mod, SIGNAL_KEY)) {
            S(signal_func) = PyObject_GetAttrString(signal_mod, SIGNAL_KEY);
        } else {
            S(signal_func) = PyObject_GetAttrString(signal_mod, "signal");
            if (!S(signal_func))
                goto finally;
            if (PyObject_SetAttrString(signal_mod, SIGNAL_KEY, S(signal_func)))
                goto finally;
            if (PyObject_SetAttrString(signal_mod, "signal", signal_func))
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
        Py_XDECREF(signal_func);
        return err;
    }
    return 0;
}

#include "capsule.h"

static int
signalmodule_create_api(PyObject *module)
{
    LOG("(%p)", module);
    Capsule_CREATE(module, SIGNAL_API);
    return 0;
}

static int
signalmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _CTX_set_module(module);
    Py_VISIT(S(signal_func));
    return 0;
}

static int
signalmodule_clear(PyObject *module)
{
    _CTX_set_module(module);
    Py_CLEAR(S(signal_func));
    return 0;
}

static void
signalmodule_free(void *module)
{
    LOG("(%p)", module);
    signalmodule_clear(module);
}

static PyMethodDef signalmodule_methods[] = {
    SIGNAL_SIGNAL_METHODDEF
    {NULL},
};

static PyModuleDef_Slot signalmodule_slots[] = {
    {Py_mod_exec, signalmodule_exec},
    {Py_mod_exec, signalmodule_create_api},
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