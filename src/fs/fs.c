// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "core/base.h"
#include "core/capsule.h"
#include "core/handle.h"
#include "core/memory.h"
#include "core/module.h"
#include "core/uvh.h"
#include "core/converters/numeric.h"
#include "core/converters/file.h"
#include "core/converters/string.h"
#include "fs.h"

/*[clinic input]
module fs
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=de53127b95d08f91]*/

typedef struct {
    PyObject *FSError;
    PyTypeObject *StatType;
    PyTypeObject *FileIOType;
    Capsule_MOUNT(PROMISE_API)
    Capsule_MOUNT(LOOP_API)
} _modulestate;

#if BUFSIZ < (8 * 1024)
#define SMALLCHUNK (8 * 1024)
#elif (BUFSIZ >= (2 << 25))
#error "unreasonable BUFSIZ > 64 MiB defined"
#else
#define SMALLCHUNK BUFSIZ
#endif

#define Request_CloseFs(req) \
    uv_fs_req_cleanup(req);  \
    Request_Close(req)

#define FS_CALL(loop, promise, func, ...)           \
    Request_SETUP(uv_fs_t, _req, promise)           \
    if (_req) {                                     \
        UV_CALL(func, loop, _req, __VA_ARGS__)      \
        {                                           \
            Request_CloseFs(_req);                  \
            Py_SetUVError(S(FSError), uv_errno);    \
            Py_DECREF(promise);                     \
            promise = NULL;                         \
        }                                           \
    }

typedef struct {
    PyObject_HEAD
    uv_file fd;
    int closefd;
    _ctx_var;
} FileIO;

typedef struct {
    PyObject_HEAD
    uv_stat_t st;
} Stat;

#include "clinic/fs.c.h"

#define Promise_RejectFsExc(promise, uverr) Promise_RejectUVError(promise, S(FSError), uverr)

static void
int_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *value = PyLong_FromSsize_t(req->result);
            if (!value) {
                Promise_Reject(p, NULL);
            } else {
                Promise_Resolve(p, value);
                Py_DECREF(value);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
none_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            Promise_Resolve(p, Py_None);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
bool_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_Resolve(p, Py_False);
        } else {
            Promise_Resolve(p, Py_True);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
path_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *result = PyUnicode_DecodeFSDefault(req->path);
            if (!result) {
                Promise_Reject(p, NULL);
            } else {
                Promise_Resolve(p, result);
                Py_DECREF(result);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
ptr_path_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *result = PyUnicode_DecodeFSDefault(req->ptr);
            if (!result) {
                Promise_Reject(p, NULL);
            } else {
                Promise_Resolve(p, result);
                Py_DECREF(result);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
stat_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            Stat *obj = (Stat *) Py_New(S(StatType));
            if (!obj) {
                Promise_Reject(p, NULL);
            } else {
                PyTrack_MarkAllocated(obj);
                obj->st = req->statbuf;
                Promise_Resolve(p, (PyObject *) obj);
                Py_DECREF(obj);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Stat(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_stat, path, stat_callback)
    return promise;
}

CAPSULE_API(FS_API, Promise *)
Fs_Lstat(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_lstat, path, stat_callback)
    return promise;
}

/*[clinic input]
fs.stat
    path: Path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_stat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=3ca9a0ff706b0282 input=86f245764cee8a69]*/
{
    _CTX_set_module(module);
    if (follow_symlinks) {
        return (PyObject *) Fs_Stat(_ctx, PyBytes_AS_STRING(path));
    } else {
        return (PyObject *) Fs_Lstat(_ctx, PyBytes_AS_STRING(path));
    }
}

CAPSULE_API(FS_API, Promise *)
Fs_Fstat(_ctx_var, int fd)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_fstat, fd, stat_callback)
    return promise;
}

/*[clinic input]
fs.fstat
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_fstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=6009b13fb6a840ae input=5d9d8127d47ae9f2]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Fstat(_ctx, fd);
}

typedef struct {
    uv_work_t base;
    uv_file fd;
    Py_off_t pos;
    int whence;
    Py_off_t result;
} SeekReq;

static void
seek_work(SeekReq *req)
{
    Py_off_t pos;
#ifdef MS_WINDOWS
    // python uses _Py_BEGIN_SUPPRESS_IPH/_Py_END_SUPPRESS_IPH, need we the same calls?
    pos = _lseeki64(req->fd, req->pos, req->whence);
#else
    pos = lseek(req->fd, req->pos, req->whence);
#endif
    if (pos == -1) {
#ifdef MS_WINDOWS
        req->result = uv_translate_sys_error(GetLastError());
#else
        req->result = uv_translate_sys_error(errno);
#endif
    } else {
        req->result = pos;
    }
}

static int
fs__seek(uv_loop_t *loop, SeekReq *req, uv_file fd, Py_off_t pos, int how, uv_after_work_cb cb)
{
#ifdef SEEK_SET
    /* Turn 0, 1, 2 into SEEK_{SET,CUR,END} */
    switch (how) {// NOLINT(hicpp-multiway-paths-covered)
        case 0:
            how = SEEK_SET;
            break;
        case 1:
            how = SEEK_CUR;
            break;
        case 2:
            how = SEEK_END;
            break;
    }
#endif
    req->fd = fd;
    req->pos = pos;
    req->whence = how;
    uv_queue_work(loop, (uv_work_t *) req, (uv_work_cb) seek_work, cb);
    return 0;
}

static void
seek_callback(SeekReq *req, int status)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE((uv_req_t *) req);
        if (status == UV_ECANCELED) {
            Promise_RejectFsExc(p, status);
        } else if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *value = PyLong_FromOff_t(req->result);
            if (!value) {
                Promise_Reject(p, NULL);
            } else {
                Promise_Resolve(p, value);
                Py_DECREF(value);
            }
        }
        Request_Close(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Seek(_ctx_var, int fd, Py_off_t pos, int how)
{
    Loop_SETUP(loop)
    Request_SETUP(SeekReq, req, promise)
    if (req) {
        UV_CALL(fs__seek, loop, req, fd, pos, how, (uv_after_work_cb) seek_callback) {
        };
    }
    return promise;
}

/*[clinic input]
fs.seek
    fd: fd
    pos: off_t
    how: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_seek_impl(PyObject *module, int fd, Py_off_t pos, int how)
/*[clinic end generated code: output=ef51eb2dceb31d78 input=3c990ca37e7bc185]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Seek(_ctx, fd, pos, how);
}

CAPSULE_API(FS_API, Promise *)
Fs_Open(_ctx_var, const char *path, int flags, int mode)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_open, path, flags, mode, int_callback)
    return promise;
}

/*[clinic input]
fs.openfd
    path: Path
    flags: int
    mode: int = 0o666
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_openfd_impl(PyObject *module, PyObject *path, int flags, int mode)
/*[clinic end generated code: output=ea8de6fdaaadaf08 input=b662728d8f4b901e]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Open(_ctx, PyBytes_AS_STRING(path), flags, mode);
}

static PyObject *
fileio_new(_ctx_var, int fd, int closefd)
{
    FileIO *self = (FileIO *) Py_New(S(FileIOType));
    if (!self)
        return NULL;
    PyTrack_MarkAllocated(self);
    self->fd = fd;
    self->closefd = closefd;
    _CTX_save(self);
    return (PyObject *) self;
}

static void
open_stat_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
            goto finally;
        }
#if defined(S_ISDIR)
        if (S_ISDIR(req->statbuf.st_mode)) {
            Promise_RejectFsExc(p, UV_EISDIR);
            goto finally;
        }
#endif
        PyObject *fileobj = fileio_new(_ctx, *Promise_DATA(p, uv_file), 1);
        if (!fileobj) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        Promise_Resolve(p, fileobj);
        Py_DECREF(fileobj);

    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static void
open_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
            goto finally;
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, p);
        if (!nreq) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        uv_file fd = (uv_file) req->result;
        *Promise_DATA(p, uv_file) = fd;
        UV_CALL(uv_fs_fstat, req->loop, nreq, fd, open_stat_callback) {
            Request_CloseFs(nreq);
            Promise_RejectFsExc(p, uv_errno);
        }
    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static Promise *
fs_fileio_open(_ctx_var, const char *path, int flags)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_open, path, flags, 0666, open_callback)
    return promise;
}

/*[clinic input]
fs.open
    path: object
    mode: cstring = "r"
    closefd: bool(accept={int}) = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_open_impl(PyObject *module, PyObject *path, const char *mode, int closefd)
/*[clinic end generated code: output=d0e1b7085b442b73 input=d1f22ba3363fc2df]*/
{
    _CTX_set_module(module);
    int flags = 0, writable = 0, readable = 0;

    for (unsigned long i = 0; i < strlen(mode); i++) {
        char c = mode[i];
        switch (c) {
            case 'x':
                if (writable || readable) {
                bad_mode:
                    PyErr_SetString(PyExc_ValueError,
                                    "must have exactly one of create/read/write/append mode");
                    return NULL;
                }
                writable = 1;
                flags |= UV_FS_O_EXCL | UV_FS_O_CREAT;
                break;
            case 'r':
                if (writable || readable) {
                    goto bad_mode;
                }
                readable = 1;
                break;
            case 'w':
                if (writable || readable) {
                    goto bad_mode;
                }
                writable = 1;
                flags |= UV_FS_O_CREAT | UV_FS_O_TRUNC;
                break;
            case 'a':
                if (writable || readable) {
                    goto bad_mode;
                }
                writable = 1;
                flags |= UV_FS_O_APPEND | UV_FS_O_CREAT;
                break;
            case '+':
                writable = 1;
                break;
            case 'b':
                break;
            default:
                goto invalid_mode;
        }

        /* c must not be duplicated */
        if (strchr(mode + i + 1, c)) {
        invalid_mode:
            PyErr_Format(PyExc_ValueError, "invalid mode: '%s'", mode);
            return NULL;
        }
    }

    if (readable && writable) {
        flags |= UV_FS_O_RDWR;
    } else if (readable) {
        flags |= UV_FS_O_RDONLY;
    } else if (writable) {
        flags |= UV_FS_O_WRONLY;
    } else {
        goto bad_mode;
    }

#ifdef O_NOINHERIT
    flags |= O_NOINHERIT;
#endif

    if (PyNumber_Check(path)) {
        int fd = _PyLong_AsInt(path);
        if (fd < 0) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "negative file descriptor");
            }
            return NULL;
        }
        PyObject *fileobj = fileio_new(_ctx, fd, closefd);
        if (!fileobj)
            return NULL;
        return (PyObject *) Promise_NewResolved(fileobj, NULL);
    }

    if (!closefd) {
        PyErr_SetString(PyExc_ValueError, "Cannot use closefd=False with file name");
        return NULL;
    }

    PyObject *fspath;
    if (!PyUnicode_FSConverter(path, &fspath)) {
        return NULL;
    }

    Promise *result = fs_fileio_open(_ctx, PyBytes_AS_STRING(fspath), flags);
    Py_DECREF(fspath);

    return (PyObject *) result;
}

Py_LOCAL_INLINE(int)
fs__read(uv_loop_t *loop, uv_fs_t *req, uv_file fd, char *buffer, size_t size, Py_off_t offset, uv_fs_cb cb)
{
    uv_buf_t buf;
    buf.base = buffer;
    buf.len = size > _PY_READ_MAX ? _PY_READ_MAX : size;
    return uv_fs_read(loop, req, fd, &buf, 1, offset, cb);
}

static void
read_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        PyObject *buffer = Promise_SetCtx(p, NULL);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
            goto finally;
        }
        Py_ssize_t size = req->result;
        if (size < PyBytes_GET_SIZE(buffer)) {
            PyTrack_MarkBeginResize(buffer);
            if (_PyBytes_Resize(&buffer, size)) {
                Promise_Reject(p, NULL);
                goto finally;
            }
            PyTrack_MarkEndResize(buffer);
        }
        Promise_Resolve(p, buffer);
    finally:
        PyTrack_XDECREF(buffer);
        Request_CloseFs(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Read(_ctx_var, int fd, Py_ssize_t size, Py_off_t offset)
{
    if (size > _PY_READ_MAX) {
        size = _PY_READ_MAX;
    }
    Loop_SETUP(loop)
    PyObject *buffer = PyBytes_FromStringAndSize(NULL, size);
    if (!buffer)
        return NULL;
    char *buffer_ptr = PyBytes_AS_STRING(buffer);
    FS_CALL(loop, promise, fs__read, fd, buffer_ptr, size, offset, read_callback)
    if (!promise) {
        Py_DECREF(buffer);
        return NULL;
    }
    PyTrack_MarkAllocated(buffer);
    Promise_SetCtx(promise, buffer);
    return promise;
}

static size_t
new_buffersize(size_t currentsize)
{
    size_t addend;
    /* Expand the buffer by an amount proportional to the current size,
       giving us amortized linear-time behavior. For bigger sizes, use a
       less-than-double growth factor to avoid excessive allocation. */
    assert(currentsize <= PY_SSIZE_T_MAX);
    if (currentsize > 65536)
        addend = currentsize >> 3;
    else
        addend = 256 + currentsize;
    if (addend < SMALLCHUNK)
        /* Avoid tiny read() calls. */
        addend = SMALLCHUNK;
    return addend + currentsize;
}

typedef struct {
    uv_file fd;
    PyObject *buffer;
    Py_ssize_t bytes_read;
    Py_off_t pos;
} readallstate;

static void
readall_read_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        readallstate *readstate = Promise_DATA(p, readallstate);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        cleanup:
            PyTrack_DECREF(readstate->buffer);
            goto finally;
        }
        if (req->result == 0) {
            if (PyBytes_GET_SIZE(readstate->buffer) > readstate->bytes_read) {
                PyTrack_MarkBeginResize(readstate->buffer);
                if (_PyBytes_Resize(&readstate->buffer, readstate->bytes_read) < 0) {
                    Promise_Reject(p, NULL);
                    goto finally;
                }
                PyTrack_MarkEndResize(readstate->buffer);
            }
            Promise_Resolve(p, readstate->buffer);
            goto cleanup;
        }
        readstate->bytes_read += req->result;
        if (readstate->bytes_read >= PyBytes_GET_SIZE(readstate->buffer)) {
            size_t new_size = new_buffersize(readstate->bytes_read);
            if (new_size > PY_SSIZE_T_MAX) {
                Promise_RejectString(p, PyExc_OverflowError,
                                     "unbounded read returned more bytes "
                                     "than a Python bytes object can hold");
                goto cleanup;
            }
            if (_PyBytes_Resize(&readstate->buffer, (Py_ssize_t) new_size) < 0) {
                Promise_Reject(p, NULL);
                goto finally;
            }
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, p);
        if (!nreq) {
            Promise_Reject(p, NULL);
            goto cleanup;
        }
        char *buf = PyBytes_AS_STRING(readstate->buffer) + readstate->bytes_read;
        Py_ssize_t size = PyBytes_GET_SIZE(readstate->buffer) - readstate->bytes_read;
        UV_CALL(fs__read, req->loop, nreq, readstate->fd, buf, size, -1, readall_read_callback) {
            Request_CloseFs(nreq);
            Promise_RejectFsExc(p, uv_errno);
            goto cleanup;
        }
    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static void
readall_stat_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        readallstate *readstate = Promise_DATA(p, readallstate);
        Py_off_t pos = readstate->pos;
        Py_off_t end;
        size_t bufsize;
        if (req->result < 0) {
            end = (Py_off_t) -1;
        } else {
            end = (Py_off_t) req->statbuf.st_size;
        }
        if (end > 0 && end >= pos && pos >= 0 && end - pos < PY_SSIZE_T_MAX) {
            /* This is probably a real file, so we try to allocate a
                   buffer one byte larger than the rest of the file.  If the
                   calculation is right then we should get EOF without having
                   to enlarge the buffer. */
            bufsize = (size_t) end - pos + 1;
        } else {
            bufsize = SMALLCHUNK;
        }
        PyObject *buffer = PyBytes_FromStringAndSize(NULL, (Py_ssize_t) bufsize);
        if (!buffer) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, p);
        if (!nreq) {
            Promise_Reject(p, NULL);
        cleanup:
            Py_DECREF(buffer);
            goto finally;
        }
        readstate->buffer = buffer;
        char *buffer_ptr = PyBytes_AS_STRING(buffer);
        UV_CALL(fs__read, req->loop, nreq, readstate->fd, buffer_ptr, bufsize, -1, readall_read_callback) {
            Request_CloseFs(nreq);
            Promise_RejectFsExc(p, uv_errno);
            goto cleanup;
        }
        PyTrack_MarkAllocated(buffer);
    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static void
readall_seek_callback(SeekReq *req, int status)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE((uv_req_t *) req);
        if (status == UV_ECANCELED) {
            Promise_RejectFsExc(p, status);
            goto finally;
        }
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
            goto finally;
        }
        readallstate *readstate = Promise_DATA(p, readallstate);
        readstate->pos = req->result;
        uv_fs_t *nreq = Request_New(uv_fs_t, p);
        if (!nreq) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        UV_CALL(uv_fs_fstat, req->base.loop, nreq, readstate->fd, readall_stat_callback) {
            Request_CloseFs(nreq);
            Promise_RejectFsExc(p, uv_errno);
        }
    finally:
        Request_Close(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Readall(_ctx_var, int fd)
{
    Loop_SETUP(loop)
    Request_SETUP(SeekReq, req, promise)
    if (req) {
        *Promise_DATA(promise, readallstate) = (readallstate) {.fd = fd, .pos = 0, .bytes_read = 0};
        UV_CALL(fs__seek, loop, req, fd, 0, 1, (uv_after_work_cb) readall_seek_callback) {
        }
    }
    return promise;
}

/*[clinic input]
fs.read
    fd: fd
    size: ssize_t = -1
    offset: off_t = -1
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_read_impl(PyObject *module, int fd, Py_ssize_t size, Py_off_t offset)
/*[clinic end generated code: output=04e8e196783beea5 input=a4e8469faca2f474]*/
{
    _CTX_set_module(module);
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be specified in conjunction with the size parameter");
            return NULL;
        }
        return (PyObject *) Fs_Readall(_ctx, fd);
    } else {
        return (PyObject *) Fs_Read(_ctx, fd, size, offset);
    }
}

Py_LOCAL_INLINE(int)
fs__write(uv_loop_t *loop, uv_fs_t *req, uv_file fd, char *buffer, size_t size, Py_off_t offset, uv_fs_cb cb)
{
    uv_buf_t buf;
    buf.base = buffer;
    buf.len = size > _PY_READ_MAX ? _PY_READ_MAX : size;
    return uv_fs_write(loop, req, fd, &buf, 1, offset, cb);
}

static void
write_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *size = PyLong_FromSsize_t(req->result);
            Promise_Resolve(p, size);
            Py_DECREF(size);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Write(_ctx_var, int fd, PyObject *data, Py_off_t offset)
{
    Loop_SETUP(loop)
    char *data_ptr = PyBytes_AsString(data);
    Py_ssize_t data_size = PyBytes_GET_SIZE(data);
    FS_CALL(loop, promise, fs__write, fd, data_ptr, data_size, offset, write_callback)
    if (promise) {
        PyTrack_INCREF(data);
        Promise_SetCtx(promise, data);
    }
    return promise;
}

/*[clinic input]
fs.write
    fd: fd
    data: object
    offset: off_t = -1
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_write_impl(PyObject *module, int fd, PyObject *data, Py_off_t offset)
/*[clinic end generated code: output=1a454c115dbc90ed input=534300ee283e827f]*/
{
    _CTX_set_module(module);
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_Write(_ctx, fd, data, offset);
}

CAPSULE_API(FS_API, Promise *)
Fs_Close(_ctx_var, int fd)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_close, fd, none_callback)
    return promise;
}

/*[clinic input]
fs.close
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_close_impl(PyObject *module, int fd)
/*[clinic end generated code: output=f901b196ba9112b2 input=9924e6ef7e6d1220]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Close(_ctx, fd);
}

CAPSULE_API(FS_API, Promise *)
Fs_Unlink(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_unlink, path, none_callback)
    return promise;
}

/*[clinic input]
fs.unlink
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_unlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=7ad634ae2068c296 input=cb4c0bd1f0318581]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Unlink(_ctx, PyBytes_AS_STRING(path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Mkdir(_ctx_var, const char *path, int mode)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_mkdir, path, mode, none_callback)
    return promise;
}

/*[clinic input]
fs.mkdir
    path: Path
    mode: int = 0o777
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_mkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=cc6ca10eed6b5de4 input=98d19f5dc82fe45e]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Mkdir(_ctx, PyBytes_AS_STRING(path), mode);
}

CAPSULE_API(FS_API, Promise *)
Fs_Rmdir(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_rmdir, path, none_callback)
    return promise;
}

/*[clinic input]
fs.rmdir
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_rmdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=8a899641fa9fc5cb input=ea608c81e46fdc83]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Rmdir(_ctx, PyBytes_AS_STRING(path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Mkdtemp(_ctx_var, const char *tpl)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_mkdtemp, tpl, path_callback)
    return promise;
}

/*[clinic input]
fs.mkdtemp
    tpl: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_mkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=4845725af98ca58c input=0cb22bb81ab08559]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Mkdtemp(_ctx, PyBytes_AS_STRING(tpl));
}

static void
mkstemp_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
        } else {
            PyObject *result = Py_BuildValue("ns", req->result, req->path);
            if (result) {
                Promise_Resolve(p, result);
                Py_DECREF(result);
            } else {
                Promise_Reject(p, NULL);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Mkstemp(_ctx_var, const char *tpl)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_mkstemp, tpl, mkstemp_callback)
    return promise;
}

/*[clinic input]
fs.mkstemp
    tpl: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_mkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=eb08f9d67d79bb6f input=78c43098a049ed25]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Mkstemp(_ctx, PyBytes_AS_STRING(tpl));
}

static void
scandir_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (req->result < 0) {
            Promise_RejectFsExc(p, (int) req->result);
            goto finally;
        }
        Py_ssize_t n = req->result;
        PyObject *result = PyTuple_New(n);
        if (!result) {
        error:
            Promise_Reject(p, NULL);
            goto finally;
        }
        if (n) {
            uv_dirent_t dent;
            Py_ssize_t index = 0;
            while (UV_EOF != uv_fs_scandir_next(req, &dent)) {
                int type = 0;
                switch (dent.type) {
                    case UV_DIRENT_UNKNOWN:
                        type = 0;
                        break;
                    case UV_DIRENT_FILE:
                        type = 1;
                        break;
                    case UV_DIRENT_DIR:
                        type = 2;
                        break;
                    case UV_DIRENT_LINK:
                        type = 3;
                        break;
                    case UV_DIRENT_FIFO:
                        type = 4;
                        break;
                    case UV_DIRENT_SOCKET:
                        type = 5;
                        break;
                    case UV_DIRENT_CHAR:
                        type = 6;
                        break;
                    case UV_DIRENT_BLOCK:
                        type = 7;
                        break;
                }
                PyObject *item = Py_BuildValue("is", type, dent.name);
                if (!item) {
                    Py_DECREF(result);
                    goto error;
                }
                PyTuple_SET_ITEM(result, index, item);
                index++;
            }
        }
        Promise_Resolve(p, result);
        Py_DECREF(result);
    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

CAPSULE_API(FS_API, Promise *)
Fs_Scandir(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_scandir, path, 0, scandir_callback)
    return promise;
}

/*[clinic input]
fs.scandir
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_scandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=4a789c9f969b08a3 input=cada47c048b838f2]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Scandir(_ctx, PyBytes_AS_STRING(path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Rename(_ctx_var, const char *path, const char *new_path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_rename, path, new_path, none_callback)
    return promise;
}

/*[clinic input]
fs.rename
    path: Path
    new_path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_rename_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=1eb79ef01e1cc6eb input=d10ca1dea1c36ca9]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Rename(_ctx, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Fsync(_ctx_var, int fd)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_fsync, fd, none_callback)
    return promise;
}

/*[clinic input]
fs.fsync
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_fsync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=77234dc5eba48e15 input=1dd4da63ef5f4b34]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Fsync(_ctx, fd);
}

CAPSULE_API(FS_API, Promise *)
Fs_Ftruncate(_ctx_var, int fd, Py_ssize_t length)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_ftruncate, fd, length, none_callback)
    return promise;
}

/*[clinic input]
fs.ftruncate
    fd: fd
    length: ssize_t
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length)
/*[clinic end generated code: output=4c3adf479c59bac5 input=3037b1c14c63fd76]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Ftruncate(_ctx, fd, length);
}

CAPSULE_API(FS_API, Promise *)
Fs_Fdatasync(_ctx_var, int fd)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_fdatasync, fd, none_callback)
    return promise;
}

/*[clinic input]
fs.fdatasync
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_fdatasync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=1172614bb569d996 input=12f7058927d6d90c]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Fdatasync(_ctx, fd);
}

CAPSULE_API(FS_API, Promise *)
Fs_Copyfile(_ctx_var, const char *path, const char *new_path, int flags)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_copyfile, path, new_path, flags, none_callback)
    return promise;
}

/*[clinic input]
fs.copyfile
    path: Path
    new_path: Path
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_copyfile_impl(PyObject *module, PyObject *path, PyObject *new_path,
                 int flags)
/*[clinic end generated code: output=f26db733a792e8d0 input=18e6000f66ed993e]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Copyfile(_ctx, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

CAPSULE_API(FS_API, Promise *)
Fs_Sendfile(_ctx_var, int out_fd, int in_fd, Py_off_t in_offset, size_t length)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_sendfile, out_fd, in_fd, in_offset, length, int_callback)
    return promise;
}

/*[clinic input]
fs.sendfile
    out_fd: fd
    in_fd: fd
    offset: off_t
    count: ssize_t
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_sendfile_impl(PyObject *module, int out_fd, int in_fd, Py_off_t offset,
                 Py_ssize_t count)
/*[clinic end generated code: output=ac27efde8b1056f5 input=7147bf805abbfeab]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Sendfile(_ctx, out_fd, in_fd, offset, count);
}

CAPSULE_API(FS_API, Promise *)
Fs_Access(_ctx_var, const char *path, int mode)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_access, path, mode, bool_callback)
    return promise;
}

/*[clinic input]
fs.access
    path: Path
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_access_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=66baaaf393f5d323 input=bceb734cd1fe2473]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Access(_ctx, PyBytes_AS_STRING(path), mode);
}

CAPSULE_API(FS_API, Promise *)
Fs_Chmod(_ctx_var, const char *path, int mode)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_chmod, path, mode, none_callback)
    return promise;
}

/*[clinic input]
fs.chmod
    path: Path
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_chmod_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=c96185fc378f4240 input=5ad22c77ad1094ae]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Chmod(_ctx, PyBytes_AS_STRING(path), mode);
}

CAPSULE_API(FS_API, Promise *)
Fs_Fchmod(_ctx_var, int fd, int mode)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_fchmod, fd, mode, none_callback)
    return promise;
}

/*[clinic input]
fs.fchmod
    fd: fd
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_fchmod_impl(PyObject *module, int fd, int mode)
/*[clinic end generated code: output=34682ca7db418d11 input=57ef52ad97618eb2]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Fchmod(_ctx, fd, mode);
}

CAPSULE_API(FS_API, Promise *)
Fs_Utime(_ctx_var, const char *path, double atime, double mtime)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_utime, path, atime, mtime, none_callback)
    return promise;
}

CAPSULE_API(FS_API, Promise *)
Fs_Lutime(_ctx_var, const char *path, double atime, double mtime)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_lutime, path, atime, mtime, none_callback)
    return promise;
}

/*[clinic input]
fs.utime
    path: Path
    atime: double
    mtime: double,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_utime_impl(PyObject *module, PyObject *path, double atime, double mtime,
              int follow_symlinks)
/*[clinic end generated code: output=935a5e77f4392a74 input=d2d2ada07f37e8ac]*/
{
    _CTX_set_module(module);
    if (follow_symlinks) {
        return (PyObject *) Fs_Utime(_ctx, PyBytes_AS_STRING(path), atime, mtime);
    } else {
        return (PyObject *) Fs_Lutime(_ctx, PyBytes_AS_STRING(path), atime, mtime);
    }
}

CAPSULE_API(FS_API, Promise *)
Fs_Futime(_ctx_var, int fd, double atime, double mtime)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_futime, fd, atime, mtime, none_callback)
    return promise;
}

/*[clinic input]
fs.futime
    fd: fd
    atime: double
    mtime: double
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_futime_impl(PyObject *module, int fd, double atime, double mtime)
/*[clinic end generated code: output=f4620d30861b1bf1 input=005fcf64445c1b4b]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Futime(_ctx, fd, atime, mtime);
}

CAPSULE_API(FS_API, Promise *)
Fs_Link(_ctx_var, const char *path, const char *new_path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_link, path, new_path, none_callback)
    return promise;
}

/*[clinic input]
fs.link
    path: Path
    new_path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_link_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=2a0f7745bbf5e142 input=a1c3d02af0f400c2]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Link(_ctx, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Symlink(_ctx_var, const char *path, const char *new_path, int flags)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_symlink, path, new_path, flags, none_callback)
    return promise;
}

/*[clinic input]
fs.symlink
    path: Path
    new_path: Path
    *
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                int flags)
/*[clinic end generated code: output=7dda80446611250f input=3b2df6695e025ce0]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Symlink(_ctx, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

CAPSULE_API(FS_API, Promise *)
Fs_Readlink(_ctx_var, const char *path)
{
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_readlink, path, ptr_path_callback)
    return promise;
}

/*[clinic input]
fs.readlink
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_readlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=1a033f553e3cc0df input=2e4577d2fc1fce38]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Readlink(_ctx, PyBytes_AS_STRING(path));
}

CAPSULE_API(FS_API, Promise *)
Fs_Chown(_ctx_var, const char *path, uv_uid_t uid, uv_gid_t gid)
{
#ifndef MS_WINDOWS
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_chown, path, uid, gid, none_callback)
    return promise;
#else
    PyErr_SetString(PyExc_NotImplementedError, "Not supported");
    return NULL;
#endif
}

CAPSULE_API(FS_API, Promise *)
Fs_Lchown(_ctx_var, const char *path, uv_uid_t uid, uv_gid_t gid)
{
#ifndef MS_WINDOWS
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_lchown, path, uid, gid, none_callback)
    return promise;
#else
    PyErr_SetString(PyExc_NotImplementedError, "Not supported");
    return NULL;
#endif
}

/*[clinic input]
fs.chown
    path: Path
    uid: uid_t,
    gid: gid_t,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_chown_impl(PyObject *module, PyObject *path, uid_t uid, gid_t gid,
              int follow_symlinks)
/*[clinic end generated code: output=b4f92ec2c249651f input=1c9528d26ba50b43]*/
{
    _CTX_set_module(module);
    if (follow_symlinks) {
        return (PyObject *) Fs_Chown(_ctx, PyBytes_AS_STRING(path), uid, gid);
    } else {
        return (PyObject *) Fs_Lchown(_ctx, PyBytes_AS_STRING(path), uid, gid);
    }
}

CAPSULE_API(FS_API, Promise *)
Fs_Fchown(_ctx_var, int fd, uv_uid_t uid, uv_gid_t gid)
{
#ifndef MS_WINDOWS
    Loop_SETUP(loop)
    FS_CALL(loop, promise, uv_fs_fchown, fd, uid, gid, none_callback)
    return promise;
#else
    PyErr_SetString(PyExc_NotImplementedError, "Not supported");
    return NULL;
#endif
}

/*[clinic input]
fs.fchown
    fd: fd
    uid: uid_t,
    gid: gid_t,
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_fchown_impl(PyObject *module, int fd, uid_t uid, gid_t gid)
/*[clinic end generated code: output=08465d9f9297e283 input=be2d9ec2487e007f]*/
{
    _CTX_set_module(module);
    return (PyObject *) Fs_Fchown(_ctx, fd, uid, gid);
}

static PyObject *
Stat_st_dev(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_dev);
}

static PyObject *
Stat_st_mode(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_mode);
}

static PyObject *
Stat_st_nlink(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_nlink);
}

static PyObject *
Stat_st_uid(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_uid);
}

static PyObject *
Stat_st_gid(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_gid);
}

static PyObject *
Stat_st_rdev(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_rdev);
}

static PyObject *
Stat_st_ino(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_ino);
}

static PyObject *
Stat_st_size(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_size);
}

static PyObject *
Stat_st_blksize(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_blksize);
}

static PyObject *
Stat_st_blocks(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_blocks);
}

static PyObject *
Stat_st_flags(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_flags);
}

static PyObject *
Stat_st_gen(Stat *self, __attribute__((unused)) void *closure)
{
    return PyLong_FromUint64_t(self->st.st_gen);
}

static PyObject *
Stat_st_atim(Stat *self, __attribute__((unused)) void *closure)
{
    return PyFloat_FromDouble((double) self->st.st_atim.tv_sec + 1e-9 * (double) self->st.st_atim.tv_nsec);
}

static PyObject *
Stat_st_mtim(Stat *self, __attribute__((unused)) void *closure)
{
    return PyFloat_FromDouble((double) self->st.st_mtim.tv_sec + 1e-9 * (double) self->st.st_mtim.tv_nsec);
}

static PyObject *
Stat_st_ctim(Stat *self, __attribute__((unused)) void *closure)
{
    return PyFloat_FromDouble((double) self->st.st_ctim.tv_sec + 1e-9 * (double) self->st.st_ctim.tv_nsec);
}

static PyObject *
Stat_st_birthtim(Stat *self, __attribute__((unused)) void *closure)
{
    return PyFloat_FromDouble((double) self->st.st_birthtim.tv_sec + 1e-9 * (double) self->st.st_birthtim.tv_nsec);
}

static PyGetSetDef stat_getset[] = {
    {"st_dev", (getter) Stat_st_dev, NULL, NULL, NULL},
    {"st_mode", (getter) Stat_st_mode, NULL, NULL, NULL},
    {"st_nlink", (getter) Stat_st_nlink, NULL, NULL, NULL},
    {"st_uid", (getter) Stat_st_uid, NULL, NULL, NULL},
    {"st_gid", (getter) Stat_st_gid, NULL, NULL, NULL},
    {"st_rdev", (getter) Stat_st_rdev, NULL, NULL, NULL},
    {"st_ino", (getter) Stat_st_ino, NULL, NULL, NULL},
    {"st_size", (getter) Stat_st_size, NULL, NULL, NULL},
    {"st_blksize", (getter) Stat_st_blksize, NULL, NULL, NULL},
    {"st_blocks", (getter) Stat_st_blocks, NULL, NULL, NULL},
    {"st_flags", (getter) Stat_st_flags, NULL, NULL, NULL},
    {"st_gen", (getter) Stat_st_gen, NULL, NULL, NULL},
    {"st_atim", (getter) Stat_st_atim, NULL, NULL, NULL},
    {"st_mtim", (getter) Stat_st_mtim, NULL, NULL, NULL},
    {"st_ctim", (getter) Stat_st_ctim, NULL, NULL, NULL},
    {"st_birthtim", (getter) Stat_st_birthtim, NULL, NULL, NULL},
    {NULL},
};

static void
stat_dealloc(PyObject *self)
{
    PyTrack_MarkDeleted(self);
    Py_Delete(self);
}

static PyType_Slot stat_slots[] = {
    {Py_tp_getset, stat_getset},
    {Py_tp_dealloc, stat_dealloc},
    {0, 0},
};

static PyType_Spec stat_spec = {
    "promisedio.fs.Stat",
    sizeof(Stat),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    stat_slots,
};

/*[clinic input]
class fs.FileIO "FileIO *" "S(FileIOType)"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=adc5c20470ff4ace]*/

/*[clinic input]
fs.FileIO.close

Close this handle.

This method has no effect if the file is already closed. Once the file is closed,
any operation on the file (e.g. reading or writing) will raise a ValueError.
As a convenience, it is allowed to call this method more than once; only the first
call, however, will have an effect.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_close_impl(FileIO *self)
/*[clinic end generated code: output=b65a5e38c4695b3a input=f92aa7dd8d5bd472]*/
{
    _CTX_set(self);
    if (self->fd < 0) {
        return (PyObject *) Promise_NewResolved(Py_None, NULL);
    } else if (!self->closefd) {
        self->fd = -1;
        return (PyObject *) Promise_NewResolved(Py_None, NULL);
    } else {
        int fd = self->fd;
        self->fd = -1;
        return (PyObject *) Fs_Close(_ctx, fd);
    }
}

Py_LOCAL_INLINE(PyObject *)
fileio_err_closed(void)
{
    PyErr_SetString(PyExc_ValueError, "I/O operation on closed file");
    return NULL;
}

/*[clinic input]
fs.FileIO.fileno

Return the underlying file descriptor (an integer).
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_fileno_impl(FileIO *self)
/*[clinic end generated code: output=32fe8f526c33df6f input=d2d29c72594b6486]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    return PyLong_FromLong((long) self->fd);
}

/*[clinic input]
fs.FileIO.read
    size: ssize_t = -1
    offset: off_t = -1

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_off_t offset)
/*[clinic end generated code: output=f5520bf613a7bcd6 input=1e3d51dfb810aac3]*/
{
    _CTX_set(self);
    if (self->fd < 0)
        return fileio_err_closed();
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be used with the size argument");
            return NULL;
        }
        return (PyObject *) Fs_Readall(_ctx, self->fd);
    } else {
        return (PyObject *) Fs_Read(_ctx, self->fd, size, offset);
    }
}

/*[clinic input]
fs.FileIO.write
    data: object
    offset: off_t = -1

Write the given bytes to the IO handle.

Returns the number of bytes written, which is always the length of data in bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_write_impl(FileIO *self, PyObject *data, Py_off_t offset)
/*[clinic end generated code: output=5101a0529fb1e4b7 input=af78dece81ce7e29]*/
{
    _CTX_set(self);
    if (self->fd < 0)
        return fileio_err_closed();
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_Write(_ctx, self->fd, data, offset);
}

/*[clinic input]
fs.FileIO.seek
    offset: off_t
    whence: int = 0

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_seek_impl(FileIO *self, Py_off_t offset, int whence)
/*[clinic end generated code: output=d4fdae935ebc13c7 input=34a065c6cb86e967]*/
{
    _CTX_set(self);
    if (self->fd < 0)
        return fileio_err_closed();
    return (PyObject *) Fs_Seek(_ctx, self->fd, offset, whence);
}

static void
fileio_dealloc(FileIO *self)
{
    _CTX_set(self);
    int fd = self->fd;
    PyTrack_MarkDeleted(self);
    Py_Delete(self);
    if (fd >= 0) {
        // close silently, ignore errors, sync
        uv_fs_t req;
        uv_loop_t *loop = Loop_Get();
        Py_BEGIN_ALLOW_THREADS
        uv_fs_close(loop, &req, fd, NULL);
        Py_END_ALLOW_THREADS
    }
}

static PyObject *
get_closed(FileIO *self, void *closure)
{
    return PyBool_FromLong((long) (self->fd < 0));
}

static PyObject *
get_closefd(FileIO *self, void *closure)
{
    return PyBool_FromLong((long) (self->closefd));
}

static PyGetSetDef fileio_getsetlist[] = {
    {"closed", (getter) get_closed, NULL, "True if the file is closed"},
    {"closefd", (getter) get_closefd, NULL, "True if the file descriptor will be closed by close()."},
    {NULL},
};

static PyMethodDef fileio_methods[] = {
    FS_FILEIO_CLOSE_METHODDEF
    FS_FILEIO_FILENO_METHODDEF
    FS_FILEIO_READ_METHODDEF
    FS_FILEIO_SEEK_METHODDEF
    FS_FILEIO_WRITE_METHODDEF
    {NULL}, /* Sentinel */
};

static PyType_Slot fileio_slots[] = {
    {Py_tp_getset, fileio_getsetlist},
    {Py_tp_methods, fileio_methods},
    {Py_tp_dealloc, fileio_dealloc},
    {0, 0},
};

static PyType_Spec fileio_spec = {
    "promisedio.fs.FileIO",
    sizeof(FileIO),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    fileio_slots
};

static int
fsmodule_init(PyObject *module)
{
    _CTX_set_module(module);
    LOG("(%p)", module);
    Capsule_LOAD("promisedio.promise", PROMISE_API);
    Capsule_LOAD("promisedio.loop", LOOP_API);
    return 0;
}

static int
fsmodule_init_types_and_constants(PyObject *module)
{
    _CTX_set_module(module);
#define ADDINT(n, v)                           \
    if (PyModule_AddIntConstant(module, n, v)) \
    return -1
    ADDINT("COPYFILE_EXCL", UV_FS_COPYFILE_EXCL);
    ADDINT("COPYFILE_FICLONE", UV_FS_COPYFILE_FICLONE);
    ADDINT("COPYFILE_FICLONE_FORCE", UV_FS_COPYFILE_FICLONE_FORCE);
    ADDINT("DIRENT_UNKNOWN", UV_DIRENT_UNKNOWN);
    ADDINT("DIRENT_FILE", UV_DIRENT_FILE);
    ADDINT("DIRENT_DIR", UV_DIRENT_DIR);
    ADDINT("DIRENT_LINK", UV_DIRENT_LINK);
    ADDINT("DIRENT_FIFO", UV_DIRENT_FIFO);
    ADDINT("DIRENT_SOCKET", UV_DIRENT_SOCKET);
    ADDINT("DIRENT_CHAR", UV_DIRENT_CHAR);
    ADDINT("DIRENT_BLOCK", UV_DIRENT_BLOCK);
    ADDINT("F_OK", F_OK);
    ADDINT("R_OK", R_OK);
    ADDINT("W_OK", W_OK);
    ADDINT("X_OK", X_OK);
    ADDINT("O_APPEND", UV_FS_O_APPEND);
    ADDINT("O_CREAT", UV_FS_O_CREAT);
    ADDINT("O_EXCL", UV_FS_O_EXCL);
    ADDINT("O_FILEMAP", UV_FS_O_FILEMAP);
    ADDINT("O_RANDOM", UV_FS_O_RANDOM);
    ADDINT("O_RDONLY", UV_FS_O_RDONLY);
    ADDINT("O_RDWR", UV_FS_O_RDWR);
    ADDINT("O_SEQUENTIAL", UV_FS_O_SEQUENTIAL);
    ADDINT("O_SHORT_LIVED", UV_FS_O_SHORT_LIVED);
    ADDINT("O_TEMPORARY", UV_FS_O_TEMPORARY);
    ADDINT("O_TRUNC", UV_FS_O_TRUNC);
    ADDINT("O_WRONLY", UV_FS_O_WRONLY);
    ADDINT("O_DIRECT", UV_FS_O_DIRECT);
    ADDINT("O_DIRECTORY", UV_FS_O_DIRECTORY);
    ADDINT("O_DSYNC", UV_FS_O_DSYNC);
    ADDINT("O_EXLOCK", UV_FS_O_EXLOCK);
    ADDINT("O_NOATIME", UV_FS_O_NOATIME);
    ADDINT("O_NOCTTY", UV_FS_O_NOCTTY);
    ADDINT("O_NOFOLLOW", UV_FS_O_NOFOLLOW);
    ADDINT("O_NONBLOCK", UV_FS_O_NONBLOCK);
    ADDINT("O_SYMLINK", UV_FS_O_SYMLINK);
    ADDINT("O_SYNC", UV_FS_O_SYNC);
    ADDINT("SYMLINK_DIR", UV_FS_SYMLINK_DIR);
    ADDINT("SYMLINK_JUNCTION", UV_FS_SYMLINK_JUNCTION);

    S(FSError) = PyErr_NewException("promisedio.fs.FSError", PyExc_OSError, NULL);
    if (!S(FSError))
        return -1;
    S(StatType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &stat_spec, NULL);
    if (S(StatType) == NULL)
        return -1;
    S(FileIOType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &fileio_spec, NULL);
    if (S(FileIOType) == NULL)
        return -1;
    PyObject *d = PyModule_GetDict(module);
    if (PyDict_SetItemString(d, "FSError", S(FSError)) < 0) {
        return -1;
    }
    return 0;
}

#include "capsule.h"

static int
fsmodule_create_api(PyObject *module)
{
    LOG("(%p)", module);
    Capsule_CREATE(module, FS_API);
    return 0;
}

static int
fsmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _CTX_set_module(module);
    Capsule_VISIT(LOOP_API);
    Capsule_VISIT(PROMISE_API);
    Py_VISIT(S(FSError));
    Py_VISIT(S(StatType));
    Py_VISIT(S(FileIOType));
    return 0;
}

static int
fsmodule_clear(PyObject *module)
{
    _CTX_set_module(module);
    Py_CLEAR(S(FSError));
    Py_CLEAR(S(StatType));
    Py_CLEAR(S(FileIOType));
    return 0;
}

static void
fsmodule_free(void *module)
{
    _CTX_set_module(module);
    LOG("(%p)", module);
    Capsule_CLEAR(LOOP_API);
    Capsule_CLEAR(PROMISE_API);
    fsmodule_clear(module);
}

static PyMethodDef fsmodule_methods[] = {
    FS_STAT_METHODDEF
    FS_FSTAT_METHODDEF
    FS_SEEK_METHODDEF
    FS_CLOSE_METHODDEF
    FS_OPENFD_METHODDEF
    FS_OPEN_METHODDEF
    FS_READ_METHODDEF
    FS_WRITE_METHODDEF
    FS_UNLINK_METHODDEF
    FS_MKDIR_METHODDEF
    FS_RMDIR_METHODDEF
    FS_MKDTEMP_METHODDEF
    FS_MKSTEMP_METHODDEF
    FS_SCANDIR_METHODDEF
    FS_RENAME_METHODDEF
    FS_FSYNC_METHODDEF
    FS_FTRUNCATE_METHODDEF
    FS_FDATASYNC_METHODDEF
    FS_COPYFILE_METHODDEF
    FS_SENDFILE_METHODDEF
    FS_ACCESS_METHODDEF
    FS_CHMOD_METHODDEF
    FS_FCHMOD_METHODDEF
    FS_UTIME_METHODDEF
    FS_FUTIME_METHODDEF
    FS_LINK_METHODDEF
    FS_SYMLINK_METHODDEF
    FS_READLINK_METHODDEF
    FS_CHOWN_METHODDEF
    FS_FCHOWN_METHODDEF
    {NULL},
};

static PyModuleDef_Slot fsmodule_slots[] = {
    {Py_mod_exec, fsmodule_init},
    {Py_mod_exec, fsmodule_init_types_and_constants},
    {Py_mod_exec, fsmodule_create_api},
    {0, NULL},
};

static struct PyModuleDef fsmodule_def = {
    PyModuleDef_HEAD_INIT,
    "promisedio.fs",
    NULL,
    sizeof(_modulestate),
    fsmodule_methods,
    fsmodule_slots,
    fsmodule_traverse,
    fsmodule_clear,
    fsmodule_free,
};

PyMODINIT_FUNC
PyInit_fs(void)
{
    return PyModuleDef_Init(&fsmodule_def);
}