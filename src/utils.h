// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef UTILS_H
#define UTILS_H

#include <Python.h>
#include <uv.h>
#include "common.h"
#include "promise.h"

#define INIT_PROMISE_AND_REQ(promise, req, req_type)    \
    (req) = NULL;                                       \
    (promise) = promise_new();                          \
    if (promise) {                                      \
        req = Request_NEW(promise, req_type);           \
        if (!(req)) {                                   \
            Py_DECREF(promise);                         \
            promise = NULL;                             \
        }                                               \
    }

//#define UV_CALL_START(func, req, ...) {      \
//    uv_loop_t *_loop = loop_get();           \
//    int error;                               \
//    Py_BEGIN_ALLOW_THREADS                   \
//    error = func(_loop, req, __VA_ARGS__);   \
//    Py_END_ALLOW_THREADS
//
//#define ON_ERROR if (error < 0)
//#define UV_CALL_END }

#define BEGIN_UV_CALL(func, req, ...) {      \
    uv_loop_t *_loop = loop_get();           \
    int error;                               \
    Py_BEGIN_ALLOW_THREADS                   \
    error = func(_loop, req, __VA_ARGS__);   \
    Py_END_ALLOW_THREADS                     \
    if (error < 0) {                         \
        Request_CLOSE(req);

#define END_UV_CALL }}

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
