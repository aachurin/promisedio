// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef FS_H
#define FS_H

#include <Python.h>
#include <uv.h>
#include "common.h"
#include "promise.h"
#include "loop.h"
#include "memory.h"
#include "c2py.h"

#ifndef MS_WINDOWS
    #ifndef O_CLOEXEC
        #error O_CLOEXEC not supported
    #endif
#endif

Generate_PyType_Header(
    StatObj,
    MEMBER(st, uv_stat_t)
)

PyObject * Fs_Path(PyObject *path);
Promise * Fs_stat(const char *path, int follow_symlinks);
Promise * Fs_fstat(int fd);
Promise * Fs_open(const char *path, const char *flags, int mode);
Promise * Fs_read(int fd, Py_ssize_t size, Py_off_t offset);
Promise * Fs_readall(int fd);
Promise * Fs_write(int fd, PyObject *data, Py_off_t offset);
Promise * Fs_seek(int fd, Py_off_t pos, int how);
Promise * Fs_close(int fd);
Promise * Fs_unlink(const char *path);
Promise * Fs_mkdir(const char *path, int mode);
Promise * Fs_rmdir(const char *path);
Promise * Fs_mkdtemp(const char *tpl);
Promise * Fs_mkstemp(const char *tpl);
int Fs_module_init();

#endif
