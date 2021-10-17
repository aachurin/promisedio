// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_FS_H
#define PROMISEDIO_FS_H

#include "core/base.h"
#include "core/capsule.h"
#include "core/module.h"

#define FS_API_ID _fs_API_1_0

#define FS_STAT_API_ID          0
#define FS_LSTAT_API_ID         1
#define FS_FSTAT_API_ID         2
#define FS_OPEN_API_ID          3
#define FS_READ_API_ID          4
#define FS_READALL_API_ID       5
#define FS_WRITE_API_ID         6
#define FS_SEEK_API_ID          7
#define FS_CLOSE_API_ID         8
#define FS_UNLINK_API_ID        9
#define FS_MKDIR_API_ID         10
#define FS_RMDIR_API_ID         11
#define FS_MKDTEMP_API_ID       12
#define FS_MKSTEMP_API_ID       13
#define FS_SCANDIR_API_ID       14
#define FS_RENAME_API_ID        15
#define FS_FTRUNCATE_API_ID     16
#define FS_FSYNC_API_ID         17
#define FS_FDATASYNC_API_ID     18
#define FS_COPYFILE_API_ID      19
#define FS_SENDFILE_API_ID      20
#define FS_ACCESS_API_ID        21
#define FS_CHMOD_API_ID         22
#define FS_FCHMOD_API_ID        23
#define FS_UTIME_API_ID         24
#define FS_FUTIME_API_ID        25
#define FS_LUTIME_API_ID        26
#define FS_LINK_API_ID          27
#define FS_SYMLINK_API_ID       28
#define FS_READLINK_API_ID      29
#ifndef MS_WINDOWS
#define FS_CHOWN_API_ID         30
#define FS_FCHOWN_API_ID        31
#define FS_LCHOWN_API_ID        32
#endif

#endif
