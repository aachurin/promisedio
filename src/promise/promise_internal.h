// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_INTERNAL_H
#define PROMISEDIO_PROMISE_INTERNAL_H

#include "core/base.h"
#include "core/chain.h"

typedef PyObject *(*promisecb)(PyObject *value, PyObject *promise);
typedef void (*unlockloop)(void* ctx);

enum {
    PROMISE_INITIAL = 0x0000,
    PROMISE_FULFILLED = 0x0001,
    PROMISE_REJECTED = 0x0002,
    PROMISE_RESOLVING = 0x0004,
    PROMISE_RESOLVED = 0x0008,
    PROMISE_INTERIM = 0x0010,
    PROMISE_C_CALLBACK = 0x0020,
    PROMISE_PY_CALLBACK = 0x0040,
    PROMISE_VALUABLE = 0x0100
};

enum {
    PROMISE_HAS_CALLBACK = (PROMISE_C_CALLBACK | PROMISE_PY_CALLBACK),
    PROMISE_SCHEDULED = (PROMISE_FULFILLED | PROMISE_REJECTED),
    PROMISE_FREEZED = (PROMISE_RESOLVING | PROMISE_RESOLVED)
};

#define PROMISE_PUBLIC_FIELDS   \
    PyObject_HEAD               \
    Chain_NODE(Promise);        \
    PyObject *ctx;              \
    char data[32];              \
    int flags;

typedef struct promise_s Promise;

Py_LOCAL_INLINE(PyObject *)
Py_NewError(PyObject *exception, const char *msg)
{
    PyObject *value = PyUnicode_FromString(msg);
    if (!value)
        return NULL;
    PyObject *exc = PyObject_CallOneArg(exception, value);
    Py_DECREF(value);
    return exc;
}

Py_LOCAL_INLINE(PyObject *)
Py_FetchError()
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

#endif
