// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef NS_H
#define NS_H

#include "common.h"
#include "promise.h"

Promise * ns_getaddrinfo(const char *node, const char *service, int family, int type, int proto, int flags);
Promise * ns_getnameinfo(const struct sockaddr *addr, int flags);
Promise * ns_tcp_connect(const struct sockaddr *addr, int tcp_nodelay, int tcp_keepalive, Py_ssize_t buffer_limit);

int ns_module_init(PyObject *module);

#endif
