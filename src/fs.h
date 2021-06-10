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

typedef struct {
    PyObject_HEAD
    uv_stat_t st;
} Stat;

void fs_set_error(int error);
int convert_open_flags(const char *flags, int* open_flags);

Promise * fs_stat(const char *path);
Promise * fs_lstat(const char *path);
Promise * fs_fstat(int fd);
Promise * fs_open(const char *path, int flags, int mode);
Promise * fs_read(int fd, Py_ssize_t size, Py_off_t offset);
Promise * fs_readall(int fd);
Promise * fs_write(int fd, PyObject *data, Py_off_t offset);
Promise * fs_seek(int fd, Py_off_t pos, int how);
Promise * fs_close(int fd);
Promise * fs_unlink(const char *path);
Promise * fs_mkdir(const char *path, int mode);
Promise * fs_rmdir(const char *path);
Promise * fs_mkdtemp(const char *tpl);
Promise * fs_mkstemp(const char *tpl);
Promise * fs_scandir(const char *path);
Promise * fs_rename(const char *path, const char *new_path);
Promise * fs_ftruncate(int fd, Py_ssize_t length);
Promise * fs_fsync(int fd);
Promise * fs_fdatasync(int fd);
Promise * fs_copyfile(const char *path, const char *new_path, int flags);
Promise * fs_sendfile(int out_fd, int in_fd, Py_off_t in_offset, size_t length);
Promise * fs_access(const char *path, int mode);
Promise * fs_chmod(const char *path, int mode);
Promise * fs_fchmod(int fd, int mode);
Promise * fs_utime(const char *path, double atime, double mtime);
Promise * fs_futime(int fd, double atime, double mtime);
Promise * fs_lutime(const char *path, double atime, double mtime);
Promise * fs_link(const char *path, const char *new_path);
Promise * fs_symlink(const char *path, const char *new_path, int flags);
Promise * fs_readlink(const char *path);

#ifndef MS_WINDOWS
Promise * fs_chown(const char *path, uv_uid_t uid, uv_gid_t gid);
Promise * fs_fchown(int fd, uv_uid_t uid, uv_gid_t gid);
Promise * fs_lchown(const char *path, uv_uid_t uid, uv_gid_t gid);
#endif

typedef struct {
    PyObject_HEAD
    uv_file fd;
    int closefd;
} FileIO;

FileIO * fileio_new(int fd, int closefd);
Promise * fs_fileio_open(const char *path, int flags, int mode);

int fs_module_init(PyObject *module);

#endif
