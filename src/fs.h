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
Promise * Fs_stat(const char *path);
Promise * Fs_lstat(const char *path);
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
Promise * Fs_scandir(const char *path);
Promise * Fs_rename(const char *path, const char *new_path);
Promise * Fs_ftruncate(int fd, Py_ssize_t length);
Promise * Fs_fsync(int fd);
Promise * Fs_fdatasync(int fd);
Promise * Fs_copyfile(const char *path, const char *new_path, int flags);
Promise * Fs_sendfile(int out_fd, int in_fd, Py_off_t in_offset, size_t length);
Promise * Fs_access(const char *path, int mode);
Promise * Fs_chmod(const char *path, int mode);
Promise * Fs_fchmod(int fd, int mode);
Promise * Fs_utime(const char *path, double atime, double mtime);
Promise * Fs_futime(int fd, double atime, double mtime);
Promise * Fs_lutime(const char *path, double atime, double mtime);
Promise * Fs_link(const char *path, const char *new_path);
Promise * Fs_symlink(const char *path, const char *new_path, int flags);
Promise * Fs_readlink(const char *path);

#ifndef MS_WINDOWS
Promise * Fs_chown(const char *path, uv_uid_t uid, uv_gid_t gid);
Promise * Fs_fchown(int fd, uv_uid_t uid, uv_gid_t gid);
Promise * Fs_lchown(const char *path, uv_uid_t uid, uv_gid_t gid);
#endif

int Fs_module_init(PyObject *module);

#endif
