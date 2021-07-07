// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include "promise.h"

#define INIT_DEFAULT_LOOP(loop)                             \
    uv_loop_t *loop = loop_get();

#define INIT_PROMISE_AND_REQUEST(req_type, req, promise)    \
    Promise *promise = promise_new();                       \
    if (!promise)                                           \
        return NULL;                                        \
    req_type *req = Request_New(req_type, promise);         \
    if (!req) {                                             \
        Py_DECREF(promise);                                 \
        return NULL;                                        \
    }

#define UV_REQUEST(func, ...)                               \
    int error;                                              \
    Py_BEGIN_ALLOW_THREADS                                  \
    error = func(__VA_ARGS__);                              \
    Py_END_ALLOW_THREADS                                    \
    if (error < 0)

static inline void
set_uv_error(PyObject *exc, int uverr)
{
    PyObject *args = Py_BuildValue("(is)", uverr, uv_strerror(uverr));
    if (args != NULL) {
        PyErr_SetObject(exc, args);
        Py_DECREF(args);
    }
}

static inline void
promise_reject_uv_error(Promise *promise, PyObject *exc, int uverr)
{
    PyObject *args = Py_BuildValue("(is)", uverr, uv_strerror(uverr));
    if (args == NULL || promise_reject_args(promise, exc, args)) {
        promise_reject_py_exc(promise);
    }
    Py_DECREF(args);
}

#define DefinePyType(type_name, struct_name, dealloc) \
    static PyTypeObject type_name = {                 \
        PyVarObject_HEAD_INIT(NULL, 0)                \
        .tp_name = #struct_name,                      \
        .tp_basicsize = sizeof(struct_name),          \
        .tp_itemsize = 0,                             \
        .tp_flags = Py_TPFLAGS_DEFAULT,               \
        .tp_dealloc = (destructor) (dealloc)          \
    };

#endif
