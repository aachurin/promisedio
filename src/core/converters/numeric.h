// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_CONVERTERS_INT_H
#define PROMISEDIO_CORE_CONVERTERS_INT_H

Py_LOCAL_INLINE(int)
off_t_converter(PyObject *arg, Py_off_t *addr)
{
    Py_off_t ret = PyLong_AsOff_t(arg);
    if (ret == -1 && PyErr_Occurred()) {
        return 0;
    }
    *addr = ret;
    return 1;
}

Py_LOCAL_INLINE(int)
ssize_t_converter(PyObject *arg, Py_ssize_t *addr)
{
    Py_ssize_t ret = PyLong_AsSsize_t(arg);
    if (ret == -1 && PyErr_Occurred())
        return 0;
    *addr = ret;
    return 1;
}

#endif //PROMISEDIO_CORE_CONVERTERS_INT_H
