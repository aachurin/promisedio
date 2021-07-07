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

typedef union {
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
} sockaddr_any;

static inline int
inet_addr_converter(PyObject *arg, sockaddr_any *addr)
{
    const char *ipaddr;
    if (!PyTuple_Check(arg)) {
        PyErr_SetString(PyExc_TypeError, "sockaddr must be a tuple");
        return 0;
    }
    Py_ssize_t sockaddr_size = PyTuple_GET_SIZE(arg);
    if (sockaddr_size < 2 || sockaddr_size > 4) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument");
        return 0;
    }
    if (!cstring_converter(PyTuple_GET_ITEM(arg, 0), &ipaddr)) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid ip or ip6 address");
        return 0;
    }

    if (!PyLong_Check(PyTuple_GET_ITEM(arg, 1))) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid port");
        return 0;
    }

    unsigned short port;
    if (!_PyLong_UnsignedShort_Converter(PyTuple_GET_ITEM(arg, 1), &port)) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid port");
        return 0;
    }

    if (sockaddr_size == 2) {
        if (!uv_ip4_addr(ipaddr, port, (struct sockaddr_in *) addr)) {
            return 1;
        }
        if (!uv_ip6_addr(ipaddr, port, (struct sockaddr_in6 *) addr)) {
            return 1;
        }
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid ip or ip6 address");
        return 0;
    }
    if (uv_ip6_addr(ipaddr, port, (struct sockaddr_in6 *) addr)) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid ip6 address");
        return 0;
    }
    unsigned int flowinfo = 0, scope_id = 0;
    if (!_PyLong_UnsignedInt_Converter(PyTuple_GET_ITEM(arg, 2), &flowinfo)) {
        PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid flowinfo");
        return 0;
    }
    if (sockaddr_size > 3) {
        if (!_PyLong_UnsignedInt_Converter(PyTuple_GET_ITEM(arg, 3), &scope_id)) {
            PyErr_SetString(PyExc_ValueError, "illegal sockaddr argument, invalid scope_id");
            return 0;
        }
    }
    ((struct sockaddr_in6 *) addr)->sin6_flowinfo = flowinfo;
    if (((struct sockaddr_in6 *) addr)->sin6_scope_id == 0) {
        ((struct sockaddr_in6 *) addr)->sin6_scope_id = scope_id;
    }
    return 1;
}

#endif
