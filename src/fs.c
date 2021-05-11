// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "fs.h"
#include "c2py.h"

#if BUFSIZ < (8*1024)
    #define SMALLCHUNK (8*1024)
#elif (BUFSIZ >= (2 << 25))
    #error "unreasonable BUFSIZ > 64 MiB defined"
#else
    #define SMALLCHUNK BUFSIZ
#endif

#define new_file_request(promise) \
    Request_New(promise, uv_fs_t)

#define close_file_request(req) \
    uv_fs_req_cleanup(req);     \
    Request_Close(req)

Generate_PyType_Impl(
    StatObj,
    READONLY(st_dev, st.st_dev, uint64_t),
    READONLY(st_mode, st.st_mode, uint64_t),
    READONLY(st_nlink, st.st_nlink, uint64_t),
    READONLY(st_uid, st.st_uid, uint64_t),
    READONLY(st_gid, st.st_gid, uint64_t),
    READONLY(st_rdev, st.st_rdev, uint64_t),
    READONLY(st_ino, st.st_ino, uint64_t),
    READONLY(st_size, st.st_size, uint64_t),
    READONLY(st_blksize, st.st_blksize, uint64_t),
    READONLY(st_blocks, st.st_blocks, uint64_t),
    READONLY(st_flags, st.st_flags, uint64_t),
    READONLY(st_gen, st.st_gen, uint64_t),
    READONLY(st_atim, st.st_atim, uv_timespec_t),
    READONLY(st_mtim, st.st_mtim, uv_timespec_t),
    READONLY(st_ctim, st.st_ctim, uv_timespec_t),
    READONLY(st_birthtim, st.st_birthtim, uv_timespec_t)
)

#define INIT_REQUEST(req, promise)                     \
    if (((req) = new_file_request(promise)) == NULL) { \
        return -1;                                     \
    }

static void
promise_reject_with_oserror(Promise *promise, ssize_t err)
{
    PyObject *args, *exc = NULL;
    args = Py_BuildValue("(is)", err, uv_strerror((int) err));
    if (args != NULL) {
        exc = PyObject_Call(PyExc_OSError, args, NULL);
        Py_DECREF(args);
    }
    if (exc == NULL) {
        Promise_RejectWithPyErr(promise);
    } else {
        Promise_Reject(promise, exc);
        Py_DECREF(exc);
    }
}

static void
stat_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
    ssize_t result = req->result;
    if (result >= 0) {
        StatObj *obj = StatObj_New();
        if (obj == NULL) {
            Promise_RejectWithPyErr(Request_Promise(req));
        } else {
            obj->st = req->statbuf;
            Promise_Resolve(Request_Promise(req), (PyObject *) obj);
            Py_DECREF(obj);
        }
    } else {
        promise_reject_with_oserror(Request_Promise(req), result);
    }
    close_file_request(req);
    RELEASE_GIL
}

static void
fstat_noraise_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
    ssize_t result = req->result;
    if (result >= 0) {
        StatObj *obj = StatObj_New();
        if (obj == NULL) {
            Promise_RejectWithPyErr(Request_Promise(req));
        } else {
            obj->st = req->statbuf;
            Promise_Resolve(Request_Promise(req), (PyObject *) obj);
            Py_DECREF(obj);
        }
    } else {
        Promise_Resolve(Request_Promise(req), Py_None);
    }
    close_file_request(req);
    RELEASE_GIL
}

int
Fs_Stat1(Promise *promise, const char *path, int follow_symlinks)
{
    uv_fs_t *req;
    INIT_REQUEST(req, promise)
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    if (follow_symlinks) {
        uv_fs_stat(loop, req, path, stat_callback);
    } else {
        uv_fs_lstat(loop, req, path, stat_callback);
    }
    END_ALLOW_THREADS
    return 0;
}

Promise *
Fs_stat(const char *path, int follow_symlinks)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_Stat1(promise, path, follow_symlinks)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

int
Fs_Fstat1(Promise * promise, uv_file fd)
{
    uv_fs_t *req;
    if (fd < 0) {
        PyErr_SetString(PyExc_ValueError, "negative file descriptor");
        return -1;
    }
    INIT_REQUEST(req, promise)
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_fstat(loop, req, fd, stat_callback);
    END_ALLOW_THREADS
    return 0;
}

Promise *
Fs_fstat(uv_file fd)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_Fstat1(promise, fd)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

int
Fs_Fstat_NoRaise1(Promise *promise, uv_file fd)
{
    uv_fs_t *req;
    if (fd < 0) {
        PyErr_SetString(PyExc_ValueError, "negative file descriptor");
        return -1;
    }
    INIT_REQUEST(req, promise)
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_fstat(loop, req, fd, fstat_noraise_callback);
    END_ALLOW_THREADS
    return 0;
}

typedef struct {
    uv_work_t work;
    uv_file fd;
    Py_off_t pos;
    int whence;
    Py_off_t result;
} fs_seek_req_t;

static void
fs_seek_work(fs_seek_req_t* req)
{
    Py_off_t pos;
#ifdef MS_WINDOWS
    pos = _lseeki64(req->fd, req->pos, req->whence);
#else
    pos = lseek(req->fd, req->pos, req->whence);
#endif
    if (pos < 0) {
        req->result = uv_translate_sys_error(errno);
    } else {
        req->result = pos;
    }
}

static void
seek_callback(fs_seek_req_t* req, int status)
{
    ACQUIRE_GIL
    Py_off_t result = req->result;
    if (result >= 0) {
        PyObject *value = PyLong_FromOff_t(result);
        if (value == NULL) {
            Promise_RejectWithPyErr(Request_Promise(req));
        } else {
            Promise_Resolve(Request_Promise(req), value);
            Py_DECREF(value);
        }
    } else {
        promise_reject_with_oserror(Request_Promise(req), result);
    }
        Request_Close(req);
    RELEASE_GIL
}

int
Fs_seek1(Promise *promise, uv_file fd, Py_off_t pos, int whence)
{
    fs_seek_req_t *req;
    if (fd < 0) {
        PyErr_SetString(PyExc_ValueError, "negative file descriptor");
        return -1;
    }
    if ((req = Request_New(promise, fs_seek_req_t)) == NULL) {
        return -1;
    }
    req->fd = fd;
    req->pos = pos;
    req->whence = whence;
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_queue_work(loop, (uv_work_t *) req, (uv_work_cb) fs_seek_work, (uv_after_work_cb) seek_callback);
    END_ALLOW_THREADS
    return 0;
}

Promise *
Fs_seek(uv_file fd, Py_off_t pos, int whence)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_seek1(promise, fd, pos, whence)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

static void
fileop_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
    if (req->result < 0) {
        promise_reject_with_oserror(Request_Promise(req), req->result);
    } else {
        PyObject *value = PyLong_FromSsize_t(req->result);
        if (value == NULL) {
            Promise_RejectWithPyErr(Request_Promise(req));
        } else {
            Promise_Resolve(Request_Promise(req), value);
            Py_DECREF(value);
        }
    }
    close_file_request(req);
    RELEASE_GIL
}

int
Fs_open1(Promise *promise, const char *path, const char *flags, int mode)
{
    uv_fs_t *req;
    int open_flags = 0, writable = 0, readable = 0;

    for (unsigned long i = 0; i < strlen(flags); i++) {
        char c = flags[i];
        switch (c) {
            case 'x':
                if (writable || readable) {
                bad_mode:
                    PyErr_SetString(PyExc_ValueError,
                                    "must have exactly one of create/read/write/append mode");
                    return -1;
                }
                writable = 1;
                open_flags |= UV_FS_O_EXCL | UV_FS_O_CREAT;
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
                open_flags |= UV_FS_O_CREAT | UV_FS_O_TRUNC;
                break;
            case 'a':
                if (writable || readable) {
                    goto bad_mode;
                }
                writable = 1;
                open_flags |= UV_FS_O_APPEND | UV_FS_O_CREAT;
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
        if (strchr(flags + i + 1, c)) {
          invalid_mode:
            PyErr_Format(PyExc_ValueError, "invalid mode: '%s'", mode);
            return -1;
        }
    }

    if (readable && writable) {
        open_flags |= UV_FS_O_RDWR;
    } else if (readable) {
        open_flags |= UV_FS_O_RDONLY;
    } else if (writable) {
        open_flags |= UV_FS_O_WRONLY;
    } else {
        goto bad_mode;
    }

    #ifdef O_BINARY
        open_flags |= O_BINARY;
    #endif

    #ifdef MS_WINDOWS
        open_flags |= O_NOINHERIT;
    #else
        open_flags |= O_CLOEXEC;
    #endif

    INIT_REQUEST(req, promise);
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_open(loop, req, path, open_flags, mode, fileop_callback);
    END_ALLOW_THREADS
    return 0;
}

Promise *
Fs_open(const char *path, const char *flags, int mode)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_open1(promise, path, flags, mode)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

static int
readbuf1(Promise *promise, uv_file fd, char* buffer, size_t size, Py_off_t offset)
{
    uv_fs_t *req;
    INIT_REQUEST(req, promise)
    uv_buf_t buf = uv_buf_init(buffer, size);
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_read(loop, req, fd, &buf, 1, offset, fileop_callback);
    END_ALLOW_THREADS
    return 0;
}

static Promise *
readbuf(uv_file fd, char* buffer, size_t size, Py_off_t offset)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (readbuf1(promise, fd, buffer, size, offset)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

static PyObject *
read_callback(PyObject *buffer, PyObject *result)
{
    Py_ssize_t size = PyLong_AsSsize_t(result);
    if (size < PyBytes_GET_SIZE(buffer)) {
        if (_PyBytes_Resize(&buffer, size) != 0)
            return NULL;
    }
    return buffer;
}

int
Fs_read1(Promise *promise, uv_file fd, Py_ssize_t size, Py_off_t offset)
{
    PyObject *buffer = PyBytes_FromStringAndSize(NULL, size);
    if (readbuf1(promise, fd, PyBytes_AS_STRING(buffer), size, offset)) {
        Py_DECREF(buffer);
        return -1;
    }
    Promise_CALLBACK(promise, read_callback, NULL, buffer);
    return 0;
}

Promise *
Fs_read(uv_file fd, Py_ssize_t size, Py_off_t offset)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_read1(promise, fd, size, offset)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

Generate_PyType(
    ReadContext,
    MEMBER(fd, uv_file),
    MEMBER(buffer, PyObject *),
    MEMBER(bufsize, size_t),
    MEMBER(bytes_read, Py_ssize_t),
    MEMBER(pos, Py_off_t),
    CLEAR(buffer)
)

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

static PyObject *
readall_readbuf_callback(ReadContext *context, PyObject *n)
{
    if (n != NULL) {
        Py_ssize_t size = PyLong_AsSsize_t(n);
        if (size == 0) {
            if (PyBytes_GET_SIZE(context->buffer) > context->bytes_read) {
                if (_PyBytes_Resize(&context->buffer, context->bytes_read) < 0)
                    return NULL;
            }
            PyObject *result = context->buffer;
            context->buffer = NULL;
            Py_DECREF(context);
            return result;
        }
        context->bytes_read += size;
        context->pos += size;
    }
    if (context->bytes_read >= (Py_ssize_t) context->bufsize) {
        context->bufsize = new_buffersize(context->bytes_read);
        if (context->bufsize > PY_SSIZE_T_MAX || context->bufsize <= 0) {
            PyErr_SetString(PyExc_OverflowError,
                            "unbounded read returned more bytes "
                            "than a Python bytes object can hold");
            Py_DECREF(context);
            return NULL;
        }
        if (PyBytes_GET_SIZE(context->buffer) < (Py_ssize_t) context->bufsize) {
            if (_PyBytes_Resize(&context->buffer, (Py_ssize_t) context->bufsize) < 0) {
                Py_DECREF(context);
                return NULL;
            }
        }
    }
    Promise *promise = readbuf(context->fd,
                               PyBytes_AS_STRING(context->buffer) + context->bytes_read,
                               context->bufsize - context->bytes_read, context->pos);
    if (promise == NULL) {
        Py_DECREF(context);
        return NULL;
    }
    Promise_CALLBACK(promise, readall_readbuf_callback, NULL, context);
    return (PyObject *) promise;
}

static PyObject *
readall_fstat_callback(ReadContext *context, StatObj *value)
{
    Py_off_t pos = context->pos;
    Py_off_t end;
    if ((PyObject *) value == Py_None) {
        end = (Py_off_t) -1;
    } else {
        end = (Py_off_t) value->st.st_size;
    }
    if (end > 0 && end >= pos && pos >= 0 && end - pos < PY_SSIZE_T_MAX) {
        /* This is probably a real file, so we try to allocate a
           buffer one byte larger than the rest of the file.  If the
           calculation is right then we should get EOF without having
           to enlarge the buffer. */
        context->bufsize = (size_t)(end - pos + 1);
    } else {
        context->bufsize = SMALLCHUNK;
    }
    context->buffer = PyBytes_FromStringAndSize(NULL, (Py_ssize_t) context->bufsize);
    if (context->buffer == NULL) {
        Py_DECREF(context);
        return NULL;
    }
    return readall_readbuf_callback(context, NULL);
}

int
Fs_readall1(Promise *promise, int fd, Py_off_t offset)
{
    ReadContext *context = ReadContext_New();
    if (context == NULL) {
        return -1;
    }
    context->fd = fd;
    context->pos = offset;
    context->bytes_read = 0;
    if (Fs_Fstat_NoRaise1(promise, fd)) {
        Py_DECREF(context);
        return -1;
    }
    Promise_CALLBACK(promise, readall_fstat_callback, NULL, context);
    return 0;
}

Promise *
Fs_readall(int fd, Py_off_t offset)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_readall1(promise, fd, offset)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

int
Fs_write1(Promise *promise, int fd, PyObject *data, Py_off_t offset)
{
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes argument expected");
        return -1;
    }
    uv_fs_t *req;
    INIT_REQUEST(req, promise)
    uv_buf_t buf = uv_buf_init(PyBytes_AS_STRING(data), PyBytes_GET_SIZE(data));
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_write(loop, req, fd, &buf, 1, offset, fileop_callback);
    END_ALLOW_THREADS
    Promise_CALLBACK(promise, NULL, NULL, data);
    return 0;
}

Promise *
Fs_write(int fd, PyObject *data, Py_off_t offset)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_write1(promise, fd, data, offset)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

static void
close_callback(uv_fs_t *req)
{
    ACQUIRE_GIL
    ssize_t result = req->result;
    if (result == 0) {
        Promise_Resolve(Request_Promise(req), Py_None);
    } else {
        promise_reject_with_oserror(Request_Promise(req), result);
    }
    close_file_request(req);
    RELEASE_GIL
}

int
Fs_close1(Promise *promise, uv_file fd)
{
    uv_fs_t *req;
    INIT_REQUEST(req, promise);
    uv_loop_t *loop = Loop_Get();
    BEGIN_ALLOW_THREADS
    uv_fs_close(loop, req, fd, close_callback);
    END_ALLOW_THREADS
    return 0;
}

Promise *
Fs_close(uv_file fd)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Fs_close1(promise, fd)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}

PyObject*
Fs_Path(PyObject *path)
{
    path = PyOS_FSPath(path);
    if (path == NULL)
        return NULL;
    if (!PyBytes_Check(path)) {
        PyObject *encoded_path = PyUnicode_AsUTF8String(path);
        Py_DECREF(path);
        if (encoded_path == NULL)
            return NULL;
        path = encoded_path;
    }
    if (PyBytes_GET_SIZE(path) != (Py_ssize_t) strlen(PyBytes_AS_STRING(path))) {
        PyErr_SetString(PyExc_ValueError, "embedded null byte");
        Py_DECREF(path);
        return NULL;
    }
    return path;
}

int
Fs_module_init()
{
    if (PyType_Ready(&StatObj_Type) < 0) {
        return -1;
    }
    return 0;
}
