// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_NS_H
#define PROMISEDIO_NS_H

#include "core/base.h"
#include "core/capsule.h"
#include "core/module.h"

#define NS_API_ID _ns_API_1_0

#define STREAM_BASE_FIELDS      \
    Py_ssize_t buffer_limit;    \
    Py_ssize_t chunk_min_size;  \
    PyObject *sslcontext;       \
    char *server_hostname;

typedef struct {
    STREAM_BASE_FIELDS
} StreamConfig;

typedef struct {
    STREAM_BASE_FIELDS
    int tcp_nodelay;
    int tcp_keepalive;
} TCPStreamConfig;

#endif
