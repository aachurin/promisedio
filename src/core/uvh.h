// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_UVH_H
#define PROMISEDIO_CORE_UVH_H

#define UV_CALL(func, ...)      \
    int error;                  \
    Py_BEGIN_ALLOW_THREADS      \
    error = func(__VA_ARGS__);  \
    Py_END_ALLOW_THREADS        \
    if (error < 0)


Py_LOCAL_INLINE(PyObject *)
PyErr_UVError(PyObject *exc, int uverr)
{
    PyObject *args = Py_BuildValue("(is)", uverr, uv_strerror(uverr));
    PyObject *ret = NULL;
    if (args) {
        ret = PyObject_CallOneArg(exc, args);
        Py_DECREF(args)
    }
    return ret;
}

Py_LOCAL_INLINE(void)
PyErr_SetUVError(PyObject *exc, int uverr)
{
    PyObject *args = Py_BuildValue("(is)", uverr, uv_strerror(uverr));
    if (args != NULL) {
        PyErr_SetObject(exc, args);
        Py_DECREF(args);
    }
}

#define Promise_RejectUVError(promise, exc, uverr) {                    \
    PyObject *_args = Py_BuildValue("(is)", uverr, uv_strerror(uverr)); \
    if (!_args) {                                                       \
        Promise_RejectPyExc(promise);                                   \
    } else {                                                            \
        Promise_RejectArgs(promise, exc, _args);                        \
    }                                                                   \
    Py_XDECREF(_args);                                                  \
}

#endif //PROMISEDIO_CORE_UVH_H
