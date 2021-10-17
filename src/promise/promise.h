// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_H
#define PROMISEDIO_PROMISE_H

#include "core/base.h"
#include "core/capsule.h"
#include "core/module.h"

Py_LOCAL_INLINE(PyObject *)
new_exception(PyObject *exception, const char *msg)
{
    PyObject *value = PyUnicode_FromString(msg);
    if (value == NULL) {
        return NULL;
    }
    PyObject *exc = PyObject_CallOneArg(exception, value);
    Py_DECREF(value);
    return exc;
}

Py_LOCAL_INLINE(PyObject *)
fetch_current_exception()
{
    PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    if (exc == NULL)
        return NULL;
    PyErr_NormalizeException(&exc, &val, &tb);
    if (tb != NULL) {
        PyException_SetTraceback(val, tb);
        Py_DECREF(tb);
    }
    Py_DECREF(exc);
    return val;
}

Py_LOCAL_INLINE(void)
promise_print_unhandled_exception()
{
    PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    if (exc == NULL) {
        PySys_WriteStderr("lost exception value\n");
        return;
    }
    PyErr_NormalizeException(&exc, &val, &tb);
    if (tb == NULL) {
        tb = Py_NewRef(Py_None);
    }
    PyException_SetTraceback(val, tb);
    PyErr_Display(exc, val, tb);
    PyException_SetTraceback(val, Py_None);
    Py_DECREF(exc);
    Py_DECREF(val);
    Py_DECREF(tb);
    PySys_WriteStderr("\n");
}

typedef PyObject *(*promisecb)(PyObject *value, PyObject *promise);

#define PROMISE_API_ID _promise_API_1_0
#define PROMISE_NEW_API_ID 0
#define PROMISE_THEN_API_ID 1
#define PROMISE_NEWRESOLVED_API_ID 2
#define PROMISE_ISRESOLVED_API_ID 3
#define PROMISE_RESOLVE_API_ID 4
#define PROMISE_REJECT_API_ID 5
#define PROMISE_REJECTARGS_API_ID 6
#define PROMISE_REJECTSTRING_API_ID 7
#define PROMISE_REJECTPYEXC_API_ID 8
#define PROMISE_CALLBACK_API_ID 9
#define PROMISE_CLEARCHAIN_API_ID 10
#define PROMISE_PROCESSCHAIN_API_ID 11
#define PROMISE_DATA_API_ID 12
#define PROMISE_GETCTX_API_ID 13
#define PROMISE_SETCTX_API_ID 14

#endif
