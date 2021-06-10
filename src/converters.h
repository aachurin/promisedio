// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef CONVERTERS_H
#define CONVERTERS_H

static inline int
cstring_converter(PyObject *arg, const char **addr)
{
    Py_ssize_t size;
    const char *ret;
    if (PyUnicode_Check(arg)) {
        ret = PyUnicode_AsUTF8AndSize(arg, &size);
        if (ret == NULL)
            return 0;
    } else if (PyBytes_Check(arg)) {
        ret = PyBytes_AS_STRING(arg);
        size = PyBytes_GET_SIZE(arg);
    } else {
        PyErr_SetString(PyExc_TypeError,
                        "str or bytes expected");
        return 0;
    }
    if (strlen(ret) != (size_t) size) {
        PyErr_SetString(PyExc_ValueError,
                        "embedded null character");
        return 0;
    }
    *addr = ret;
    return 1;
}

static inline int
cstring_optional_converter(PyObject *arg, const char **addr)
{
    if (arg == Py_None) {
        *addr = NULL;
        return 1;
    }
    return cstring_converter(arg, addr);
}

static inline int
off_t_converter(PyObject *arg, Py_off_t *addr)
{
    Py_off_t ret = PyLong_AsOff_t(arg);
    if (ret == -1 && PyErr_Occurred()) {
        return 0;
    }
    *addr = ret;
    return 1;
}

static inline int
ssize_t_converter(PyObject *arg, Py_ssize_t *addr)
{
    Py_ssize_t ret = PyLong_AsSsize_t(arg);
    if (ret == -1 && PyErr_Occurred())
        return 0;
    *addr = ret;
    return 1;
}

static inline int
fd_converter(PyObject *arg, int *addr)
{
    int ret = _PyLong_AsInt(arg);
    if (ret < 0) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "negative file descriptor");
        }
        return 0;
    }
    *addr = ret;
    return 1;
}

#endif
