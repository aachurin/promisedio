// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef NS_H
#define NS_H

#include "common.h"
#include "promise.h"
#include <uv.h>

Promise * ns_getaddrinfo(const char *node, const char *service, int family, int type, int proto, int flags);
Promise * ns_getnameinfo(const char *node, const char *service, unsigned int flowinfo, unsigned int scope_id, int flags);

int ns_module_init(PyObject *module);

#endif
