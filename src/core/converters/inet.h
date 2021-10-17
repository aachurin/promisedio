// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_CONVERTERS_INET_H
#define PROMISEDIO_CORE_CONVERTERS_INET_H

typedef union {
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
} sockaddr_any;

Py_LOCAL_INLINE(int)
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

#endif //PROMISEDIO_CORE_CONVERTERS_INET_H
