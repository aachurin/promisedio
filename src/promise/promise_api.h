// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_API_H
#define PROMISEDIO_PROMISE_API_H

#include "promise.h"

typedef struct promise_s Promise;

struct promise_s {
    PyObject_HEAD
    Chain_NODE(Promise)
};

#define Promise_New() \
    Capsule_METH_NOARGS(PROMISE_API_ID, PROMISE_NEW_API_ID, PyObject*)

#define Promise_Then(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_THEN_API_ID, PyObject*, ARGS(PyObject*), __VA_ARGS__)

#define Promise_NewResolved(...) \
    Capsule_METH(PROMISE_API_ID, PROMISE_NEWRESOLVED_API_ID, PyObject*, ARGS(PyObject*), __VA_ARGS__)

#define Promise_IsResolved(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_ISRESOLVED_API_ID, int, ARGS(PyObject*), __VA_ARGS__)

#define Promise_Resolve(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_RESOLVE_API_ID, void, ARGS(PyObject*, PyObject*), __VA_ARGS__)

#define Promise_Reject(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_REJECT_API_ID, void, ARGS(PyObject*, PyObject*), __VA_ARGS__)

#define Promise_RejectArgs(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_REJECTARGS_API_ID, void, ARGS(PyObject*, PyObject*, PyObject*), __VA_ARGS__)

#define Promise_RejectString(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_REJECTSTRING_API_ID, void, ARGS(PyObject*, PyObject*, const char*), __VA_ARGS__)

#define Promise_RejectPyExc(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_REJECTPYEXC_API_ID, void, ARGS(PyObject*), __VA_ARGS__)

#define Promise_Callback(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_CALLBACK_API_ID, void, ARGS(PyObject*, promisecb, promisecb), __VA_ARGS__)

#define Promise_ClearChain() \
    Capsule_METH_NOARGS(PROMISE_API_ID, PROMISE_CLEARCHAIN_API_ID, void)

#define Promise_ProcessChain() \
    Capsule_METH_NOARGS(PROMISE_API_ID, PROMISE_PROCESSCHAIN_API_ID, int)

#define Promise_Data(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_DATA_API_ID, void*, ARGS(PyObject*), __VA_ARGS__)

#define Promise_GetCtx(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_GETCTX_API_ID, PyObject *, ARGS(PyObject*), __VA_ARGS__)

#define Promise_SetCtx(...) \
    Capsule_FUNC(PROMISE_API_ID, PROMISE_SETCTX_API_ID, PyObject *, ARGS(PyObject*, PyObject*), __VA_ARGS__)

#define Promise_DATA(promise, type) \
    ((type *) Promise_Data(promise))


#endif
