// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_FS_API_H
#define PROMISEDIO_FS_API_H

#include "fs.h"

#define Fs_stat(...) \
    Capsule_METH(FS_API_ID, FS_STAT_API_ID, PyObject*, ARGS(const char *), __VA_ARGS__)

#define Fs_lstat(...) \
    Capsule_METH(FS_API_ID, FS_LSTAT_API_ID, PyObject*, ARGS(const char *), __VA_ARGS__)

#define Fs_fstat(...) \
    Capsule_METH(FS_API_ID, FS_FSTAT_API_ID, PyObject*, ARGS(int), __VA_ARGS__)

#define Fs_open(...) \
    Capsule_METH(FS_API_ID, FS_OPEN_API_ID, PyObject*, ARGS(const char *, int, int), __VA_ARGS__)

#define Fs_read(...) \
    Capsule_METH(FS_API_ID, FS_READ_API_ID, PyObject*, ARGS(int, Py_ssize_t, Py_off_t), __VA_ARGS__)

#define Fs_readall(...) \
    Capsule_METH(FS_API_ID, Fs_Readall_API_ID, PyObject*, ARGS(int), __VA_ARGS__)

#define Fs_write(...) \
    Capsule_METH(FS_API_ID, Fs_Write_API_ID, PyObject*, ARGS(int, PyObject*, Py_off_t), __VA_ARGS__)

#define Fs_seek(...) \
    Capsule_METH(FS_API_ID, Fs_Seek_API_ID, PyObject*, ARGS(int, Py_off_t, int), __VA_ARGS__)

#define Fs_close(...) \
    Capsule_METH(FS_API_ID, Fs_Close_API_ID, PyObject*, ARGS(int), __VA_ARGS__)

#define Fs_unlink(...) \
    Capsule_METH(FS_API_ID, Fs_Unlink_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#define Fs_mkdir(...) \
    Capsule_METH(FS_API_ID, Fs_Mkdir_API_ID, PyObject*, ARGS(const char*, int), __VA_ARGS__)

#define Fs_rmdir(...) \
    Capsule_METH(FS_API_ID, Fs_Rmdir_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#define Fs_mkdtemp(...) \
    Capsule_METH(FS_API_ID, Fs_Mkdtemp_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#define Fs_mkstemp(...) \
    Capsule_METH(FS_API_ID, Fs_Mkstemp_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#define Fs_scandir(...) \
    Capsule_METH(FS_API_ID, Fs_Scandir_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#define Fs_rename(...) \
    Capsule_METH(FS_API_ID, Fs_Rename_API_ID, PyObject*, ARGS(const char*, const char*), __VA_ARGS__)

#define Fs_ftruncate(...) \
    Capsule_METH(FS_API_ID, Fs_Ftruncate_API_ID, PyObject*, ARGS(int, Py_ssize_t), __VA_ARGS__)

#define Fs_fsync(...) \
    Capsule_METH(FS_API_ID, Fs_Fsync_API_ID, PyObject*, ARGS(int), __VA_ARGS__)

#define Fs_fdatasync(...) \
    Capsule_METH(FS_API_ID, Fs_Fdatasync_API_ID, PyObject*, ARGS(int), __VA_ARGS__)

#define Fs_copyfile(...) \
    Capsule_METH(FS_API_ID, Fs_Copyfile_API_ID, PyObject*, ARGS(const char*, const char*, int), __VA_ARGS__)

#define Fs_sendfile(...) \
    Capsule_METH(FS_API_ID, Fs_Sendfile_API_ID, PyObject*, ARGS(int, int, Py_off_t, size_t), __VA_ARGS__)

#define Fs_access(...) \
    Capsule_METH(FS_API_ID, Fs_Access_API_ID, PyObject*, ARGS(const char*, int), __VA_ARGS__)

#define Fs_chmod(...) \
    Capsule_METH(FS_API_ID, Fs_Chmod_API_ID, PyObject*, ARGS(const char*, int), __VA_ARGS__)

#define Fs_fchmod(...) \
    Capsule_METH(FS_API_ID, Fs_Fchmod_API_ID, PyObject*, ARGS(int, int), __VA_ARGS__)

#define Fs_utime(...) \
    Capsule_METH(FS_API_ID, Fs_Utime_API_ID, PyObject*, ARGS(const char*, double atime, double mtime), __VA_ARGS__)

#define Fs_futime(...) \
    Capsule_METH(FS_API_ID, Fs_Futime_API_ID, PyObject*, ARGS(int, double, double), __VA_ARGS__)

#define Fs_lutime(...) \
    Capsule_METH(FS_API_ID, Fs_Lutime_API_ID, PyObject*, ARGS(const char*, double, double), __VA_ARGS__)

#define Fs_link(...) \
    Capsule_METH(FS_API_ID, Fs_Link_API_ID, PyObject*, ARGS(const char*, const char*), __VA_ARGS__)

#define Fs_symlink(...) \
    Capsule_METH(FS_API_ID, Fs_Symlink_API_ID, PyObject*, ARGS(const char*, const char*, int flags), __VA_ARGS__)

#define Fs_readlink(...) \
    Capsule_METH(FS_API_ID, Fs_Readlink_API_ID, PyObject*, ARGS(const char*), __VA_ARGS__)

#ifndef MS_WINDOWS

#define Fs_chown(...) \
    Capsule_METH(FS_API_ID, Fs_Chown_API_ID, PyObject*, ARGS(const char*, uv_uid_t, uv_gid_t), __VA_ARGS__)

#define Fs_fchown(...) \
    Capsule_METH(FS_API_ID, Fs_Fchown_API_ID, PyObject*, ARGS(int, uv_uid_t, uv_gid_t), __VA_ARGS__)

#define Fs_lchown(...) \
    Capsule_METH(FS_API_ID, Fs_Lchown_API_ID, PyObject*, ARGS(const char*, uv_uid_t, uv_gid_t), __VA_ARGS__)

#endif

#endif
