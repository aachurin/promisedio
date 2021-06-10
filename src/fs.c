// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "fs.h"
#include "utils.h"
#include "clinic/fs.c.h"

/*[clinic input]
module fs
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=de53127b95d08f91]*/


#if BUFSIZ < (8*1024)
    #define SMALLCHUNK (8*1024)
#elif (BUFSIZ >= (2 << 25))
    #error "unreasonable BUFSIZ > 64 MiB defined"
#else
    #define SMALLCHUNK BUFSIZ
#endif

#define BEGIN_FS_CALLBACK(req)  \
    ACQUIRE_GIL                 \
    uv_fs_t *_req = req;

#define END_FS_CALLBACK         \
    uv_fs_req_cleanup(_req);    \
    Request_CLOSE(_req);        \
    RELEASE_GIL

#define UV_FS_PROXY_CALL(promise, func, ...) {      \
    uv_fs_t *req = NULL;                            \
    (promise) = promise_new();                      \
    if (promise) {                                  \
        req = Request_NEW(promise, uv_fs_t);        \
        if (req) {                                  \
            BEGIN_UV_CALL(func, req, __VA_ARGS__)   \
                set_fs_error(error);                \
                Py_DECREF(promise);                 \
                promise = NULL;                     \
            END_UV_CALL                             \
        } else {                                    \
            Py_DECREF(promise);                     \
            promise = NULL;                         \
        }                                           \
    }                                               \
}

static PyObject *fserror;

static PyObject *
Stat_st_dev(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_dev);
}

static PyObject *
Stat_st_mode(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_mode);
}

static PyObject *
Stat_st_nlink(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_nlink);
}

static PyObject *
Stat_st_uid(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_uid);
}

static PyObject *
Stat_st_gid(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_gid);
}

static PyObject *
Stat_st_rdev(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_rdev);
}

static PyObject *
Stat_st_ino(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_ino);
}

static PyObject *
Stat_st_size(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_size);
}

static PyObject *
Stat_st_blksize(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_blksize);
}

static PyObject *
Stat_st_blocks(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_blocks);
}

static PyObject *
Stat_st_flags(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_flags);
}

static PyObject *
Stat_st_gen(Stat *self, __attribute__((unused)) void *closure) {
    return PyLong_FromUint64_t(self->st.st_gen);
}

static PyObject *
Stat_st_atim(Stat *self, __attribute__((unused)) void *closure) {
    return PyFloat_FromDouble((double)self->st.st_atim.tv_sec + 1e-9 * (double)self->st.st_atim.tv_nsec);
}

static PyObject *
Stat_st_mtim(Stat *self, __attribute__((unused)) void *closure) {
    return PyFloat_FromDouble((double)self->st.st_mtim.tv_sec + 1e-9 * (double)self->st.st_mtim.tv_nsec);
}

static PyObject *
Stat_st_ctim(Stat *self, __attribute__((unused)) void *closure) {
    return PyFloat_FromDouble((double)self->st.st_ctim.tv_sec + 1e-9 * (double)self->st.st_ctim.tv_nsec);
}

static PyObject *
Stat_st_birthtim(Stat *self, __attribute__((unused)) void *closure) {
    return PyFloat_FromDouble((double)self->st.st_birthtim.tv_sec + 1e-9 * (double)self->st.st_birthtim.tv_nsec);
}

static PyGetSetDef Stat_getsetlist[] = {
    {"st_dev",      (getter) Stat_st_dev,      NULL, NULL, NULL},
    {"st_mode",     (getter) Stat_st_mode,     NULL, NULL, NULL},
    {"st_nlink",    (getter) Stat_st_nlink,    NULL, NULL, NULL},
    {"st_uid",      (getter) Stat_st_uid,      NULL, NULL, NULL},
    {"st_gid",      (getter) Stat_st_gid,      NULL, NULL, NULL},
    {"st_rdev",     (getter) Stat_st_rdev,     NULL, NULL, NULL},
    {"st_ino",      (getter) Stat_st_ino,      NULL, NULL, NULL},
    {"st_size",     (getter) Stat_st_size,     NULL, NULL, NULL},
    {"st_blksize",  (getter) Stat_st_blksize,  NULL, NULL, NULL},
    {"st_blocks",   (getter) Stat_st_blocks,   NULL, NULL, NULL},
    {"st_flags",    (getter) Stat_st_flags,    NULL, NULL, NULL},
    {"st_gen",      (getter) Stat_st_gen,      NULL, NULL, NULL},
    {"st_atim",     (getter) Stat_st_atim,     NULL, NULL, NULL},
    {"st_mtim",     (getter) Stat_st_mtim,     NULL, NULL, NULL},
    {"st_ctim",     (getter) Stat_st_ctim,     NULL, NULL, NULL},
    {"st_birthtim", (getter) Stat_st_birthtim, NULL, NULL, NULL},
    {NULL}
};

static PyTypeObject StatType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Stat",
    .tp_basicsize = sizeof(Stat),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_getset = Stat_getsetlist,
    .tp_dealloc = (destructor) mem__free
};

static inline void
set_fs_error(int uverr)
{
    set_uv_error(fserror, uverr);
}

static inline void
reject_fs_error(Promise *promise, int uverr)
{
    promise_reject_uv_error(promise, fserror, uverr);
}

static void
int_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *value = PyLong_FromSsize_t(req->result);
        if (!value) {
            promise_reject_py_exc(promise);
        } else {
            promise_resolve(promise, value);
            Py_DECREF(value);
        }
    }
    END_FS_CALLBACK
}

static void
none_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        promise_resolve(promise, Py_None);
    }
    END_FS_CALLBACK
}

static void
bool_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        promise_resolve(promise, Py_False);
    } else {
        promise_resolve(promise, Py_True);
    }
    END_FS_CALLBACK
}

static void
path_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *result = PyUnicode_DecodeFSDefault(req->path);
        if (!result) {
            promise_reject_py_exc(promise);
        } else {
            promise_resolve(promise, result);
            Py_DECREF(result);
        }
    }
    END_FS_CALLBACK
}

static void
ptr_path_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *result = PyUnicode_DecodeFSDefault(req->ptr);
        if (!result) {
            promise_reject_py_exc(promise);
        } else {
            promise_resolve(promise, result);
            Py_DECREF(result);
        }
    }
    END_FS_CALLBACK
}

static void
stat_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        Stat *obj = Mem_NEW(Stat, &StatType);
        if (!obj) {
            promise_reject_py_exc(promise);
        } else {
            obj->st = req->statbuf;
            promise_resolve(promise, (PyObject *) obj);
            Py_DECREF(obj);
        }
    }
    END_FS_CALLBACK
}

Promise *
fs_stat(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_stat, path, stat_callback);
    return promise;
}

Promise *
fs_lstat(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_lstat, path, stat_callback);
    return promise;
}

Promise *
fs_fstat(uv_file fd)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_fstat, fd, stat_callback);
    return promise;
}

typedef struct {
    uv_work_t work;
    uv_file fd;
    Py_off_t pos;
    int whence;
    Py_off_t result;
} fs_seek_req_t;

static void
seek_work(fs_seek_req_t* req)
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
fs__seek(uv_loop_t *loop, fs_seek_req_t *req, uv_file fd, Py_off_t pos, int how, uv_after_work_cb cb)
{
#ifdef SEEK_SET
    /* Turn 0, 1, 2 into SEEK_{SET,CUR,END} */
    switch (how) { // NOLINT(hicpp-multiway-paths-covered)
        case 0: how = SEEK_SET; break;
        case 1: how = SEEK_CUR; break;
        case 2: how = SEEK_END; break;
    }
#endif
    req->fd = fd;
    req->pos = pos;
    req->whence = how;
    uv_queue_work(loop, (uv_work_t *) req, (uv_work_cb) seek_work, cb);
    return 0;
}

static void
seek_callback(fs_seek_req_t* req, int status)
{
    ACQUIRE_GIL
    Promise *promise = Request_PROMISE(req);
    if (status == UV_ECANCELED) {
        reject_fs_error(promise, status);
    } else if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *value = PyLong_FromOff_t(req->result);
        if (!value) {
            promise_reject_py_exc(promise);
        } else {
            promise_resolve(promise, value);
            Py_DECREF(value);
        }
    }
    Request_CLOSE(req);
    RELEASE_GIL
}

Promise *
fs_seek(uv_file fd, Py_off_t pos, int how)
{
    Promise *promise = promise_new();
    if (!promise)
        return NULL;

    fs_seek_req_t *req = Request_NEW(promise, fs_seek_req_t);
    if (!req) {
        Py_DECREF(promise);
        return NULL;
    }
    BEGIN_UV_CALL(fs__seek, req, fd, pos, how, (uv_after_work_cb) seek_callback) END_UV_CALL
    return promise;
}

Promise *
fs_open(const char *path, int flags, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_open, path, flags, mode, int_callback);
    return promise;
}

static inline int
fs__read(uv_loop_t *loop, uv_fs_t *req, uv_file fd, char* buffer, size_t size, Py_off_t offset, uv_fs_cb cb)
{
    uv_buf_t buf;
    buf.base = buffer;
    buf.len = size > _PY_READ_MAX ? _PY_READ_MAX : size;
    return uv_fs_read(loop, req, fd, &buf, 1, offset, cb);
}

static void
read_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    PyObject *buffer = *Promise_DATA(promise, PyObject *);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        goto finally;
    }

    Py_ssize_t size = req->result;
    if (size < PyBytes_GET_SIZE(buffer)) {
        if (_PyBytes_Resize(&buffer, size) != 0) {
            promise_reject_py_exc(promise);
            goto finally;
        }
    }

    promise_resolve(promise, buffer);

    finally:
    Py_XDECREF(buffer);
    END_FS_CALLBACK
}

Promise *
fs_read(uv_file fd, Py_ssize_t size, Py_off_t offset)
{
    if (size > _PY_READ_MAX) {
        size = _PY_READ_MAX;
    }
    PyObject *buffer = PyBytes_FromStringAndSize(NULL, size);
    if (!buffer)
        return NULL;
    Promise *promise;
    UV_FS_PROXY_CALL(promise, fs__read, fd, PyBytes_AS_STRING(buffer), size, offset, read_callback);
    if (!promise) {
        Py_DECREF(buffer);
        return NULL;
    }
    *Promise_DATA(promise, PyObject *) = buffer;
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
    PyObject* buffer;
    Py_ssize_t bytes_read;
    Py_off_t pos;
} read_state_t;

static void
readall_read_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    read_state_t *state = Promise_DATA(promise, read_state_t);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        cleanup:
        Py_DECREF(state->buffer);
        goto finally;
    }

    if (req->result == 0) {
        if (PyBytes_GET_SIZE(state->buffer) > state->bytes_read) {
            if (_PyBytes_Resize(&state->buffer, state->bytes_read) < 0) {
                promise_reject_py_exc(promise);
                goto finally;
            }
        }
        promise_resolve(promise, state->buffer);
        goto cleanup;
    }

    state->bytes_read += req->result;
    if (state->bytes_read >= PyBytes_GET_SIZE(state->buffer)) {
        size_t new_size = new_buffersize(state->bytes_read);
        if (new_size > PY_SSIZE_T_MAX) {
            promise_reject_string(promise, PyExc_OverflowError,
                                  "unbounded read returned more bytes "
                                  "than a Python bytes object can hold");
            goto cleanup;
        }
        if (_PyBytes_Resize(&state->buffer, (Py_ssize_t) new_size) < 0) {
            promise_reject_py_exc(promise);
            goto finally;
        }
    }
    uv_fs_t *nreq = Request_NEW(promise, uv_fs_t);
    if (!nreq) {
        promise_reject_py_exc(promise);
        goto cleanup;
    }

    char *buf = PyBytes_AS_STRING(state->buffer) + state->bytes_read;
    Py_ssize_t size = PyBytes_GET_SIZE(state->buffer) - state->bytes_read;
    BEGIN_UV_CALL(fs__read, nreq, state->fd, buf, size, -1, readall_read_callback)
        reject_fs_error(promise, error);
        goto cleanup;
    END_UV_CALL

    finally:
    END_FS_CALLBACK
}

static void
readall_stat_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    read_state_t *state = Promise_DATA(promise, read_state_t);
    Py_off_t pos = state->pos;
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
        promise_reject_py_exc(promise);
        goto finally;
    }

    uv_fs_t *nreq = Request_NEW(promise, uv_fs_t);
    if (!nreq) {
        promise_reject_py_exc(promise);
        cleanup:
        Py_DECREF(buffer);
        goto finally;
    }

    state->buffer = buffer;
    BEGIN_UV_CALL(fs__read, nreq, state->fd, PyBytes_AS_STRING(buffer), bufsize, -1, readall_read_callback)
        reject_fs_error(promise, error);
        goto cleanup;
    END_UV_CALL

    finally:
    END_FS_CALLBACK
}

static void
readall_seek_callback(fs_seek_req_t *req, int status)
{
    ACQUIRE_GIL
    Promise *promise = Request_PROMISE(req);
    if (status == UV_ECANCELED) {
        reject_fs_error(promise, status);
        goto finally;
    }
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        goto finally;
    }

    read_state_t *state = Promise_DATA(promise, read_state_t);
    state->pos = req->result;
    uv_fs_t *nreq = Request_NEW(promise, uv_fs_t);
    if (!nreq) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    BEGIN_UV_CALL(uv_fs_fstat, nreq, state->fd, readall_stat_callback)
        reject_fs_error(promise, error);
    END_UV_CALL

    finally:
    Request_CLOSE(req);
    RELEASE_GIL
}

Promise *
fs_readall(int fd)
{
    Promise *promise = promise_new();
    if (!promise)
        return NULL;

    fs_seek_req_t *req = Request_NEW(promise, fs_seek_req_t);
    if (!req) {
        Py_DECREF(promise);
        return NULL;
    }

    read_state_t *state = Promise_DATA(promise, read_state_t);
    state->fd = fd;
    state->pos = 0;
    state->bytes_read = 0;
    BEGIN_UV_CALL(fs__seek, req, fd, 0, 1, (uv_after_work_cb) readall_seek_callback) END_UV_CALL
    return promise;
}

static inline int
fs__write(uv_loop_t *loop, uv_fs_t *req, uv_file fd, char* buffer, size_t size, Py_off_t offset, uv_fs_cb cb)
{
    uv_buf_t buf;
    buf.base = buffer;
    buf.len = size > _PY_READ_MAX ? _PY_READ_MAX : size;
    return uv_fs_write(loop, req, fd, &buf, 1, offset, cb);
}

static void
write_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    PyObject *buffer = *Promise_DATA(promise, PyObject *);
    Py_DECREF(buffer);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *size = PyLong_FromSsize_t(req->result);
        promise_resolve(promise, size);
        Py_DECREF(size);
    }
    END_FS_CALLBACK
}

Promise *
fs_write(int fd, PyObject *data, Py_off_t offset)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, fs__write, fd, PyBytes_AS_STRING(data), PyBytes_GET_SIZE(data), offset, write_callback);
    if (promise) {
        *Promise_DATA(promise, PyObject *) = data;
    }
    return promise;
}

Promise *
fs_close(uv_file fd)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_close, fd, none_callback);
    return promise;
}


Promise *
fs_unlink(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_unlink, path, none_callback);
    return promise;
}

Promise *
fs_mkdir(const char *path, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_mkdir, path, mode, none_callback);
    return promise;
}

Promise *
fs_rmdir(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_rmdir, path, none_callback);
    return promise;
}

Promise *
fs_mkdtemp(const char *tpl)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_mkdtemp, tpl, path_callback);
    return promise;
}

static void
mkstemp_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
    } else {
        PyObject *result = Py_BuildValue("ns", req->result, req->path);
        if (result) {
            promise_resolve(promise, result);
            Py_DECREF(result);
        } else {
            promise_reject_py_exc(promise);
        }
    }
    END_FS_CALLBACK
}

Promise *
fs_mkstemp(const char *tpl)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_mkstemp, tpl, mkstemp_callback);
    return promise;
}

static void
scandir_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        goto finally;
    }

    Py_ssize_t n = req->result;
    PyObject *result = PyTuple_New(n);
    if (!result) {
        error:
        promise_reject_py_exc(promise);
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

    promise_resolve(promise, result);
    Py_DECREF(result);

    finally:
    END_FS_CALLBACK
}

Promise *
fs_scandir(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_scandir, path, 0, scandir_callback);
    return promise;
}

Promise *
fs_rename(const char *path, const char *new_path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_rename, path, new_path, none_callback);
    return promise;
}

Promise *
fs_fsync(int fd)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_fsync, fd, none_callback);
    return promise;
}

Promise *
fs_ftruncate(int fd, Py_ssize_t length)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_ftruncate, fd, length, none_callback);
    return promise;
}

Promise *
fs_fdatasync(int fd)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_fdatasync, fd, none_callback);
    return promise;
}

Promise *
fs_copyfile(const char *path, const char *new_path, int flags)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_copyfile, path, new_path, flags, none_callback);
    return promise;
}

Promise *
fs_sendfile(int out_fd, int in_fd, Py_off_t in_offset, size_t length)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_sendfile, out_fd, in_fd, in_offset, length, int_callback);
    return promise;
}

Promise *
fs_access(const char *path, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_access, path, mode, bool_callback);
    return promise;
}

Promise *
fs_chmod(const char *path, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_chmod, path, mode, none_callback);
    return promise;
}

Promise *
fs_fchmod(int fd, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_fchmod, fd, mode, none_callback);
    return promise;
}

Promise *
fs_utime(const char *path, double atime, double mtime)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_utime, path, atime, mtime, none_callback);
    return promise;
}

Promise *
fs_futime(int fd, double atime, double mtime)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_futime, fd, atime, mtime, none_callback);
    return promise;
}

Promise *
fs_lutime(const char *path, double atime, double mtime)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_lutime, path, atime, mtime, none_callback);
    return promise;
}

Promise *
fs_link(const char *path, const char *new_path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_link, path, new_path, none_callback);
    return promise;
}

Promise *
fs_symlink(const char *path, const char *new_path, int flags)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_symlink, path, new_path, flags, none_callback);
    return promise;
}

Promise *
fs_readlink(const char *path)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_readlink, path, ptr_path_callback);
    return promise;
}

/*[clinic input]
class fs.FileIO "FileIO *" "&FileIOType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=60e4ff47d829f04a]*/

/*[clinic input]
fs.FileIO.close

Close this stream.

This method has no effect if the file is already closed. Once the file is closed,
any operation on the file (e.g. reading or writing) will raise a ValueError.
As a convenience, it is allowed to call this method more than once; only the first
call, however, will have an effect.
[clinic start generated code]*/

static inline PyObject *
fs_FileIO_close_impl(FileIO *self)
/*[clinic end generated code: output=d56ab91c4e369cda input=cf8d6fe4f00cec49]*/
{
    if (self->fd < 0) {
        return (PyObject *) promise_new_resolved(Py_None);
    } else if (!self->closefd) {
        self->fd = -1;
        return (PyObject *) promise_new_resolved(Py_None);
    } else {
        int fd = self->fd;
        self->fd = -1;
        return (PyObject *) fs_close(fd);
    }
}

static inline PyObject *
fileio_err_closed(void)
{
    PyErr_SetString(PyExc_ValueError, "I/O operation on closed file");
    return NULL;
}

/*[clinic input]
fs.FileIO.fileno

Return the underlying file descriptor (an integer).
[clinic start generated code]*/

static inline PyObject *
fs_FileIO_fileno_impl(FileIO *self)
/*[clinic end generated code: output=b1a47ffee4b47bd3 input=d2d29c72594b6486]*/
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

static inline PyObject *
fs_FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_off_t offset)
/*[clinic end generated code: output=d3e0eb218f81f033 input=1e3d51dfb810aac3]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be used with the size argument");
            return NULL;
        }
        return (PyObject *) fs_readall(self->fd);
    } else {
        return (PyObject *) fs_read(self->fd, size, offset);
    }
}

/*[clinic input]
fs.FileIO.write
    data: object
    offset: off_t = -1

Write the given bytes to the IO stream.

Returns the number of bytes written, which is always the length of data in bytes.
[clinic start generated code]*/

static inline PyObject *
fs_FileIO_write_impl(FileIO *self, PyObject *data, Py_off_t offset)
/*[clinic end generated code: output=88ae5db085c04d84 input=ce3445150d5a415a]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) fs_write(self->fd, data, offset);
}

/*[clinic input]
fs.FileIO.seek
    offset: off_t
    whence: int = 0

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

static inline PyObject *
fs_FileIO_seek_impl(FileIO *self, Py_off_t offset, int whence)
/*[clinic end generated code: output=2ccb813ff272c60f input=34a065c6cb86e967]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    return (PyObject *) fs_seek(self->fd, offset, whence);
}

static void
fileio_dealloc(FileIO *self)
{
    if (self->fd >= 0) {
        // close silently, ignore errors, sync
        int fd = self->fd;
        self->fd = -1;
        uv_fs_t req;
        uv_fs_close(loop_get(), &req, fd, NULL);
    }
    Mem_DEL(self);
}

static PyObject *
get_closed(FileIO *self, void *closure)
{
    return PyBool_FromLong((long)(self->fd < 0));
}

static PyObject *
get_closefd(FileIO *self, void *closure)
{
    return PyBool_FromLong((long)(self->closefd));
}

static PyGetSetDef FileIOType_getsetlist[] = {
    {"closed", (getter)get_closed, NULL, "True if the file is closed"},
    {"closefd", (getter)get_closefd, NULL, "True if the file descriptor will be closed by close()."},
    {NULL},
};

static PyMethodDef FileIOType_methods[] = {
    FS_FILEIO_CLOSE_METHODDEF
    FS_FILEIO_FILENO_METHODDEF
    FS_FILEIO_READ_METHODDEF
    FS_FILEIO_SEEK_METHODDEF
    FS_FILEIO_WRITE_METHODDEF
    {NULL}  /* Sentinel */
};

static PyTypeObject FileIOType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "FileIO",
    .tp_basicsize = sizeof(FileIO),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor) fileio_dealloc,
    .tp_methods = FileIOType_methods,
    .tp_getset = FileIOType_getsetlist
};

FileIO *
fileio_new(int fd, int closefd)
{
    FileIO *fileobj = Mem_NEW(FileIO, &FileIOType);
    if (!fileobj)
        return NULL;
    fileobj->fd = fd;
    fileobj->closefd = closefd;
    return fileobj;
}

static void
open_stat_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        goto finally;
    }
#if defined(S_ISDIR)
    if (S_ISDIR(req->statbuf.st_mode)) {
        reject_fs_error(promise, UV_EISDIR);
        goto finally;
    }
#endif
    PyObject *fileobj = (PyObject *) fileio_new(*Promise_DATA(promise, uv_file), 1);
    if (!fileobj) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    promise_resolve(promise, fileobj);
    Py_DECREF(fileobj);

    finally:
    END_FS_CALLBACK
}

static void
open_callback(uv_fs_t *req)
{
    BEGIN_FS_CALLBACK(req)
    Promise *promise = Request_PROMISE(req);
    if (req->result < 0) {
        reject_fs_error(promise, (int) req->result);
        goto finally;
    }

    uv_fs_t *nreq = Request_NEW(promise, uv_fs_t);
    if (!nreq) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    uv_file fd = (uv_file) req->result;
    *Promise_DATA(promise, uv_file) = fd;

    BEGIN_UV_CALL(uv_fs_fstat, nreq, fd, open_stat_callback)
        reject_fs_error(promise, error);
    END_UV_CALL

    finally:
    END_FS_CALLBACK
}

Promise *
fs_fileio_open(const char *path, int flags, int mode)
{
    Promise *promise;
    UV_FS_PROXY_CALL(promise, uv_fs_open, path, flags, mode, open_callback);
    return promise;
}

int
fs_module_init(PyObject *module)
{
    if (PyType_Ready(&StatType) < 0)
        return -1;
    if (PyType_Ready(&FileIOType) < 0)
        return -1;
    if (PyModule_AddIntConstant(module, "COPYFILE_EXCL", UV_FS_COPYFILE_EXCL))
        return -1;
    if (PyModule_AddIntConstant(module, "COPYFILE_FICLONE", UV_FS_COPYFILE_FICLONE))
        return -1;
    if (PyModule_AddIntConstant(module, "COPYFILE_FICLONE_FORCE", UV_FS_COPYFILE_FICLONE_FORCE))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_UNKNOWN", UV_DIRENT_UNKNOWN))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_FILE", UV_DIRENT_FILE))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_DIR", UV_DIRENT_DIR))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_LINK", UV_DIRENT_LINK))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_FIFO", UV_DIRENT_FIFO))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_SOCKET", UV_DIRENT_SOCKET))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_CHAR", UV_DIRENT_CHAR))
        return -1;
    if (PyModule_AddIntConstant(module, "DIRENT_BLOCK", UV_DIRENT_BLOCK))
        return -1;
    if (PyModule_AddIntConstant(module, "F_OK", F_OK))
        return -1;
    if (PyModule_AddIntConstant(module, "R_OK", R_OK))
        return -1;
    if (PyModule_AddIntConstant(module, "W_OK", W_OK))
        return -1;
    if (PyModule_AddIntConstant(module, "X_OK", X_OK))
        return -1;
    if (PyModule_AddIntConstant(module, "O_APPEND", UV_FS_O_APPEND))
        return -1;
    if (PyModule_AddIntConstant(module, "O_CREAT", UV_FS_O_CREAT))
        return -1;
    if (PyModule_AddIntConstant(module, "O_EXCL", UV_FS_O_EXCL))
        return -1;
    if (PyModule_AddIntConstant(module, "O_FILEMAP", UV_FS_O_FILEMAP))
        return -1;
    if (PyModule_AddIntConstant(module, "O_RANDOM", UV_FS_O_RANDOM))
        return -1;
    if (PyModule_AddIntConstant(module, "O_RDONLY", UV_FS_O_RDONLY))
        return -1;
    if (PyModule_AddIntConstant(module, "O_RDWR", UV_FS_O_RDWR))
        return -1;
    if (PyModule_AddIntConstant(module, "O_SEQUENTIAL", UV_FS_O_SEQUENTIAL))
        return -1;
    if (PyModule_AddIntConstant(module, "O_SHORT_LIVED", UV_FS_O_SHORT_LIVED))
        return -1;
    if (PyModule_AddIntConstant(module, "O_TEMPORARY", UV_FS_O_TEMPORARY))
        return -1;
    if (PyModule_AddIntConstant(module, "O_TRUNC", UV_FS_O_TRUNC))
        return -1;
    if (PyModule_AddIntConstant(module, "O_WRONLY", UV_FS_O_WRONLY))
        return -1;
    if (PyModule_AddIntConstant(module, "O_DIRECT", UV_FS_O_DIRECT))
        return -1;
    if (PyModule_AddIntConstant(module, "O_DIRECTORY", UV_FS_O_DIRECTORY))
        return -1;
    if (PyModule_AddIntConstant(module, "O_DSYNC", UV_FS_O_DSYNC))
        return -1;
    if (PyModule_AddIntConstant(module, "O_EXLOCK", UV_FS_O_EXLOCK))
        return -1;
    if (PyModule_AddIntConstant(module, "O_NOATIME", UV_FS_O_NOATIME))
        return -1;
    if (PyModule_AddIntConstant(module, "O_NOCTTY", UV_FS_O_NOCTTY))
        return -1;
    if (PyModule_AddIntConstant(module, "O_NOFOLLOW", UV_FS_O_NOFOLLOW))
        return -1;
    if (PyModule_AddIntConstant(module, "O_NONBLOCK", UV_FS_O_NONBLOCK))
        return -1;
    if (PyModule_AddIntConstant(module, "O_SYMLINK", UV_FS_O_SYMLINK))
        return -1;
    if (PyModule_AddIntConstant(module, "O_SYNC", UV_FS_O_SYNC))
        return -1;
    if (PyModule_AddIntConstant(module, "SYMLINK_DIR", UV_FS_SYMLINK_DIR))
        return -1;
    if (PyModule_AddIntConstant(module, "SYMLINK_JUNCTION", UV_FS_SYMLINK_JUNCTION))
        return -1;
    fserror = PyErr_NewException("promisedio.fserror", PyExc_OSError, NULL);
    if (!fserror)
        return -1;
    Py_INCREF(fserror);
    PyModule_AddObject(module, "fserror", fserror);
    return 0;
}
