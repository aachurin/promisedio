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
#include "loop/loop_api.h"
#include "promise/promise_api.h"
#include "fs.h"

/*[clinic input]
module fsmodule
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=e64efc353b3914bd]*/

typedef struct {
    uv_loop_t *loop;
    PyObject *FSError;
    PyTypeObject *StatType;
    PyTypeObject *FileIOType;
    Capsule_MOUNT_POINT(PROMISE_API_ID)
    Capsule_MOUNT_POINT(LOOP_API_ID)
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

#define FS_CALL(promise, func, ...)                     \
    Request_SETUP(uv_fs_t, _req, promise);              \
    if (_req) {                                         \
        UV_CALL(func, S(loop), _req, __VA_ARGS__)       \
        {                                               \
            Request_CloseFs(_req);                      \
            PyErr_SetUvErr(S(FSError), error);          \
            Py_DECREF(promise);                         \
            promise = NULL;                             \
        }                                               \
    }

typedef struct {
    PyObject_HEAD
    uv_file fd;
    int closefd;
    _STATE_var;
} FileIO;

typedef struct {
    PyObject_HEAD
    uv_stat_t st;
} Stat;

#include "clinic/fs.c.h"

#define Request_RejectFsExc(req, uverr) Request_RejectUVError(req, S(FSError), uverr)

static void
int_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            PyObject *value = PyLong_FromSsize_t(req->result);
            if (!value) {
                Request_RejectPyExc(req);
            } else {
                Request_Resolve(req, value);
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
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            Request_Resolve(req, Py_None);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
bool_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_Resolve(req, Py_False);
        } else {
            Request_Resolve(req, Py_True);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static void
path_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            PyObject *result = PyUnicode_DecodeFSDefault(req->path);
            if (!result) {
                Request_RejectPyExc(req);
            } else {
                Request_Resolve(req, result);
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
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            PyObject *result = PyUnicode_DecodeFSDefault(req->ptr);
            if (!result) {
                Request_RejectPyExc(req);
            } else {
                Request_Resolve(req, result);
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
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            Stat *obj = (Stat *) Object_New(S(StatType));
            if (!obj) {
                Request_RejectPyExc(req);
            } else {
                obj->st = req->statbuf;
                Request_Resolve(req, (PyObject *) obj);
                Py_DECREF(obj);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
Fs_stat(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_stat, path, stat_callback);
    return promise;
}

static PyObject *
Fs_lstat(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_lstat, path, stat_callback);
    return promise;
}

/*[clinic input]
fsmodule.stat
    path: Path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_stat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=34c2110f495eb75b input=8c9a45ec0514120d]*/
{
    _STATE_setmodule(module)

    if (follow_symlinks) {
        return Fs_stat(_state, PyBytes_AS_STRING(path));
    } else {
        return Fs_lstat(_state, PyBytes_AS_STRING(path));
    }
}

static PyObject *
Fs_fstat(_STATE_var, uv_file fd)
{
    FS_CALL(promise, uv_fs_fstat, fd, stat_callback);
    return promise;
}

/*[clinic input]
fsmodule.fstat
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_fstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=c3640f76f04482ed input=3b1318ca74906749]*/
{
    _STATE_setmodule(module)

    return Fs_fstat(_state, fd);
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
    case 0:how = SEEK_SET;
        break;
    case 1:how = SEEK_CUR;
        break;
    case 2:how = SEEK_END;
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
        _STATE_setreq(req)

        if (status == UV_ECANCELED) {
            Request_RejectFsExc((uv_req_t *) req, status);
        } else if (req->result < 0) {
            Request_RejectFsExc((uv_req_t *) req, (int) req->result);
        } else {
            PyObject *value = PyLong_FromOff_t(req->result);
            if (!value) {
                Request_RejectPyExc((uv_req_t *) req);
            } else {
                Request_Resolve((uv_req_t *) req, value);
                Py_DECREF(value);
            }
        }
        Request_Close(req);
    RELEASE_GIL
}

static PyObject *
Fs_seek(_STATE_var, uv_file fd, Py_off_t pos, int how)
{
    Request_SETUP(SeekReq, req, promise)
    if (req) {
        UV_CALL(fs__seek, S(loop), req, fd, pos, how, (uv_after_work_cb) seek_callback) {
        };
    }
    return promise;
}

/*[clinic input]
fsmodule.seek
    fd: fd
    pos: off_t
    how: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_seek_impl(PyObject *module, int fd, Py_off_t pos, int how)
/*[clinic end generated code: output=906a3566cff61a30 input=47bc1ab2bd2e1be8]*/
{
    _STATE_setmodule(module)

    return Fs_seek(_state, fd, pos, how);
}

static PyObject *
Fs_open(_STATE_var, const char *path, int flags, int mode)
{
    FS_CALL(promise, uv_fs_open, path, flags, mode, int_callback);
    return promise;
}

/*[clinic input]
fsmodule.openfd
    path: Path
    flags: int
    mode: int = 0o666
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_openfd_impl(PyObject *module, PyObject *path, int flags, int mode)
/*[clinic end generated code: output=869ea355647b4244 input=b9e82dbbfbed4151]*/
{
    _STATE_setmodule(module)

    return Fs_open(_state, PyBytes_AS_STRING(path), flags, mode);
}

static PyObject *
fileio_new(_STATE_var, int fd, int closefd)
{
    FileIO *self = (FileIO *) Object_New(S(FileIOType));
    if (!self)
        return NULL;
    self->fd = fd;
    self->closefd = closefd;
    _STATE_save(self);
    return (PyObject *) self;
}

static void
open_stat_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
            goto finally;
        }
#if defined(S_ISDIR)
        if (S_ISDIR(req->statbuf.st_mode)) {
            Request_RejectFsExc(req, UV_EISDIR);
            goto finally;
        }
#endif
        PyObject *fileobj = fileio_new(_state, *Request_DATA(req, uv_file), 1);
        if (!fileobj) {
            Request_RejectPyExc(req);
            goto finally;
        }
        Request_Resolve(req, fileobj);
        Py_DECREF(fileobj);

    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static void
open_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
            goto finally;
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, Request_PROMISE(req));
        if (!nreq) {
            Request_RejectPyExc(req);
            goto finally;
        }
        uv_file fd = (uv_file) req->result;
        *Request_DATA(req, uv_file) = fd;
        UV_CALL(uv_fs_fstat, req->loop, nreq, fd, open_stat_callback) {
            Request_CloseFs(nreq);
            Request_RejectFsExc(nreq, error);
        }
    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
fs_fileio_open(_STATE_var, const char *path, int flags)
{
    FS_CALL(promise, uv_fs_open, path, flags, 0666, open_callback);
    return promise;
}

/*[clinic input]
fsmodule.open
    path: object
    mode: cstring = "r"
    closefd: bool(accept={int}) = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_open_impl(PyObject *module, PyObject *path, const char *mode,
                   int closefd)
/*[clinic end generated code: output=467a96136cf13efe input=8a45bf55955e17a5]*/
{
    _STATE_setmodule(module)

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
        case '+':writable = 1;
            break;
        case 'b': break;
        default: goto invalid_mode;
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
        PyObject *fileobj = fileio_new(_state, fd, closefd);
        if (!fileobj)
            return NULL;
        return Promise_NewResolved(fileobj);
    }

    if (!closefd) {
        PyErr_SetString(PyExc_ValueError, "Cannot use closefd=False with file name");
        return NULL;
    }

    PyObject *fspath;
    if (!PyUnicode_FSConverter(path, &fspath)) {
        return NULL;
    }

    PyObject *result = fs_fileio_open(_state, PyBytes_AS_STRING(fspath), flags);
    Py_DECREF(fspath);

    return result;
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
        _STATE_setreq(req)
        PyObject *buffer = Request_SetCtx(req, NULL);
        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
            goto finally;
        }
        Py_ssize_t size = req->result;
        if (size < PyBytes_GET_SIZE(buffer)) {
            if (_PyBytes_Resize(&buffer, size) != 0) {
                Request_RejectPyExc(req);
                goto finally;
            }
        }
        Request_Resolve(req, buffer);

    finally:
        Py_XDECREF(buffer);
        Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
Fs_read(_STATE_var, uv_file fd, Py_ssize_t size, Py_off_t offset)
{
    if (size > _PY_READ_MAX) {
        size = _PY_READ_MAX;
    }
    PyObject *buffer = PyBytes_FromStringAndSize(NULL, size);
    if (!buffer)
        return NULL;
    FS_CALL(promise, fs__read, fd, PyBytes_AS_STRING(buffer), size, offset, read_callback);
    if (!promise) {
        Py_DECREF(buffer);
        return NULL;
    }
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
        _STATE_setreq(req)

        readallstate *readstate = Request_DATA(req, readallstate);
        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        cleanup:
            Py_DECREF(readstate->buffer);
            goto finally;
        }
        if (req->result == 0) {
            if (PyBytes_GET_SIZE(readstate->buffer) > readstate->bytes_read) {
                if (_PyBytes_Resize(&readstate->buffer, readstate->bytes_read) < 0) {
                    Request_RejectPyExc(req);
                    goto finally;
                }
            }
            Request_Resolve(req, readstate->buffer);
            goto cleanup;
        }
        readstate->bytes_read += req->result;
        if (readstate->bytes_read >= PyBytes_GET_SIZE(readstate->buffer)) {
            size_t new_size = new_buffersize(readstate->bytes_read);
            if (new_size > PY_SSIZE_T_MAX) {
                Request_RejectString(req, PyExc_OverflowError,
                                     "unbounded read returned more bytes "
                                     "than a Python bytes object can hold");
                goto cleanup;
            }
            if (_PyBytes_Resize(&readstate->buffer, (Py_ssize_t) new_size) < 0) {
                Request_RejectPyExc(req);
                goto finally;
            }
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, Request_PROMISE(req));
        if (!nreq) {
            Request_RejectPyExc(req);
            goto cleanup;
        }
        char *buf = PyBytes_AS_STRING(readstate->buffer) + readstate->bytes_read;
        Py_ssize_t size = PyBytes_GET_SIZE(readstate->buffer) - readstate->bytes_read;
        UV_CALL(fs__read, req->loop, nreq, readstate->fd, buf, size, -1, readall_read_callback) {
            Request_CloseFs(nreq);
            Request_RejectFsExc(nreq, error);
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
        _STATE_setreq(req)

        readallstate *readstate = Request_DATA(req, readallstate);
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
            Request_RejectPyExc(req);
            goto finally;
        }
        uv_fs_t *nreq = Request_New(uv_fs_t, Request_PROMISE(req));
        if (!nreq) {
            Request_RejectPyExc(req);
        cleanup:
            Py_DECREF(buffer);
            goto finally;
        }
        readstate->buffer = buffer;
        UV_CALL(fs__read, req->loop, nreq, readstate->fd, PyBytes_AS_STRING(buffer), bufsize, -1, readall_read_callback) {
            Request_CloseFs(nreq);
            Request_RejectFsExc(nreq, error);
            goto cleanup;
        }

    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static void
readall_seek_callback(SeekReq *req, int status)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (status == UV_ECANCELED) {
            Request_RejectFsExc((uv_req_t *) req, status);
            goto finally;
        }
        if (req->result < 0) {
            Request_RejectFsExc((uv_req_t *) req, (int) req->result);
            goto finally;
        }
        readallstate *readstate = Request_DATA((uv_req_t *) req, readallstate);
        readstate->pos = req->result;
        uv_fs_t *nreq = Request_New(uv_fs_t, Request_PROMISE((uv_req_t *) req));
        if (!nreq) {
            Request_RejectPyExc((uv_req_t *) req);
            goto finally;
        }
        UV_CALL(uv_fs_fstat, req->base.loop, nreq, readstate->fd, readall_stat_callback) {
            Request_CloseFs(nreq);
            Request_RejectFsExc((uv_req_t *) req, error);
        }

    finally:
        Request_Close(req);
    RELEASE_GIL
}

static PyObject *
Fs_readall(_STATE_var, int fd)
{
    Request_SETUP(SeekReq, req, promise)
    if (req) {
        *Promise_DATA(promise, readallstate) = (readallstate) {.fd = fd, .pos = 0, .bytes_read = 0};
        UV_CALL(fs__seek, S(loop), req, fd, 0, 1, (uv_after_work_cb) readall_seek_callback) {
        }
    }
    return promise;
}

/*[clinic input]
fsmodule.read
    fd: fd
    size: ssize_t = -1
    offset: off_t = -1
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_read_impl(PyObject *module, int fd, Py_ssize_t size,
                   Py_off_t offset)
/*[clinic end generated code: output=396bd62d96251937 input=05d94314bbcc3693]*/
{
    _STATE_setmodule(module)

    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be specified in conjunction with the size parameter");
            return NULL;
        }
        return Fs_readall(_state, fd);
    } else {
        return Fs_read(_state, fd, size, offset);
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
        _STATE_setreq(req)
        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            PyObject *size = PyLong_FromSsize_t(req->result);
            Request_Resolve(req, size);
            Py_DECREF(size);
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
Fs_write(_STATE_var, int fd, PyObject *data, Py_off_t offset)
{
    FS_CALL(promise, fs__write, fd, PyBytes_AS_STRING(data), PyBytes_GET_SIZE(data), offset, write_callback);
    if (promise) {
        Promise_SetCtx(promise, Py_NewRef(data));
    }
    return promise;
}

/*[clinic input]
fsmodule.write
    fd: fd
    data: object
    offset: off_t = -1
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_write_impl(PyObject *module, int fd, PyObject *data,
                    Py_off_t offset)
/*[clinic end generated code: output=808eccfd68701fd6 input=8ca508c266830447]*/
{
    _STATE_setmodule(module)

    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return Fs_write(_state, fd, data, offset);
}

static PyObject *
Fs_close(_STATE_var, uv_file fd)
{
    FS_CALL(promise, uv_fs_close, fd, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.close
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_close_impl(PyObject *module, int fd)
/*[clinic end generated code: output=d04156d19c252b76 input=817c95eb436fb04e]*/
{
    _STATE_setmodule(module)

    return Fs_close(_state, fd);
}

/*[clinic input]
fsmodule.closefd
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_closefd_impl(PyObject *module, int fd)
/*[clinic end generated code: output=eaef24198e88257b input=989b9539ed00e505]*/
{
    _STATE_setmodule(module)

    return Fs_close(_state, fd);
}

static PyObject *
Fs_unlink(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_unlink, path, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.unlink
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_unlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=2f123e2d9bf6524b input=87d37afcb53600eb]*/
{
    _STATE_setmodule(module)

    return Fs_unlink(_state, PyBytes_AS_STRING(path));
}

static PyObject *
Fs_mkdir(_STATE_var, const char *path, int mode)
{
    FS_CALL(promise, uv_fs_mkdir, path, mode, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.mkdir
    path: Path
    mode: int = 0o777
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=028a207967a67392 input=e2450206189560e5]*/
{
    _STATE_setmodule(module)

    return Fs_mkdir(_state, PyBytes_AS_STRING(path), mode);
}

static PyObject *
Fs_rmdir(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_rmdir, path, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.rmdir
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_rmdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=7c5cfd7e64059a73 input=7bc63aaf140d3f0a]*/
{
    _STATE_setmodule(module)

    return Fs_rmdir(_state, PyBytes_AS_STRING(path));
}

static PyObject *
Fs_mkdtemp(_STATE_var, const char *tpl)
{
    FS_CALL(promise, uv_fs_mkdtemp, tpl, path_callback);
    return promise;
}

/*[clinic input]
fsmodule.mkdtemp
    tpl: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=3b968f1bdab58f84 input=878aac88aaa2f23b]*/
{
    _STATE_setmodule(module)

    return Fs_mkdtemp(_state, PyBytes_AS_STRING(tpl));
}

static void
mkstemp_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
        } else {
            PyObject *result = Py_BuildValue("ns", req->result, req->path);
            if (result) {
                Request_Resolve(req, result);
                Py_DECREF(result);
            } else {
                Request_RejectPyExc(req);
            }
        }
        Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
Fs_mkstemp(_STATE_var, const char *tpl)
{
    FS_CALL(promise, uv_fs_mkstemp, tpl, mkstemp_callback);
    return promise;
}

/*[clinic input]
fsmodule.mkstemp
    tpl: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=571496324bf87333 input=955ad7257ba28006]*/
{
    _STATE_setmodule(module)

    return Fs_mkstemp(_state, PyBytes_AS_STRING(tpl));
}

static void
scandir_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (req->result < 0) {
            Request_RejectFsExc(req, (int) req->result);
            goto finally;
        }
        Py_ssize_t n = req->result;
        PyObject *result = PyTuple_New(n);
        if (!result) {
        error:
            Request_RejectPyExc(req);
            goto finally;
        }
        if (n) {
            uv_dirent_t dent;
            Py_ssize_t index = 0;
            while (UV_EOF != uv_fs_scandir_next(req, &dent)) {
                int type = 0;
                switch (dent.type) {
                case UV_DIRENT_UNKNOWN:type = 0;
                    break;
                case UV_DIRENT_FILE:type = 1;
                    break;
                case UV_DIRENT_DIR:type = 2;
                    break;
                case UV_DIRENT_LINK:type = 3;
                    break;
                case UV_DIRENT_FIFO:type = 4;
                    break;
                case UV_DIRENT_SOCKET:type = 5;
                    break;
                case UV_DIRENT_CHAR:type = 6;
                    break;
                case UV_DIRENT_BLOCK:type = 7;
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
        Request_Resolve(req, result);
        Py_DECREF(result);

    finally:
    Request_CloseFs(req);
    RELEASE_GIL
}

static PyObject *
Fs_scandir(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_scandir, path, 0, scandir_callback);
    return promise;
}

/*[clinic input]
fsmodule.scandir
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_scandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=6aa607df28540a89 input=b3d65f1989abc9fa]*/
{
    _STATE_setmodule(module)

    return Fs_scandir(_state, PyBytes_AS_STRING(path));
}

static PyObject *
Fs_rename(_STATE_var, const char *path, const char *new_path)
{
    FS_CALL(promise, uv_fs_rename, path, new_path, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.rename
    path: Path
    new_path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_rename_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=391eeecc807f0cc3 input=09a60a0c73997327]*/
{
    _STATE_setmodule(module)

    return Fs_rename(_state, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

static PyObject *
Fs_fsync(_STATE_var, int fd)
{
    FS_CALL(promise, uv_fs_fsync, fd, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.fsync
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_fsync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=abf535072a5dfb6c input=ef7886c50c403733]*/
{
    _STATE_setmodule(module)

    return Fs_fsync(_state, fd);
}

static PyObject *
Fs_ftruncate(_STATE_var, int fd, Py_ssize_t length)
{
    FS_CALL(promise, uv_fs_ftruncate, fd, length, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.ftruncate
    fd: fd
    length: ssize_t
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length)
/*[clinic end generated code: output=2779ae1035eee114 input=fcbe7801b02a45f6]*/
{
    _STATE_setmodule(module)

    return Fs_ftruncate(_state, fd, length);
}

static PyObject *
Fs_fdatasync(_STATE_var, int fd)
{
    FS_CALL(promise, uv_fs_fdatasync, fd, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.fdatasync
    fd: fd
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_fdatasync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=fe92b7b3ee14f5ce input=5bd719df409262a3]*/
{
    _STATE_setmodule(module)

    return Fs_fdatasync(_state, fd);
}

static PyObject *
Fs_copyfile(_STATE_var, const char *path, const char *new_path, int flags)
{
    FS_CALL(promise, uv_fs_copyfile, path, new_path, flags, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.copyfile
    path: Path
    new_path: Path
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_copyfile_impl(PyObject *module, PyObject *path, PyObject *new_path,
                       int flags)
/*[clinic end generated code: output=d0435f44f336e0b9 input=c0171ddd034ccb93]*/
{
    _STATE_setmodule(module)

    return Fs_copyfile(_state, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

static PyObject *
Fs_sendfile(_STATE_var, int out_fd, int in_fd, Py_off_t in_offset, size_t length)
{
    FS_CALL(promise, uv_fs_sendfile, out_fd, in_fd, in_offset, length, int_callback);
    return promise;
}

/*[clinic input]
fsmodule.sendfile
    out_fd: fd
    in_fd: fd
    offset: off_t
    count: ssize_t
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_sendfile_impl(PyObject *module, int out_fd, int in_fd,
                       Py_off_t offset, Py_ssize_t count)
/*[clinic end generated code: output=71b10da8acbadb33 input=ee4228431ddfd9b0]*/
{
    _STATE_setmodule(module)

    return Fs_sendfile(_state, out_fd, in_fd, offset, count);
}

static PyObject *
Fs_access(_STATE_var, const char *path, int mode)
{
    FS_CALL(promise, uv_fs_access, path, mode, bool_callback);
    return promise;
}

/*[clinic input]
fsmodule.access
    path: Path
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_access_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=5b4e8d8a3c3470b1 input=ca88f0e7fcf5ddf3]*/
{
    _STATE_setmodule(module)

    return Fs_access(_state, PyBytes_AS_STRING(path), mode);
}

static PyObject *
Fs_chmod(_STATE_var, const char *path, int mode)
{
    FS_CALL(promise, uv_fs_chmod, path, mode, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.chmod
    path: Path
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_chmod_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=65e1250ff82ac9f7 input=3436136e26a28a6d]*/
{
    _STATE_setmodule(module)

    return Fs_chmod(_state, PyBytes_AS_STRING(path), mode);
}

static PyObject *
Fs_fchmod(_STATE_var, int fd, int mode)
{
    FS_CALL(promise, uv_fs_fchmod, fd, mode, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.fchmod
    fd: fd
    mode: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_fchmod_impl(PyObject *module, int fd, int mode)
/*[clinic end generated code: output=cd21ad0d91f5a668 input=e5abb45d1df62cca]*/
{
    _STATE_setmodule(module)

    return Fs_fchmod(_state, fd, mode);
}

static PyObject *
Fs_utime(_STATE_var, const char *path, double atime, double mtime)
{
    FS_CALL(promise, uv_fs_utime, path, atime, mtime, none_callback);
    return promise;
}

static PyObject *
Fs_lutime(_STATE_var, const char *path, double atime, double mtime)
{
    FS_CALL(promise, uv_fs_lutime, path, atime, mtime, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.utime
    path: Path
    atime: double
    mtime: double,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_utime_impl(PyObject *module, PyObject *path, double atime,
                    double mtime, int follow_symlinks)
/*[clinic end generated code: output=103b602ebfe3ede0 input=a05a6a9ecee86b68]*/
{
    _STATE_setmodule(module)

    if (follow_symlinks) {
        return Fs_utime(_state, PyBytes_AS_STRING(path), atime, mtime);
    } else {
        return Fs_lutime(_state, PyBytes_AS_STRING(path), atime, mtime);
    }
}

static PyObject *
Fs_futime(_STATE_var, int fd, double atime, double mtime)
{
    FS_CALL(promise, uv_fs_futime, fd, atime, mtime, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.futime
    fd: fd
    atime: double
    mtime: double
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_futime_impl(PyObject *module, int fd, double atime, double mtime)
/*[clinic end generated code: output=ef0613cb96db5273 input=4aa66db7cf7f59a0]*/
{
    _STATE_setmodule(module)

    return Fs_futime(_state, fd, atime, mtime);
}

static PyObject *
Fs_link(_STATE_var, const char *path, const char *new_path)
{
    FS_CALL(promise, uv_fs_link, path, new_path, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.link
    path: Path
    new_path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_link_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=aa5d2a01a189c562 input=febee114325b702d]*/
{
    _STATE_setmodule(module)

    return Fs_link(_state, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

static PyObject *
Fs_symlink(_STATE_var, const char *path, const char *new_path, int flags)
{
    FS_CALL(promise, uv_fs_symlink, path, new_path, flags, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.symlink
    path: Path
    new_path: Path
    *
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                      int flags)
/*[clinic end generated code: output=b212d13ed5bd08cb input=a043b46b3beb5851]*/
{
    _STATE_setmodule(module)

    return Fs_symlink(_state, PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

static PyObject *
Fs_readlink(_STATE_var, const char *path)
{
    FS_CALL(promise, uv_fs_readlink, path, ptr_path_callback);
    return promise;
}

/*[clinic input]
fsmodule.readlink
    path: Path
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_readlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=1cf86d0cb6083957 input=697d7fca6a8c1249]*/
{
    _STATE_setmodule(module)

    return Fs_readlink(_state, PyBytes_AS_STRING(path));
}

#ifndef MS_WINDOWS

static PyObject *
Fs_chown(_STATE_var, const char *path, uv_uid_t uid, uv_gid_t gid)
{
    FS_CALL(promise, uv_fs_chown, path, uid, gid, none_callback);
    return promise;
}

static PyObject *
Fs_lchown(_STATE_var, const char *path, uv_uid_t uid, uv_gid_t gid)
{
    FS_CALL(promise, uv_fs_lchown, path, uid, gid, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.chown
    path: Path
    uid: uid_t,
    gid: gid_t,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_chown_impl(PyObject *module, PyObject *path, uid_t uid, gid_t gid,
                    int follow_symlinks)
/*[clinic end generated code: output=d5fe57e184fbadb8 input=83a6237f74e31297]*/
{
    _STATE_setmodule(module)

    if (follow_symlinks) {
        return Fs_chown(_state, PyBytes_AS_STRING(path), uid, gid);
    } else {
        return Fs_lchown(_state, PyBytes_AS_STRING(path), uid, gid);
    }
}

Py_LOCAL_INLINE(PyObject *)
Fs_fchown(_STATE_var, int fd, uv_uid_t uid, uv_gid_t gid)
{
    FS_CALL(promise, uv_fs_fchown, fd, uid, gid, none_callback);
    return promise;
}

/*[clinic input]
fsmodule.fchown
    fd: fd
    uid: uid_t,
    gid: gid_t,
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_fchown_impl(PyObject *module, int fd, uid_t uid, gid_t gid)
/*[clinic end generated code: output=a3be8be6755c3d2e input=13946a55330ca438]*/
{
    _STATE_setmodule(module)

    return Fs_fchown(_state, fd, uid, gid);
}

#endif

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

static PyType_Slot stat_slots[] = {
    {Py_tp_getset, stat_getset},
    {Py_tp_dealloc, Mem_Free},
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
class fsmodule.FileIO "FileIO *" "&FileIOType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=ecb73b2a06b06615]*/

/*[clinic input]
fsmodule.FileIO.close

Close this handle.

This method has no effect if the file is already closed. Once the file is closed,
any operation on the file (e.g. reading or writing) will raise a ValueError.
As a convenience, it is allowed to call this method more than once; only the first
call, however, will have an effect.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_close_impl(FileIO *self)
/*[clinic end generated code: output=60eb034b0f991c78 input=ebf3fe67bf6932e4]*/
{
    _STATE_set(self)

    if (self->fd < 0) {
        return Promise_NewResolved(Py_None);
    } else if (!self->closefd) {
        self->fd = -1;
        return Promise_NewResolved(Py_None);
    } else {
        int fd = self->fd;
        self->fd = -1;
        return (PyObject *) Fs_close(_state, fd);
    }
}

Py_LOCAL_INLINE(PyObject *)
fileio_err_closed(void)
{
    PyErr_SetString(PyExc_ValueError, "I/O operation on closed file");
    return NULL;
}

/*[clinic input]
fsmodule.FileIO.fileno

Return the underlying file descriptor (an integer).
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_fileno_impl(FileIO *self)
/*[clinic end generated code: output=8ad0732646e05d2c input=3286ff344110240d]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    return PyLong_FromLong((long) self->fd);
}

/*[clinic input]
fsmodule.FileIO.read
    size: ssize_t = -1
    offset: off_t = -1

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_off_t offset)
/*[clinic end generated code: output=a0fc4040888c3005 input=7b2cfb1b2d1627ce]*/
{
    _STATE_set(self)

    if (self->fd < 0)
        return fileio_err_closed();
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be used with the size argument");
            return NULL;
        }
        return (PyObject *) Fs_readall(_state, self->fd);
    } else {
        return (PyObject *) Fs_read(_state, self->fd, size, offset);
    }
}

/*[clinic input]
fsmodule.FileIO.write
    data: object
    offset: off_t = -1

Write the given bytes to the IO handle.

Returns the number of bytes written, which is always the length of data in bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_write_impl(FileIO *self, PyObject *data, Py_off_t offset)
/*[clinic end generated code: output=f10ec0fa444d76b3 input=f2492035a530ab61]*/
{
    _STATE_set(self)

    if (self->fd < 0)
        return fileio_err_closed();
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_write(_state, self->fd, data, offset);
}

/*[clinic input]
fsmodule.FileIO.seek
    offset: off_t
    whence: int = 0

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_seek_impl(FileIO *self, Py_off_t offset, int whence)
/*[clinic end generated code: output=c9ea49e2042d7e88 input=03b7bf4944a2e937]*/
{
    _STATE_set(self)

    if (self->fd < 0)
        return fileio_err_closed();
    return (PyObject *) Fs_seek(_state, self->fd, offset, whence);
}

static void
fileio_dealloc(FileIO *self)
{
    _STATE_set(self)

    if (self->fd >= 0) {
        // close silently, ignore errors, sync
        int fd = self->fd;
        self->fd = -1;
        uv_fs_t req;
        uv_fs_close(S(loop), &req, fd, NULL);
    }
    Object_Del(self);
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
    FSMODULE_FILEIO_CLOSE_METHODDEF
    FSMODULE_FILEIO_FILENO_METHODDEF
    FSMODULE_FILEIO_READ_METHODDEF
    FSMODULE_FILEIO_SEEK_METHODDEF
    FSMODULE_FILEIO_WRITE_METHODDEF
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
    fileio_slots};

static int
fsmodule_init(PyObject *module)
{
    LOG("fsmodule_init(%p)", module);

    _STATE_setmodule(module)

    Capsule_LOAD("promisedio.promise", PROMISE_API_ID);
    Capsule_LOAD("promisedio.loop", LOOP_API_ID);
    S(loop) = Loop_Get();
    return 0;
}

static int
fsmodule_init_types_and_constants(PyObject *module)
{
    _STATE_setmodule(module)

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

static int
fsmodule_create_api(PyObject *module)
{
    LOG("fsmodule_create_api(%p)", module);
    static void *c_api[] = {
        [FS_STAT_API_ID] = Fs_stat,
        [FS_LSTAT_API_ID] = Fs_lstat,
        [FS_FSTAT_API_ID] = Fs_fstat,
        [FS_OPEN_API_ID] = Fs_open,
        [FS_READ_API_ID] = Fs_read,
        [FS_READALL_API_ID] = Fs_readall,
        [FS_WRITE_API_ID] = Fs_write,
        [FS_SEEK_API_ID] = Fs_seek,
        [FS_CLOSE_API_ID] = Fs_close,
        [FS_UNLINK_API_ID] = Fs_unlink,
        [FS_MKDIR_API_ID] = Fs_mkdir,
        [FS_RMDIR_API_ID] = Fs_rmdir,
        [FS_MKDTEMP_API_ID] = Fs_mkdtemp,
        [FS_MKSTEMP_API_ID] = Fs_mkstemp,
        [FS_SCANDIR_API_ID] = Fs_scandir,
        [FS_RENAME_API_ID] = Fs_rename,
        [FS_FTRUNCATE_API_ID] = Fs_ftruncate,
        [FS_FSYNC_API_ID] = Fs_fsync,
        [FS_FDATASYNC_API_ID] = Fs_fdatasync,
        [FS_COPYFILE_API_ID] = Fs_copyfile,
        [FS_SENDFILE_API_ID] = Fs_sendfile,
        [FS_ACCESS_API_ID] = Fs_access,
        [FS_CHMOD_API_ID] = Fs_chmod,
        [FS_FCHMOD_API_ID] = Fs_fchmod,
        [FS_UTIME_API_ID] = Fs_utime,
        [FS_FUTIME_API_ID] = Fs_futime,
        [FS_LUTIME_API_ID] = Fs_lutime,
        [FS_LINK_API_ID] = Fs_link,
        [FS_SYMLINK_API_ID] = Fs_symlink,
        [FS_READLINK_API_ID] = Fs_readlink,
#ifndef MS_WINDOWS
        [FS_CHOWN_API_ID] = Fs_chown,
        [FS_FCHOWN_API_ID] = Fs_fchown,
        [FS_LCHOWN_API_ID] = Fs_lchown,
#endif
    };
    Capsule_CREATE(module, FS_API_ID, c_api);
    return 0;
}

static int
fsmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _STATE_setmodule(module)

    Capsule_TRAVERSE(LOOP_API_ID);
    Capsule_TRAVERSE(PROMISE_API_ID);
    Py_VISIT(S(FSError));
    Py_VISIT(S(StatType));
    Py_VISIT(S(FileIOType));
    return 0;
}

static int
fsmodule_clear(PyObject *module)
{
    _STATE_setmodule(module)

    Capsule_CLEAR(LOOP_API_ID);
    Capsule_CLEAR(PROMISE_API_ID);
    Py_CLEAR(S(FSError));
    Py_CLEAR(S(StatType));
    Py_CLEAR(S(FileIOType));
    return 0;
}

static void
fsmodule_free(void *module)
{
    LOG("fsmodule_free(%p)", module);
    fsmodule_clear(module);
}

static PyMethodDef fsmodule_methods[] = {
    FSMODULE_STAT_METHODDEF
    FSMODULE_FSTAT_METHODDEF
    FSMODULE_SEEK_METHODDEF
    FSMODULE_CLOSE_METHODDEF
    FSMODULE_CLOSEFD_METHODDEF
    FSMODULE_OPENFD_METHODDEF
    FSMODULE_OPEN_METHODDEF
    FSMODULE_READ_METHODDEF
    FSMODULE_WRITE_METHODDEF
    FSMODULE_UNLINK_METHODDEF
    FSMODULE_MKDIR_METHODDEF
    FSMODULE_RMDIR_METHODDEF
    FSMODULE_MKDTEMP_METHODDEF
    FSMODULE_MKSTEMP_METHODDEF
    FSMODULE_SCANDIR_METHODDEF
    FSMODULE_RENAME_METHODDEF
    FSMODULE_FSYNC_METHODDEF
    FSMODULE_FTRUNCATE_METHODDEF
    FSMODULE_FDATASYNC_METHODDEF
    FSMODULE_COPYFILE_METHODDEF
    FSMODULE_SENDFILE_METHODDEF
    FSMODULE_ACCESS_METHODDEF
    FSMODULE_CHMOD_METHODDEF
    FSMODULE_FCHMOD_METHODDEF
    FSMODULE_UTIME_METHODDEF
    FSMODULE_FUTIME_METHODDEF
    FSMODULE_LINK_METHODDEF
    FSMODULE_SYMLINK_METHODDEF
    FSMODULE_READLINK_METHODDEF
    FSMODULE_CHOWN_METHODDEF
    FSMODULE_FCHOWN_METHODDEF
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