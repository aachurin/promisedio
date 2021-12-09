// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_H
#define PROMISEDIO_PROMISE_H

#include "promise_internal.h"

struct promise_s {
    PROMISE_PUBLIC_FIELDS
};

Py_LOCAL_INLINE(int)
Promise_IsResolved(Promise *promise)
{
    return !!(promise->flags & PROMISE_SCHEDULED);
}

Py_LOCAL_INLINE(void *)
Promise_Data(Promise *promise)
{
    return &(promise->data);
}

#define Promise_DATA(promise, type) \
    ((type *) Promise_Data(promise))

Py_LOCAL_INLINE(PyObject *)
Promise_GetCtx(Promise *promise)
{
    return promise->ctx;
}

Py_LOCAL_INLINE(PyObject *)
Promise_SetCtx(Promise *promise, PyObject *ctx)
{
    PyObject *ret = promise->ctx;
    promise->ctx = ctx;
    return ret;
}

#endif
