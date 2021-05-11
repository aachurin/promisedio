// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef FILEIO_H
#define FILEIO_H

#include <uv.h>
#include "common.h"
#include "promise.h"

typedef struct {
    PyObject_HEAD
    uv_file fd;
    int closefd;
    Py_off_t offset;
    Promise *chain;
} FileIO;

PyObject * FileIO_Open(PyObject *file, const char *flags, int closefd);

int FileIO_module_init();

#endif