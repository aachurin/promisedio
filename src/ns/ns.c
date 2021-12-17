// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "core/base.h"
#include "core/capsule.h"
#include "core/handle.h"
#include "core/chain.h"
#include "core/memory.h"
#include "core/module.h"
#include "core/uvh.h"
#include "core/converters/numeric.h"
#include "core/converters/string.h"
#include "core/converters/inet.h"
#include "capsule/loop.h"
#include "capsule/promise.h"

/*[clinic input]
module ns
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=486bb829615864d0]*/

#define READ_BUFFER_SIZE 65536

typedef char ReadBuffer[READ_BUFFER_SIZE];

typedef struct {
    PyObject *GAIError;
    PyObject *LimitOverrunError;
    PyObject *IncompleteReadError;
    PyObject *MemoryBIO;
    PyObject *SSLWantReadError;
    PyObject *SSLWantWriteError;
    PyObject *read_buffer_size;
    PyTypeObject *SSLContextType;
    PyTypeObject *TcpStream;
    PyTypeObject *PipeStream;
//    PyTypeObject *TcpServer;
//    PyTypeObject *PipeServer;
    Freelist_MEM_HEAD(ReadBuffer)
    Capsule_MOUNT(PROMISE_API)
    Capsule_MOUNT(LOOP_API)
} _modulestate;

static PyObject *
make_ipv4_addr(const struct sockaddr_in *addr)
{
    char buf[INET_ADDRSTRLEN];
    int err = uv_ip4_name(addr, buf, sizeof(buf));
    if (err) {
        Py_SetUVError(PyExc_OSError, err);
        return NULL;
    }
    return PyUnicode_FromString(buf);
}

#ifdef ENABLE_IPV6
static PyObject *
make_ipv6_addr(const struct sockaddr_in6 *addr)
{
    char buf[INET6_ADDRSTRLEN];
    int err = uv_ip6_name(addr, buf, sizeof(buf));
    if (err) {
        Py_SetUVError(PyExc_OSError, err);
        return NULL;
    }
    return PyUnicode_FromString(buf);
}
#endif

#include PYSRC(socketmodule.h)
#include PYSRC(socketmodule.c)

static void
getaddrinfo_callback(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (status < 0) {
            Promise_RejectUVError(p, S(GAIError), status);
            goto finally;
        }
        PyObject *result = PyList_New(0);
        if (!result) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        struct addrinfo *next;
        for (next = res; next; next = next->ai_next) {
            // we use native python makesockaddr
            PyObject *addr = makesockaddr(-1, next->ai_addr, next->ai_addrlen, next->ai_protocol);
            if (!addr) {
            error:
                Promise_Reject(p, NULL);
                Py_DECREF(result);
                goto finally;
            }
            PyObject *item = Py_BuildValue("iiisO", next->ai_family, next->ai_socktype, next->ai_protocol,
                                           next->ai_canonname ? next->ai_canonname : "", addr);
            Py_DECREF(addr);
            if (!item)
                goto error;
            int err = PyList_Append(result, item);
            Py_DECREF(item);
            if (err)
                goto error;
        }
        Promise_Resolve(p, result);
        Py_DECREF(result);
    finally:
        uv_freeaddrinfo(res);
        Request_Close(req);
    RELEASE_GIL
}

CAPSULE_API(NS_API, Promise *)
Ns_GetAddrInfo(_ctx_var, const char *node, const char *service, int family, int type, int proto, int flags)
{
    Loop_SETUP(loop)
    Request_SETUP(uv_getaddrinfo_t, req, promise)
    if (req) {
        struct addrinfo hints = {
            .ai_family = family,
            .ai_socktype = type,
            .ai_protocol = proto,
            .ai_flags = flags,
        };
        UV_CALL(uv_getaddrinfo, loop, req, getaddrinfo_callback, node, service, &hints) {
            Request_Close(req);
            Py_SetUVError(S(GAIError), uv_errno);
            Py_DECREF(promise);
            return NULL;
        }
    }
    return promise;
}

/*[clinic input]
ns.getaddrinfo
    node: cstring(accept={NoneType})
    service: object
    family: int(c_default="AF_UNSPEC") = 0
    type: int = 0
    proto: int = 0
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_getaddrinfo_impl(PyObject *module, const char *node, PyObject *service,
                    int family, int type, int proto, int flags)
/*[clinic end generated code: output=59e489da5c432ea4 input=cf8b3eb1ce553acf]*/
{
    char buf[30];
    const char *service_ptr;
    _CTX_set_module(module);
    if (PyLong_Check(service)) {
        long value = PyLong_AsLong(service);
        if (value == -1 && PyErr_Occurred())
            return NULL;
        PyOS_snprintf(buf, sizeof(buf), "%ld", value);
        service_ptr = buf;
    } else if (!cstring_optional_converter(service, &service_ptr)) {
        return NULL;
    }
    return (PyObject *) Ns_GetAddrInfo(_ctx, node, service_ptr, family, type, proto, flags);
}

static void
getnameinfo_callback(uv_getnameinfo_t *req, int status, const char *hostname, const char *service)
{
    ACQUIRE_GIL
        _CTX_set_req(req)
        Promise *p = Request_PROMISE(req);
        if (status < 0) {
            Promise_RejectUVError(p, S(GAIError), status);
            goto finally;
        }
        PyObject *name = sock_decode_hostname(hostname);
        if (!name) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        PyObject *ret = Py_BuildValue("Os", name, service);
        Py_DECREF(name);
        if (!ret) {
            Promise_Reject(p, NULL);
            goto finally;
        }
        Promise_Resolve(p, ret);
        Py_DECREF(ret);
    finally:
        Request_Close(req);
    RELEASE_GIL
}

CAPSULE_API(NS_API, Promise *)
Ns_GetNameInfo(_ctx_var, const struct sockaddr *addr, int flags)
{
    Loop_SETUP(loop)
    Request_SETUP(uv_getnameinfo_t, req, promise)
    if (req) {
        UV_CALL(uv_getnameinfo, loop, req, getnameinfo_callback, addr, flags) {
            Request_Close(req);
            Py_SetUVError(S(GAIError), uv_errno);
            Py_DECREF(promise);
            return NULL;
        }
    }
    return promise;
}

/*[clinic input]
ns.getnameinfo
    sockaddr: inet_addr
    flags: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_getnameinfo_impl(PyObject *module, sockaddr_any *sockaddr, int flags)
/*[clinic end generated code: output=f557f5d59f428a37 input=e2ca53f088a02de6]*/
{
    _CTX_set_module(module);
    return (PyObject *) Ns_GetNameInfo(_ctx, (struct sockaddr *) sockaddr, flags);
}

#define ERROR (-1)
#define OK 0
#define INCOMPLETE 1

#ifdef BUILD_DEBUG_LOG
static char* result_codes[] = {
    "ERROR",
    "OK",
    "INCOMPLETE"
};
#define RESULT_CODE(n) result_codes[n + 1]
#endif

typedef struct streambufferchunk_s streambufferchunk;

struct streambufferchunk_s {
    Chain_NODE(streambufferchunk)
    Py_ssize_t size;
    char data[0];
};

typedef struct {
    Chain_ROOT(streambufferchunk)
    Py_ssize_t chunk_min_size;
    Py_ssize_t nbytes;
    Py_ssize_t offset;
    Py_ssize_t scan_size;
    Py_ssize_t scan_offset;
    streambufferchunk *scan_it;
} streambuffer;

typedef struct {
    Chain_ROOT(Promise)
} backlog;

Py_LOCAL_INLINE(void)
streambuffer_init(streambuffer *buffer, Py_ssize_t chunk_min_size)
{
    Chain_INIT(buffer);
    buffer->chunk_min_size = (chunk_min_size > 0) ? chunk_min_size : (Py_ssize_t) (512 - sizeof(streambufferchunk));
    buffer->nbytes = 0;
    buffer->offset = 0;
    buffer->scan_it = NULL;
}

Py_LOCAL_INLINE(int)
streambuffer_push(streambuffer *buffer, char *data, Py_ssize_t n)
{
    Py_ssize_t offset = 0;
    buffer->nbytes += n;
    streambufferchunk *chunk = Chain_TAIL(buffer);
    if (chunk && chunk->size < buffer->chunk_min_size) {
        Py_ssize_t bytes_to_copy = buffer->chunk_min_size - chunk->size;
        if (n < bytes_to_copy) {
            bytes_to_copy = n;
        }
        memcpy(chunk->data + chunk->size, data, bytes_to_copy);
        chunk->size += bytes_to_copy;
        n -= bytes_to_copy;
        offset = bytes_to_copy;
    }
    if (n <= 0)
        goto finally;
    Py_ssize_t chunk_size = n < buffer->chunk_min_size ? buffer->chunk_min_size : n;
    chunk = Py_Malloc(sizeof(streambufferchunk) + chunk_size);
    if (!chunk) {
        PyErr_NoMemory();
        return ERROR;
    } else {
        chunk->size = n;
        memcpy(chunk->data, data + offset, n);
        Chain_APPEND(buffer, chunk);
    }
    LOG("(%p): new chunk %p, size %zd bytes", buffer, chunk, chunk_size);
finally:
    LOG("(%p): new size %zd bytes", buffer, buffer->nbytes);
    return OK;
}

Py_LOCAL_INLINE(PyObject *)
streambuffer_pull(streambuffer *buffer, Py_ssize_t n)
{
    if (n <= 0 || n > buffer->nbytes) {
        n = buffer->nbytes;
    }

    PyObject *result = PyBytes_FromStringAndSize(NULL, n);
    if (!result)
        return NULL;

    buffer->scan_it = NULL;
    buffer->nbytes -= n;

    streambufferchunk *it;
    char *target = PyBytes_AS_STRING(result);
    Py_ssize_t offset = buffer->offset;

    while ((it = Chain_HEAD(buffer)) && n > 0) {
        Py_ssize_t chunk_size = it->size - offset;
        if (n < chunk_size) {
            memcpy(target, it->data + offset, n);
            offset += n;
            break;
        } else {
            memcpy(target, it->data + offset, chunk_size);
            Chain_HEAD(buffer) = Chain_NEXT(it);
            Py_Free(it);
            target += chunk_size;
            n -= chunk_size;
            offset = 0;
        }
    }

    buffer->offset = offset;
    LOG("(%p): new size %zd bytes", buffer, buffer->nbytes);

    return result;
}

Py_LOCAL_INLINE(Py_ssize_t)
streambuffer_scan(streambuffer *buffer, char c)
{
    Py_ssize_t scan_size, scan_offset;
    streambufferchunk *it;
    if (buffer->scan_it) {
        // continue scan
        it = buffer->scan_it;
        scan_size = buffer->scan_size;
        scan_offset = buffer->scan_offset;
    } else {
        // new scan
        it = Chain_HEAD(buffer);
        scan_size = 0;
        scan_offset = 0;
    }
    if (it) {
        while (1) {
            Py_ssize_t chunk_size = it->size - scan_offset;
            if (chunk_size > 0) {
                char *ptr = memchr(it->data + scan_offset, c, chunk_size);
                if (ptr) {
                    buffer->scan_it = NULL;
                    return scan_size + (ptr - it->data) + 1;
                }
                scan_size += chunk_size;
            }
            scan_offset = 0;
            if (!Chain_NEXT(it)) {
                buffer->scan_it = it;
                buffer->scan_size = scan_size;
                buffer->scan_offset = it->size;
                return -scan_size;
            }
            it = Chain_NEXT(it);
        }
    }
    return 0;
}

static void
streambuffer_clear(streambuffer *buffer)
{
    streambufferchunk *it;
    Chain_PULLALL(it, buffer) {
        Py_Free(it);
    }
    buffer->nbytes = 0;
    buffer->offset = 0;
    buffer->scan_it = NULL;
    LOG("(%p): clear");
}

/*
 * STATE_INIT:
 * Разрешен только метод connect() и close().
 * Остальные методы будут возвращать ошибку UV_ENOTCONN.
 * Метод connect() переводит в состояение STATE_CONNECT.
 * Метод close() закрывает сокет.
 *
 * STATE_CONNECT:
 * Метод close() закрывает сокет, даже если хендшейк не завершен (при этом может вернуть ошибку).
 * Метод connect() возвращает ошибку UV_EALREADY.
 * Остальные методы возвращают ошибку UV_ENOTCONN.
 *
 * STATE_READY:
 * Метод connect() возвращает ошибку UV_EISCONN.
 * Метод close() закрывает сокет, при этом может вернуть ошибку.
 * Можно использовать Методы write() и read().
 *
 * STATE_SHUTDOWN:
 * Метод
 *
 */

#define STATE_CONNECT   0x1
#define STATE_READY     0x2
#define STATE_SHUTDOWN  0x3
#define STATE_ERROR     0x4
#define STATE_CLOSE     0x5

#ifdef BUILD_DEBUG_LOG
static char* state_codes[] = {
    "INIT",
    "CONNECT",
    "READY",
    "SHUTDOWN",
    "ERROR",
    "CLOSE"
};
#define STATE_CODE(n) state_codes[n]
#endif

typedef struct {
    unsigned int state: 3;
    unsigned int reading: 2;            // reading, not reading, end of stream
    unsigned int writing: 1;            // 1 if data is written otherwise 0
    unsigned int sslwriting: 1;         // 1 if ssl data is written otherwise 0
    unsigned int sslreading: 1;         // 1 if ssl data expected otherwise 0
} streamstate;

#define STREAM_HANDLE_FIELDS(base)  \
    Stream *wrapper;                \
    streamstate state;              \
    PyObject *error;                \
    Promise *close_promise;         \
    backlog write_backlog;          \
    backlog read_backlog;           \
    streambuffer incomingbuffer;    \
    Py_ssize_t read_limit;          \
    PyObject *sslobj;               \
    PyObject *inbio;                \
    PyObject *outbio;               \
    HANDLE_BASE(base)

typedef struct stream_s Stream;

typedef struct {
    STREAM_HANDLE_FIELDS(uv_stream_t)
} StreamHandle;

typedef struct {
    STREAM_HANDLE_FIELDS(uv_tcp_t)
} TcpHandle;

typedef struct {
    STREAM_HANDLE_FIELDS(uv_pipe_t)
} PipeHandle;

struct stream_s {
    PyObject_HEAD
    StreamHandle *handle;
};

#include "clinic/ns.c.h"

#define UVBUF_FROM_PYBYTES(op) {.base=PyBytes_AS_STRING(op), .len=PyBytes_GET_SIZE(op)}

static void
stream_dealloc(Stream *self)
{
    StreamHandle *handle = self->handle;
    PyTrack_MarkDeleted(self);
    Py_Delete(self);
    if (handle) {
        handle->wrapper = NULL;
        Handle_Close(handle);
    }
}

static PyObject *
stream_repr(Stream *self)
{
    const char *stream_type = "Closed";
    if (self->handle) {
        stream_type = uv_handle_type_name(self->handle->base.type);
    }
    return PyUnicode_FromFormat("<%sStream object at %p>", self, stream_type);
}

static void stream_read_callback(uv_stream_t *stream, Py_ssize_t nread, const uv_buf_t *buf);

static void
stream_alloc_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    ACQUIRE_GIL
        StreamHandle *stream_handle = Handle_Get(handle, StreamHandle);
        _CTX_set(stream_handle);
        buf->len = READ_BUFFER_SIZE;
        buf->base = (char *) Freelist_Malloc(ReadBuffer);
    RELEASE_GIL
}

Py_LOCAL_INLINE(void)
stream_start_reading(StreamHandle *handle)
{
    if (handle->state.reading == 0) {
        LOG("(%p)", handle);
        handle->state.reading = 1;
        BEGIN_ALLOW_THREADS
            uv_read_start(&handle->base, stream_alloc_callback, stream_read_callback);
        END_ALLOW_THREADS
    }
}

Py_LOCAL_INLINE(void)
stream_stop_reading(StreamHandle *handle)
{
    if (handle->state.reading == 1) {
        LOG("(%p)", handle);
        handle->state.reading = 0;
        BEGIN_ALLOW_THREADS
            uv_read_stop(&handle->base);
        END_ALLOW_THREADS
    }
}

Py_LOCAL_INLINE(int)
stream_set_eof(StreamHandle *handle)
{
    LOG("(%p): EOF", handle);
    return handle->state.reading = 2;
}

Py_LOCAL_INLINE(int)
stream_is_eof(StreamHandle *handle)
{
    return handle->state.reading == 2;
}

Py_LOCAL_INLINE(void)
stream_set_writing(StreamHandle *handle)
{
    handle->state.writing = 1;
}

Py_LOCAL_INLINE(void)
stream_clear_writing(StreamHandle *handle)
{
    handle->state.writing = 0;
}

Py_LOCAL_INLINE(void)
stream_set_sslwriting(StreamHandle *handle)
{
    handle->state.sslwriting = 1;
}

Py_LOCAL_INLINE(void)
stream_clear_sslwriting(StreamHandle *handle)
{
    handle->state.sslwriting = 0;
}

Py_LOCAL_INLINE(void)
stream_set_sslreading(StreamHandle *handle, int sslonly)
{
    handle->state.sslreading = sslonly;
    stream_start_reading(handle);
}

Py_LOCAL_INLINE(int)
stream_is_write_locked(StreamHandle *handle)
{
    return (
        handle->state.writing ||
            handle->state.sslwriting ||
            handle->state.sslreading
    );
}

Py_LOCAL_INLINE(void)
stream_set_state(StreamHandle *handle, int state)
{
    handle->state.state = state;
    LOG("(%p): %s", handle, STATE_CODE(state));
}

Py_LOCAL_INLINE(int)
stream_get_state(StreamHandle *handle)
{
    return handle->state.state;
}

Py_LOCAL_INLINE(void)
stream_acquire(StreamHandle *handle)
{
    Py_XINCREF(handle->wrapper);
}

Py_LOCAL_INLINE(void)
stream_release(StreamHandle *handle)
{
    Py_XDECREF(handle->wrapper);
}

Py_LOCAL_INLINE(void) stream_write_backlog(StreamHandle *handle);

Py_LOCAL_INLINE(void)
stream__set_error(StreamHandle *handle, PyObject *error)
{
    if (!error) {
        error = Py_FetchError();
    }
    assert(error != NULL);
    LOG("(%p, %S)", handle, error);
    stream_stop_reading(handle);
    stream_set_state(handle, STATE_ERROR);
    PyException_SetContext(error, handle->error);
    handle->error = error;
}

Py_LOCAL_INLINE(void)
stream_set_error(StreamHandle *handle)
{
    stream__set_error(handle, NULL);
}

Py_LOCAL_INLINE(void)
stream_set_uv_error(StreamHandle *handle, int err)
{
    stream__set_error(handle, Py_NewUVError(PyExc_OSError, err));
}

static void
stream_write_callback(uv_write_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _CTX_set(handle);
        if (status < 0) {
            Promise_RejectUVError(Request_PROMISE(req), PyExc_OSError, status);
            stream_stop_reading(handle);
            stream_set_state(handle, STATE_ERROR);
        } else {
            Promise_Resolve(Request_PROMISE(req), Py_None);
        }
        stream_clear_writing(handle);
        stream_write_backlog(handle);
        stream_release(handle);
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
stream_write(StreamHandle *handle, Promise *promise)
{
    _CTX_set(handle);

    uv_write_t *req = Request_New(uv_write_t, promise);
    if (!req)
        return ERROR;

    PyObject *data = Promise_GetCtx(promise);
    uv_buf_t buf = UVBUF_FROM_PYBYTES(data);
    UV_CALL(uv_write, req, &handle->base, &buf, 1, stream_write_callback) {
        Request_Close(req);
        Py_SetUVError(PyExc_OSError, uv_errno);
        return ERROR;
    }
    stream_set_writing(handle);
    return OK;
}

static void
sslproto_write_ssldata_callback(uv_write_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        if (status < 0) {
            stream_set_uv_error(handle, status);
        }
        stream_clear_sslwriting(handle);
        stream_write_backlog(handle);
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
sslproto_write_ssldata(StreamHandle *handle)
{
    // this function is used to write only ssl layer data
    _CTX_set(handle);
    _Py_IDENTIFIER(read);

    PyObject *data = _PyObject_CallMethodIdNoArgs(handle->outbio, &PyId_read);
    if (!data)
        return ERROR;

    if (!PyBytes_GET_SIZE(data)) {
        Py_DECREF(data);
        return OK;
    }

    LOG("(%p): send %zd bytes", handle, PyBytes_GET_SIZE(data));

    uv_write_t *req = Request_New(uv_write_t, data);
    if (!req)
        return ERROR;

    uv_buf_t buf = UVBUF_FROM_PYBYTES(data);
    UV_CALL(uv_write, req, &handle->base, &buf, 1, sslproto_write_ssldata_callback) {
        Request_Close(req);
        Py_SetUVError(PyExc_OSError, uv_errno);
        return ERROR;
    }
    stream_set_sslwriting(handle);

    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_write_appdata(StreamHandle *handle, Promise *promise)
{
    _Py_IDENTIFIER(read);
    PyObject *data = _PyObject_CallMethodIdNoArgs(handle->outbio, &PyId_read);
    if (!data)
        return ERROR;
    if (PyBytes_GET_SIZE(data) > 0) {
        PyTrack_MarkAllocated(data);
        PyTrack_DECREF(Promise_SetCtx(promise, data));
        return stream_write(handle, promise);
    }
    Py_DECREF(data);
    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_check_error(StreamHandle *handle, int sslonly)
{
    _CTX_set(handle);
    if (PyErr_ExceptionMatches(S(SSLWantReadError))) {
        LOG("(%p): SSLWantReadError", handle);
        stream_set_sslreading(handle, sslonly);
//    } else if (PyErr_ExceptionMatches(S(SSLWantWriteError))) {
//        // Should not happen with mem_bio?
//        LOG("(%p): SSLWantWriteError ", handle);
    } else {
        LOG("(%p): ERROR", handle);
        return ERROR;
    }
    PyErr_Clear();
    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_write(StreamHandle *handle, Promise *promise)
{
    int err;
    _Py_IDENTIFIER(write);

    LOG("(%p): start", handle);

    PyObject *appdata = Promise_GetCtx(promise);
    PyObject *buffer = PyMemoryView_FromObject(appdata);

    if (!buffer) {
        err = ERROR;
        goto finally;
    }

    PyObject *ret = _PyObject_CallMethodIdOneArg(handle->sslobj, &PyId_write, buffer);
    Py_DECREF(buffer);

    if (ret) {
        Py_DECREF(ret);
        err = sslproto_write_appdata(handle, promise);
    } else {
        if (sslproto_check_error(handle, 1)) {
            err = ERROR;
        } else if (sslproto_write_ssldata(handle)) {
            err = ERROR;
        } else {
            err = INCOMPLETE;
        }
    }
finally:
    LOG("(%p): finish %s", handle, RESULT_CODE(err));
    return err;
}

Py_LOCAL_INLINE(int)
stream_connected(StreamHandle *handle, Promise *promise)
{
    _CTX_set(handle);
    Promise_Resolve(promise, (PyObject *) handle->wrapper);
    stream_set_state(handle, STATE_READY);
    Py_DECREF(handle->wrapper);
    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_do_handshake(StreamHandle *handle, Promise *promise)
{
    int err;
    _Py_IDENTIFIER(do_handshake);

    LOG("(%p): start", handle);

    PyObject *ret = _PyObject_CallMethodIdNoArgs(handle->sslobj, &PyId_do_handshake);

    if (ret) {
        Py_DECREF(ret);
        err = OK;
    } else {
        if (sslproto_check_error(handle, 1)) {
            err = ERROR;
            goto finally;
        }
        err = INCOMPLETE;
    }

    if (sslproto_write_ssldata(handle)) {
        err = ERROR;
        goto finally;
    }

    if (err == OK) {
        stream_connected(handle, promise);
    }

finally:
    LOG("(%p): finish %s", handle, RESULT_CODE(err));
    return err;
}

static void
stream_shutdown_callback(uv_shutdown_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _CTX_set(handle);
        if (status < 0) {
            Promise_RejectUVError(Request_PROMISE(req), PyExc_OSError, status);
            stream_stop_reading(handle);
            stream_set_state(handle, STATE_ERROR);
        } else {
            Promise_Resolve(Request_PROMISE(req), Py_None);
        }
        stream_clear_writing(handle);
        stream_write_backlog(handle);
        stream_release(handle);
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
stream_shutdown(StreamHandle *handle, Promise *promise)
{
    _CTX_set(handle);
    uv_shutdown_t *req = Request_New(uv_shutdown_t, promise);
    if (!req)
        return ERROR;

    UV_CALL(uv_shutdown, req, &handle->base, stream_shutdown_callback) {
        Request_Close(req);
        Py_SetUVError(PyExc_OSError, uv_errno);
        return ERROR;
    }
    stream_set_writing(handle);

    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_shutdown(StreamHandle *handle, Promise *promise)
{
    int err;
    _Py_IDENTIFIER(shutdown);

    LOG("(%p): start", handle);

    PyObject *ret = _PyObject_CallMethodIdNoArgs(handle->sslobj, &PyId_shutdown);

    if (ret) {
        Py_DECREF(ret);
        err = stream_shutdown(handle, promise);
    } else {
        if (sslproto_check_error(handle, 1)) {
            err = ERROR;
        } else if (sslproto_write_ssldata(handle)) {
            err = ERROR;
        } else {
            err = INCOMPLETE;
        }
    }

    LOG("(%p): finish %s", handle, RESULT_CODE(err));
    return err;
}

#define STREAM_WRITE 0
#define STREAM_CONNECT 1
#define STREAM_SHUTDOWN 2

Py_LOCAL_INLINE(void)
stream_reject_backlog_item(StreamHandle *handle, Promise *it)
{
    _CTX_set(handle);
    if (handle->error) {
        Promise_Reject(it, handle->error);
        Py_CLEAR(handle->error);
    } else if (stream_get_state(handle) == STATE_ERROR) {
        Promise_RejectUVError(it, PyExc_OSError, UV_EPIPE);
    } else {
        Promise_RejectUVError(it, PyExc_OSError, UV_ECANCELED);
    }
    Py_DECREF(it);
    stream_release(handle);
}

Py_LOCAL_INLINE(void)
stream_write_backlog(StreamHandle *handle)
{
    Promise *it;
    int err;

    if (stream_is_write_locked(handle))
        return;

    Chain_PULLALL(it, &handle->write_backlog) {
        if (stream_get_state(handle) >= STATE_ERROR) {
        on_error:
            stream_reject_backlog_item(handle, it);
            continue;
        }
        switch (*Promise_DATA(it, int)) {
            case STREAM_WRITE: {
                if (handle->sslobj) {
                    err = sslproto_write(handle, it);
                } else {
                    err = stream_write(handle, it);
                }
                break;
            }
            case STREAM_CONNECT: {
                if (handle->sslobj) {
                    err = sslproto_do_handshake(handle, it);
                } else {
                    err = stream_connected(handle, it);
                }
                break;
            }
            case STREAM_SHUTDOWN: {
                if (handle->sslobj) {
                    err = sslproto_shutdown(handle, it);
                } else {
                    err = stream_shutdown(handle, it);
                }
                break;
            }
        }
        if (err == ERROR) {
            stream_set_error(handle);
            goto on_error;
        }
        if (err == INCOMPLETE) {
            // could not complete write operation, try it later
            Chain_APPENDLEFT(&handle->write_backlog, it);
            break;
        }
        Py_DECREF(it);
        return;
    }
}

Py_LOCAL_INLINE(void) stream_read_backlog(StreamHandle *handle);

Py_LOCAL_INLINE(int)
stream_feed(StreamHandle *handle, char *buffer, Py_ssize_t nread)
{
    if (buffer)
        return streambuffer_push(&handle->incomingbuffer, buffer, nread);
    return OK;
}

Py_LOCAL_INLINE(int)
sslproto_feed(StreamHandle *handle, char *buffer, Py_ssize_t size)
{
    _CTX_set(handle);
    if (buffer) {
        PyObject *pybuffer = PyMemoryView_FromMemory(buffer, size, PyBUF_READ);
        if (!pybuffer)
            return ERROR;
        _Py_IDENTIFIER(write);
        PyObject *ret = _PyObject_CallMethodIdOneArg(handle->inbio, &PyId_write, pybuffer);
        Py_DECREF(pybuffer);
        if (!ret)
            return ERROR;
        Py_DECREF(ret);
    }

    if (handle->state.sslreading) {
        handle->state.sslreading = 0;
        return OK;
    }

    LOG("(%p)", handle);
    int err = OK;
    _Py_IDENTIFIER(read);
    while (1) {
        PyObject *data = _PyObject_CallMethodIdOneArg(handle->sslobj, &PyId_read, S(read_buffer_size));
        if (!data) {
            err = ERROR;
            break;
        }
        if (PyBytes_GET_SIZE(data)) {
            err = stream_feed(handle, PyBytes_AS_STRING(data), PyBytes_GET_SIZE(data));
            Py_DECREF(data);
            if (err) {
                break;
            }
        } else {
            Py_DECREF(data);
            break;
        }
    }

    if (err == ERROR) {
        if (sslproto_check_error(handle, 0)) {
            err = ERROR;
            goto finally;
        }
        err = OK;
    }

    if (sslproto_write_ssldata(handle)) {
        err = ERROR;
    }

finally:
    LOG("(%p): finish %s", handle, RESULT_CODE(err));
    return err;
}

Py_LOCAL_INLINE(void)
sslproto_set_eof(StreamHandle *handle)
{
    _Py_IDENTIFIER(write_eof);
    Py_XDECREF(_PyObject_CallMethodIdNoArgs(handle->inbio, &PyId_write_eof));
}

static void
stream_read_callback(uv_stream_t *stream, Py_ssize_t nread, const uv_buf_t *buf)
{
    ACQUIRE_GIL
        int err;
        StreamHandle *handle = Handle_Get(stream, StreamHandle);
        _CTX_set(handle);
        if (nread != 0) {
            char *buffer = buf->base;
            if (nread < 0) {
                LOG("(%p): %ld, %s", handle, nread, uv_strerror(nread));
                stream_stop_reading(handle);
                stream_set_eof(handle);
                if (handle->sslobj) {
                    sslproto_set_eof(handle);
                }
                if (nread != UV_EOF) {
                    stream_set_uv_error(handle, nread);
                }
            } else {
                LOG("(%p): received %zd byte(s)", handle, nread);
                if (handle->sslobj) {
                    err = sslproto_feed(handle, buffer, nread);
                } else {
                    err = stream_feed(handle, buffer, nread);
                }
                if (err == ERROR) {
                    stream_set_error(handle);
                }
            }
            stream_read_backlog(handle);
            stream_write_backlog(handle);
        }
        Freelist_Free(ReadBuffer, buf->base);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
stream_read_n(StreamHandle *handle, Promise *promise, Py_ssize_t n)
{
    _CTX_set(handle);
    if (handle->incomingbuffer.nbytes > 0) {
        PyObject *data = streambuffer_pull(&handle->incomingbuffer, n);
        if (!data)
            return ERROR;
        Promise_Resolve(promise, data);
        Py_DECREF(data);
        return OK;
    }
    if (stream_is_eof(handle)) {
        PyObject *data = PyBytes_FromStringAndSize(NULL, 0);
        if (!data)
            return ERROR;
        Promise_Resolve(promise, data);
        Py_DECREF(data);
        return OK;
    }
    return INCOMPLETE;
}

Py_LOCAL_INLINE(int)
stream_read_exactly(StreamHandle *handle, Promise *promise, Py_ssize_t n)
{
    _CTX_set(handle);
    if (handle->incomingbuffer.nbytes >= n) {
        PyObject *data = streambuffer_pull(&handle->incomingbuffer, n);
        if (!data)
            return ERROR;
        Promise_Resolve(promise, data);
        Py_DECREF(data);
        return OK;
    }
    return INCOMPLETE;
}

Py_LOCAL_INLINE(int)
stream_read_until(StreamHandle *handle, Promise *promise, char c)
{
    _CTX_set(handle);
    Py_ssize_t nbytes = streambuffer_scan(&handle->incomingbuffer, c);
    if (nbytes > 0) {
        PyObject *data = streambuffer_pull(&handle->incomingbuffer, nbytes);
        if (!data)
            return ERROR;
        Promise_Resolve(promise, data);
        Py_DECREF(data);
        return OK;
    } else if (-nbytes >= handle->read_limit) {
        PyObject *exc = Py_NewError(S(LimitOverrunError),
                                    "Separator is not found, and buffer exceed the limit");
        if (!exc)
            return ERROR;
        Promise_Reject(promise, exc);
        Py_DECREF(exc);
        return OK;
    }
    return INCOMPLETE;
}

Py_LOCAL_INLINE(int)
stream_incomplete_read(StreamHandle *handle, Promise *promise)
{
    _CTX_set(handle);
    PyObject *data = streambuffer_pull(&handle->incomingbuffer, -1);
    if (!data)
        return ERROR;
    PyObject *exc = Py_NewError(S(IncompleteReadError), "end of stream");
    if (!exc) {
        Py_DECREF(data);
        return ERROR;
    }
    PyObject_SetAttrString(exc, "partial", data);
    Promise_Reject(promise, exc);
    Py_DECREF(exc);
    Py_DECREF(data);
    return OK;
}

#define STREAM_READ_N       0x0
#define STREAM_READ_EXACTLY 0x1
#define STREAM_READ_UNTIL   0x2

typedef struct {
    int op;
    Py_ssize_t n;
    char c;
} streamreadargs;

Py_LOCAL_INLINE(void)
stream_read_backlog(StreamHandle *handle)
{
    Promise *it;
    int unlimited_read = 0;
    Chain_PULLALL(it, &handle->read_backlog) {
        if (stream_get_state(handle) >= STATE_ERROR && !handle->incomingbuffer.nbytes) {
        stream_error:
            stream_reject_backlog_item(handle, it);
            continue;
        }
        int err;
        streamreadargs *args = Promise_DATA(it, streamreadargs);
        switch (args->op) {
            case STREAM_READ_N:
                err = stream_read_n(handle, it, args->n);
                break;
            case STREAM_READ_EXACTLY:
                err = stream_read_exactly(handle, it, args->n);
                break;
            case STREAM_READ_UNTIL:
                err = stream_read_until(handle, it, args->c);
                break;
        }
        if (err == ERROR) {
        memory_error:
            streambuffer_clear(&handle->incomingbuffer);
            stream_set_error(handle);
            goto stream_error;
        }
        if (err == INCOMPLETE) {
            if (stream_get_state(handle) == STATE_ERROR || stream_is_eof(handle)) {
                // Error or EOF
                if (stream_incomplete_read(handle, it))
                    goto memory_error;
            } else {
                unlimited_read = args->op == STREAM_READ_EXACTLY;
                Chain_APPENDLEFT(&handle->read_backlog, it);
                break;
            }
        }
        Py_DECREF(it);
        stream_release(handle);
    }
    if (stream_get_state(handle) == STATE_READY) {
        if (unlimited_read || handle->incomingbuffer.nbytes < handle->read_limit) {
            stream_start_reading(handle);
        } else {
            stream_stop_reading(handle);
        }
    }
}

Py_LOCAL_INLINE(Promise *)
stream_read(StreamHandle *handle, int op, Py_ssize_t n, char c)
{
    _CTX_set(handle);
    Promise *promise = Promise_New();
    if (!promise)
        return NULL;

    *Promise_DATA(promise, streamreadargs) = (streamreadargs) {op, n, c};
    int empty_backlog = Chain_HEAD(&handle->read_backlog) == NULL;
    Py_INCREF(promise);
    Chain_APPEND(&handle->read_backlog, promise);
    stream_acquire(handle);
    if (empty_backlog) {
        stream_read_backlog(handle);
    }

    return promise;
}

Py_LOCAL_INLINE(int)
stream_handle_init(StreamHandle *handle, PyObject *sslcontext, Py_ssize_t limit, Py_ssize_t chunk_size,
                   const char *server_hostname, int server_socket)
{
    _CTX_set(handle);
    Py_ssize_t read_limit = limit <= 0 ? 65536 : limit;
    handle->wrapper = NULL;
    handle->state = (streamstate) {0};
    handle->error = NULL;
    handle->close_promise = NULL;
    Chain_INIT(&handle->read_backlog);
    Chain_INIT(&handle->write_backlog);
    streambuffer_init(&handle->incomingbuffer, chunk_size);
    handle->read_limit = read_limit;
    handle->sslobj = NULL;

    if (sslcontext) {
        PyObject *inbio = NULL, *outbio = NULL;
        inbio = PyObject_CallNoArgs(S(MemoryBIO));
        if (!inbio)
            goto fail;
        outbio = PyObject_CallNoArgs(S(MemoryBIO));
        if (!outbio)
            goto fail;
        _Py_IDENTIFIER(_wrap_bio);
        if (server_socket) {
            handle->sslobj = _PyObject_CallMethodId(sslcontext, &PyId__wrap_bio, "OOi", inbio, outbio, 1);
        } else {
            handle->sslobj = _PyObject_CallMethodId(sslcontext, &PyId__wrap_bio, "OOis", inbio, outbio, 0, server_hostname);
        }
        if (!handle->sslobj)
            goto fail;
        handle->inbio = inbio;
        handle->outbio = outbio;
        return OK;

    fail:
        Py_XDECREF(inbio);
        Py_XDECREF(outbio);
        return ERROR;
    }
    return OK;
}

static void
stream_handle_finalizer(StreamHandle *handle)
{
    if (handle->wrapper) {
        handle->wrapper->handle = NULL;
    }
    Py_CLEAR(handle->error);
    streambuffer_clear(&handle->incomingbuffer);
    stream_set_state(handle, STATE_CLOSE);
    // reject all read operations
    stream_read_backlog(handle);
    // reject all write operations
    stream_write_backlog(handle);
    if (handle->sslobj) {
        Py_CLEAR(handle->sslobj);
        Py_CLEAR(handle->inbio);
        Py_CLEAR(handle->outbio);
    }
    if (handle->close_promise) {
        _CTX_set(handle);
        Promise_Resolve(handle->close_promise, Py_None);
        Py_CLEAR(handle->close_promise);
    }
}

Py_LOCAL_INLINE(void)
stream_push_writeop(StreamHandle *handle, Promise *promise, int op)
{
    *Promise_DATA(promise, int) = op;
    Py_INCREF(promise);
    Chain_APPEND(&handle->write_backlog, promise);
}

static void
stream_connect_callback(uv_connect_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _CTX_set(handle);
        Promise *promise = Request_PROMISE(req);
        if (status < 0) {
            Py_CLEAR(handle->wrapper);
            Promise_RejectUVError(promise, PyExc_OSError, status);
        } else {
            stream_start_reading(handle);
            stream_push_writeop(handle, promise, STREAM_CONNECT);
            stream_write_backlog(handle);
        }
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
stream_check_alive(Stream *stream)
{
    if (!stream->handle || stream_get_state(stream->handle) == STATE_CLOSE) {
        Py_SetUVError(PyExc_OSError, UV_EBADF);
        return ERROR;
    }
    return OK;
}

/* Capsule API */

CAPSULE_API(NS_API, PyObject *)
TcpStream_GetSockName(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    struct sockaddr_storage sa;
    int sa_len = sizeof(sa);
    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return NULL;
    }
    int ret;
    BEGIN_ALLOW_THREADS
        ret = uv_tcp_getsockname((uv_tcp_t *) &stream->handle->base, (struct sockaddr *) &sa, &sa_len);
    END_ALLOW_THREADS
    if (ret < 0) {
        Py_SetUVError(PyExc_OSError, ret);
        return NULL;
    }

    return makesockaddr(-1, ((struct sockaddr *) &sa), sa_len, 0);
}

CAPSULE_API(NS_API, PyObject *)
TcpStream_GetPeerName(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    struct sockaddr_storage sa;
    int sa_len = sizeof(sa);
    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return NULL;
    }
    int ret;
    BEGIN_ALLOW_THREADS
        ret = uv_tcp_getpeername((uv_tcp_t *) &stream->handle->base, (struct sockaddr *) &sa, &sa_len);
    END_ALLOW_THREADS
    if (ret < 0) {
        Py_SetUVError(PyExc_OSError, ret);
        return NULL;
    }

    return makesockaddr(-1, ((struct sockaddr *) &sa), sa_len, 0);
}

CAPSULE_API(NS_API, int)
TcpStream_SetTcpNoDelay(Stream *stream, int enabled)
{
    if (stream_check_alive(stream))
        return ERROR;

    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return ERROR;
    }

    int err = uv_tcp_nodelay((uv_tcp_t *) &stream->handle->base, enabled > 0);
    if (err) {
        Py_SetUVError(PyExc_OSError, err);
        return ERROR;
    }

    return OK;
}

CAPSULE_API(NS_API, int)
TcpStream_SetTcpKeepAlive(Stream *stream, int delay)
{
    if (stream_check_alive(stream))
        return ERROR;

    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return ERROR;
    }

    int err = uv_tcp_keepalive((uv_tcp_t *) &stream->handle->base, delay > 0, delay);
    if (err) {
        Py_SetUVError(PyExc_OSError, err);
        return ERROR;
    }

    return OK;
}

CAPSULE_API(NS_API, Promise *)
Stream_Close(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    _CTX_set(stream->handle);
    Promise *promise = Promise_New();
    if (!promise)
        return NULL;

    StreamHandle *handle = stream->handle;
    stream->handle = NULL;
    stream_set_state(handle, STATE_CLOSE);
    handle->close_promise = promise;
    Handle_Close(handle);

    return (Promise *) Py_NewRef(promise);
}

CAPSULE_API(NS_API, Promise *)
Stream_Shutdown(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    StreamHandle *handle = stream->handle;
    _CTX_set(handle);

    Promise *promise = Promise_New();
    if (!promise)
        return NULL;

    stream_acquire(handle);
    stream_set_state(handle, STATE_SHUTDOWN);
    stream_push_writeop(handle, promise, STREAM_SHUTDOWN);
    stream_write_backlog(handle);

    return promise;
}

CAPSULE_API(NS_API, Promise *)
Stream_Write(Stream *stream, PyObject *data)
{
    if (stream_check_alive(stream))
        return NULL;

    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError, "bytes expected");
        return NULL;
    }

    StreamHandle *handle = stream->handle;
    _CTX_set(handle);
    Promise *promise = Promise_New();
    if (!promise)
        return NULL;

    PyTrack_INCREF(data);
    Promise_SetCtx(promise, data);

    stream_acquire(handle);
    stream_push_writeop(handle, promise, STREAM_WRITE);
    stream_write_backlog(handle);

    return promise;
}

CAPSULE_API(NS_API, Promise *)
Stream_Read(Stream *stream, Py_ssize_t n)
{
    if (stream_check_alive(stream))
        return NULL;
    if (n == 0) {
        PyErr_SetString(PyExc_ValueError, "n must be greater than zero");
        return NULL;
    }
    return stream_read(stream->handle, STREAM_READ_N, n, 0);
}

CAPSULE_API(NS_API, Promise *)
Stream_ReadExactly(Stream *stream, Py_ssize_t n)
{
    if (stream_check_alive(stream))
        return NULL;
    if (n <= 0) {
        PyErr_SetString(PyExc_ValueError, "n must be greater than zero");
        return NULL;
    }
    return stream_read(stream->handle, STREAM_READ_EXACTLY, n, 0);
}

CAPSULE_API(NS_API, Promise *)
Stream_ReadUntil(Stream *stream, char c)
{
    if (stream_check_alive(stream))
        return NULL;
    return stream_read(stream->handle, STREAM_READ_UNTIL, 0, c);
}

CAPSULE_API(NS_API, Promise *)
Stream_OpenConnection(_ctx_var, const struct sockaddr *remote_addr, const struct sockaddr *local_addr,
                      Py_ssize_t limit, Py_ssize_t chunk_size, int nodelay, int keepalive,
                      PyObject *ssl, const char *server_hostname)
{
    Stream *self = (Stream *) Py_New(S(TcpStream));
    if (!self)
        return NULL;

    PyTrack_MarkAllocated(self);

    self->handle = NULL;
    StreamHandle *handle = (StreamHandle *) Handle_New(TcpHandle, stream_handle_finalizer);

    if (!handle) {
    cleanup_self:
        Py_DECREF(self);
        return NULL;
    }

    if (stream_handle_init(handle, ssl, limit, chunk_size, server_hostname, 0)) {
    cleanup_handle:
        if (handle)
            Handle_Free(handle);
        goto cleanup_self;
    }

    Loop_SETUP(loop);
    Request_SETUP(uv_connect_t, conn_req, promise);

    if (!conn_req) {
    cleanup_req:
        Request_Close(conn_req);
        Py_DECREF(promise);
        goto cleanup_handle;
    }

    {
        UV_CALL(uv_tcp_init, loop, (uv_tcp_t *) &handle->base) {
            Py_SetUVError(PyExc_OSError, uv_errno);
            goto cleanup_req;
        }
    }

    handle->wrapper = self;
    self->handle = handle;
    stream_set_state(handle, STATE_CONNECT);

    if (nodelay >= 0) {
        uv_tcp_nodelay((uv_tcp_t *) &handle->base, nodelay > 0);
    }

    if (keepalive >= 0) {
        uv_tcp_keepalive((uv_tcp_t *) &handle->base, keepalive > 0, keepalive);
    }

    if (local_addr) {
        UV_CALL(uv_tcp_bind, (uv_tcp_t *) &handle->base, local_addr, 0) {
            handle = NULL; // handle closed in stream destructor
            Py_SetUVError(PyExc_OSError, uv_errno);
            goto cleanup_req;
        }
    }

    {
        UV_CALL(uv_tcp_connect, conn_req, (uv_tcp_t *) &handle->base, remote_addr, stream_connect_callback) {
            handle = NULL; // handle closed in stream destructor
            Py_SetUVError(PyExc_OSError, uv_errno);
            goto cleanup_req;
        }
    }

    return promise;
}

/*[clinic input]
class ns.TcpStream "Stream *" "S(TcpStream)"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=9335b838fe0b483f]*/

/*[clinic input]
ns.open_connection
    remote_addr: inet_addr
    *
    local_addr: object = NULL
    limit: Py_ssize_t = -1
    chunk_size: Py_ssize_t = -1
    nodelay: int = -1
    keepalive: int = -1
    ssl: object(subclass_of="_CTX_get_module(module)->SSLContextType") = NULL
    server_hostname: object = NULL

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_open_connection_impl(PyObject *module, sockaddr_any *remote_addr,
                        PyObject *local_addr, Py_ssize_t limit,
                        Py_ssize_t chunk_size, int nodelay, int keepalive,
                        PyObject *ssl, PyObject *server_hostname)
/*[clinic end generated code: output=f7062a6fee7a41d6 input=145a4efe27a91071]*/
{
    _CTX_set_module(module);
    sockaddr_any local_addr_s;
    sockaddr_any *local_addr_ptr = NULL;
    if (local_addr) {
        if (!inet_addr_converter(local_addr, &local_addr_s))
            return NULL;
        local_addr_ptr = &local_addr_s;
    }
    char *hostname = NULL;
    if (server_hostname) {
        if (!PyArg_Parse(server_hostname, "es", "ascii", &hostname))
            return NULL;
    }
    Promise *ret = Stream_OpenConnection(_ctx, (struct sockaddr *) remote_addr, (struct sockaddr *) local_addr_ptr,
                                         limit, chunk_size, nodelay, keepalive, ssl, hostname);
    if (hostname)
        Py_Free(hostname);
    return (PyObject *) ret;
}

/*[clinic input]
ns.TcpStream.set_tcp_nodelay
    enabled: bool(accept={int})

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_set_tcp_nodelay_impl(Stream *self, int enabled)
/*[clinic end generated code: output=56a1b0cb588cb0cf input=f228d670c209c71c]*/
{
    if (TcpStream_SetTcpNoDelay(self, enabled)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
ns.TcpStream.set_tcp_keepalive
    delay: int

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_set_tcp_keepalive_impl(Stream *self, int delay)
/*[clinic end generated code: output=0764bcbc450955f0 input=fea0246258a79108]*/
{
    if (TcpStream_SetTcpKeepAlive(self, delay)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
ns.TcpStream.getpeername

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_getpeername_impl(Stream *self)
/*[clinic end generated code: output=2e40824379a3a5c4 input=fb45d21e630a22d4]*/
{
    return TcpStream_GetPeerName(self);
}

/*[clinic input]
ns.TcpStream.getsockname

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_getsockname_impl(Stream *self)
/*[clinic end generated code: output=e96207c8f5dd0354 input=ead01cb6e6896ba6]*/
{
    return TcpStream_GetSockName(self);
}

/*[clinic input]
ns.TcpStream.write
    data: object

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_write_impl(Stream *self, PyObject *data)
/*[clinic end generated code: output=9423aff598070b90 input=f51c3fb94b64d42d]*/
{
    return (PyObject *) Stream_Write(self, data);
}

/*[clinic input]
ns.TcpStream.read
    n: ssize_t = -1

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_read_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=947fd3453039e96a input=e8f37fd8d6a56256]*/
{
    return (PyObject *) Stream_Read(self, n);
}

/*[clinic input]
ns.TcpStream.readexactly
    n: ssize_t

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_readexactly_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=895f49979238a17f input=5e96c2f5ae103842]*/
{
    return (PyObject *) Stream_ReadExactly(self, n);
}

/*[clinic input]
ns.TcpStream.readuntil
    c: char

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_readuntil_impl(Stream *self, char c)
/*[clinic end generated code: output=ba29ac419c1099d4 input=faed01d3602600ae]*/
{
    return (PyObject *) Stream_ReadUntil(self, c);
}

/*[clinic input]
ns.TcpStream.shutdown

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_shutdown_impl(Stream *self)
/*[clinic end generated code: output=183d1b2be0c72cbf input=239467237491a31c]*/
{
    return (PyObject *) Stream_Shutdown(self);
}

/*[clinic input]
ns.TcpStream.close

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_close_impl(Stream *self)
/*[clinic end generated code: output=f6840eb2be33b2bb input=4df88388144cf70c]*/
{
    return (PyObject *) Stream_Close(self);
}

static PyMethodDef tcpstream_methods[] = {
    NS_TCPSTREAM_GETSOCKNAME_METHODDEF
    NS_TCPSTREAM_GETPEERNAME_METHODDEF
    NS_TCPSTREAM_SET_TCP_KEEPALIVE_METHODDEF
    NS_TCPSTREAM_SET_TCP_NODELAY_METHODDEF
    NS_TCPSTREAM_WRITE_METHODDEF
    NS_TCPSTREAM_READ_METHODDEF
    NS_TCPSTREAM_READEXACTLY_METHODDEF
    NS_TCPSTREAM_READUNTIL_METHODDEF
    NS_TCPSTREAM_SHUTDOWN_METHODDEF
    NS_TCPSTREAM_CLOSE_METHODDEF
    {NULL}
};

static PyType_Slot tcpstream_slots[] = {
    {Py_tp_methods, tcpstream_methods},
    {Py_tp_repr, stream_repr},
    {Py_tp_dealloc, stream_dealloc},
    {0, 0},
};

static PyType_Spec tcpstream_spec = {
    "promisedio.ns.TcpStream",
    sizeof(Stream),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    tcpstream_slots
};

CAPSULE_API(NS_API, Promise *)
Stream_OpenUnixConnection(_ctx_var, const char *path, Py_ssize_t limit, Py_ssize_t chunk_size,
                          PyObject *ssl, const char *server_hostname)
{
    Stream *self = (Stream *) Py_New(S(PipeStream));
    if (!self)
        return NULL;

    PyTrack_MarkAllocated(self);

    self->handle = NULL;
    StreamHandle *handle = (StreamHandle *) Handle_New(PipeHandle, stream_handle_finalizer);

    if (!handle) {
    cleanup_self:
        Py_DECREF(self);
        return NULL;
    }

    if (stream_handle_init(handle, ssl, limit, chunk_size, server_hostname, 0)) {
    cleanup_handle:
        Handle_Free(handle);
        goto cleanup_self;
    }

    Loop_SETUP(loop);
    Request_SETUP(uv_connect_t, conn_req, promise);

    if (!conn_req) {
    cleanup_req:
        Request_Close(conn_req);
        Py_DECREF(promise);
        goto cleanup_handle;
    }

    UV_CALL(uv_pipe_init, loop, (uv_pipe_t *) &handle->base, 0) {
        Py_SetUVError(PyExc_OSError, uv_errno);
        goto cleanup_req;
    }

    handle->wrapper = self;
    self->handle = handle;
    stream_set_state(handle, STATE_CONNECT);

    BEGIN_ALLOW_THREADS
        uv_pipe_connect(conn_req, (uv_pipe_t *) &handle->base, path, stream_connect_callback);
    END_ALLOW_THREADS

    return promise;
}

CAPSULE_API(NS_API, PyObject *)
PipeStream_GetPeerName(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    if (stream->handle->base.type != UV_NAMED_PIPE) {
        PyErr_SetString(PyExc_TypeError, "Can be used with Pipe handle only");
        return NULL;
    }

    char mock[1];
    size_t size = 0;
    {
        UV_CALL(uv_pipe_getpeername, (uv_pipe_t *) &stream->handle->base, mock, &size) {
            Py_SetUVError(PyExc_OSError, uv_errno);
            return NULL;
        }
    }

    PyObject *result = PyBytes_FromStringAndSize(NULL, size);
    if (!result)
        return NULL;

    {
        UV_CALL(uv_pipe_getpeername, (uv_pipe_t *) &stream->handle->base, PyBytes_AS_STRING(result), &size) {
            Py_DECREF(result);
            Py_SetUVError(PyExc_OSError, uv_errno);
            return NULL;
        }
    }

    return result;
}

CAPSULE_API(NS_API, PyObject *)
PipeStream_GetSockName(Stream *stream)
{
    if (stream_check_alive(stream))
        return NULL;

    if (stream->handle->base.type != UV_NAMED_PIPE) {
        PyErr_SetString(PyExc_TypeError, "Can be used with Pipe handle only");
        return NULL;
    }
    char mock[1];
    size_t size = 0;
    {
        UV_CALL(uv_pipe_getsockname, (uv_pipe_t *) &stream->handle->base, mock, &size) {
            Py_SetUVError(PyExc_OSError, uv_errno);
            return NULL;
        }
    }

    PyObject *result = PyBytes_FromStringAndSize(NULL, size);
    if (!result)
        return NULL;

    {
        UV_CALL(uv_pipe_getsockname, (uv_pipe_t *) &stream->handle->base, PyBytes_AS_STRING(result), &size) {
            Py_DECREF(result);
            Py_SetUVError(PyExc_OSError, uv_errno);
            return NULL;
        }
    }

    return result;
}

/*[clinic input]
class ns.PipeStream "Stream *" "S(PipeStream)"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=222d37785f31e241]*/

/*[clinic input]
ns.open_unix_connection
    path: Path
    *
    limit: Py_ssize_t = -1
    chunk_size: Py_ssize_t = -1
    ssl: object(subclass_of="_CTX_get_module(module)->SSLContextType") = NULL
    server_hostname: object = NULL

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_open_unix_connection_impl(PyObject *module, PyObject *path,
                             Py_ssize_t limit, Py_ssize_t chunk_size,
                             PyObject *ssl, PyObject *server_hostname)
/*[clinic end generated code: output=638816ef396382f2 input=300c84e7b1a3b502]*/
{
    _CTX_set_module(module);
    char *hostname = NULL;
    if (server_hostname) {
        if (!PyArg_Parse(server_hostname, "es", "ascii", &hostname))
            return NULL;
    }
    Promise *ret = Stream_OpenUnixConnection(_ctx, PyBytes_AS_STRING(path), limit, chunk_size, ssl, hostname);
    if (hostname)
        Py_Free(hostname);
    return (PyObject *) ret;
}

/*[clinic input]
ns.PipeStream.getpeername

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_getpeername_impl(Stream *self)
/*[clinic end generated code: output=48dd39e769f4f297 input=9155813a1ca58fb7]*/
{
    return PipeStream_GetPeerName(self);
}

/*[clinic input]
ns.PipeStream.getsockname

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_getsockname_impl(Stream *self)
/*[clinic end generated code: output=652709e6e439d166 input=75b6ff408b4f3466]*/
{
    return PipeStream_GetSockName(self);
}

/*[clinic input]
ns.PipeStream.write
    data: object

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_write_impl(Stream *self, PyObject *data)
/*[clinic end generated code: output=30f61bfcc9b41fd5 input=aabcf4e53d4cdc38]*/
{
    return (PyObject *) Stream_Write(self, data);
}

/*[clinic input]
ns.PipeStream.read
    n: ssize_t = -1

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_read_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=c65c6945dab2d8ae input=8ad9596803e43cb3]*/
{
    return (PyObject *) Stream_Read(self, n);
}

/*[clinic input]
ns.PipeStream.readexactly
    n: ssize_t

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_readexactly_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=31eca4488c27269a input=fb7a7013386637bd]*/
{
    return (PyObject *) Stream_ReadExactly(self, n);
}

/*[clinic input]
ns.PipeStream.readuntil
    c: char

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_readuntil_impl(Stream *self, char c)
/*[clinic end generated code: output=190a548932ab703b input=316eaec2c6f26831]*/
{
    return (PyObject *) Stream_ReadUntil(self, c);
}

/*[clinic input]
ns.PipeStream.shutdown

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_shutdown_impl(Stream *self)
/*[clinic end generated code: output=374baa799c2fb0f6 input=71dd6434d63f8d49]*/
{
    return (PyObject *) Stream_Shutdown(self);
}

/*[clinic input]
ns.PipeStream.close

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
ns_PipeStream_close_impl(Stream *self)
/*[clinic end generated code: output=3d06443c42081672 input=421e5e764a12b92f]*/
{
    return (PyObject *) Stream_Close(self);
}

static PyMethodDef pipestream_methods[] = {
    NS_PIPESTREAM_GETSOCKNAME_METHODDEF
    NS_PIPESTREAM_GETPEERNAME_METHODDEF
    NS_PIPESTREAM_WRITE_METHODDEF
    NS_PIPESTREAM_READ_METHODDEF
    NS_PIPESTREAM_READEXACTLY_METHODDEF
    NS_PIPESTREAM_READUNTIL_METHODDEF
    NS_PIPESTREAM_SHUTDOWN_METHODDEF
    NS_PIPESTREAM_CLOSE_METHODDEF
    {NULL}
};

static PyType_Slot pipestream_slots[] = {
    {Py_tp_methods, pipestream_methods},
    {Py_tp_repr, stream_repr},
    {Py_tp_dealloc, stream_dealloc},
    {0, 0},
};

static PyType_Spec pipestream_spec = {
    "promisedio.ns.PipeStream",
    sizeof(Stream),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    pipestream_slots
};

static int
nsmodule_init(PyObject *module)
{
    LOG("(%p)", module);
    _CTX_set_module(module);
    Capsule_LOAD("promisedio.promise", PROMISE_API);
    Capsule_LOAD("promisedio.loop", LOOP_API);
    return 0;
}

static int
nsmodule_init_types(PyObject *module)
{
    LOG("(%p)", module);
    _CTX_set_module(module);
    S(GAIError) = PyErr_NewException("promisedio.ns.GAIError", PyExc_OSError, NULL);
    if (!S(GAIError))
        return -1;
    S(LimitOverrunError) = PyErr_NewException("promisedio.ns.LimitOverrunError", PyExc_Exception, NULL);
    if (!S(LimitOverrunError))
        return -1;
    S(IncompleteReadError) = PyErr_NewException("promisedio.ns.IncompleteReadError", PyExc_EOFError, NULL);
    if (!S(IncompleteReadError))
        return -1;
    S(TcpStream) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &tcpstream_spec, NULL);
    if (S(TcpStream) == NULL)
        return -1;
    S(PipeStream) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &pipestream_spec, NULL);
    if (S(PipeStream) == NULL)
        return -1;
//    S(TcpServer) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &tcpserver_spec, NULL);
//    if (S(TcpServer) == NULL)
//        return -1;
//    S(PipeServer) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &pipeserver_spec, NULL);
//    if (S(PipeServer) == NULL)
//        return -1;
    S(read_buffer_size) = PyLong_FromSsize_t(READ_BUFFER_SIZE);
    if (S(read_buffer_size) == NULL)
        return -1;
    PyObject *d = PyModule_GetDict(module);
    if (PyDict_SetItemString(d, "GAIError", S(GAIError)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "LimitOverrunError", S(LimitOverrunError)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "IncompleteReadError", S(IncompleteReadError)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "TcpStream", (PyObject *) S(TcpStream)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "PipeStream", (PyObject *) S(PipeStream)) < 0)
        return -1;
    Freelist_MEM_INIT(ReadBuffer, 8);
    // SSL
    int ret = -1;
    PyObject *ssl = PyImport_ImportModule("_ssl");
    if (!ssl)
        goto finally;
    S(MemoryBIO) = PyObject_GetAttrString(ssl, "MemoryBIO");
    if (!S(MemoryBIO))
        goto finally;
    S(SSLContextType) = (PyTypeObject *) PyObject_GetAttrString(ssl, "_SSLContext");
    if (!S(SSLContextType))
        goto finally;
    S(SSLWantReadError) = PyObject_GetAttrString(ssl, "SSLWantReadError");
    if (!S(SSLWantReadError))
        goto finally;
    S(SSLWantWriteError) = PyObject_GetAttrString(ssl, "SSLWantWriteError");
    if (!S(SSLWantWriteError))
        goto finally;
    ret = 0;

finally:
    Py_XDECREF(ssl);
    return ret;
}

#include "capsule.h"

static int
nsmodule_create_api(PyObject *module)
{
    LOG("(%p)", module);
    Capsule_CREATE(module, NS_API);
    return 0;
}

static int
nsmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _CTX_set_module(module);
    Capsule_VISIT(LOOP_API);
    Capsule_VISIT(PROMISE_API);
    Py_VISIT(S(GAIError));
    Py_VISIT(S(LimitOverrunError));
    Py_VISIT(S(IncompleteReadError));
    Py_VISIT(S(TcpStream));
    Py_VISIT(S(PipeStream));
//    Py_VISIT(S(TcpServer));
//    Py_VISIT(S(PipeServer));
    Py_VISIT(S(read_buffer_size));
    // SSL
    Py_VISIT(S(MemoryBIO));
    Py_VISIT(S(SSLContextType));
    Py_VISIT(S(SSLWantReadError));
    Py_VISIT(S(SSLWantWriteError));
    return 0;
}

static int
nsmodule_clear(PyObject *module)
{
    _CTX_set_module(module);
    Py_CLEAR(S(GAIError));
    Py_CLEAR(S(LimitOverrunError));
    Py_CLEAR(S(IncompleteReadError));
    Py_CLEAR(S(TcpStream));
    Py_CLEAR(S(PipeStream));
//    Py_CLEAR(S(TcpServer));
//    Py_CLEAR(S(PipeServer));
    Py_CLEAR(S(read_buffer_size));
    // SSL
    Py_CLEAR(S(MemoryBIO));
    Py_CLEAR(S(SSLContextType));
    Py_CLEAR(S(SSLWantReadError));
    Py_CLEAR(S(SSLWantWriteError));
    return 0;
}

static void
nsmodule_free(void *module)
{
    LOG("(%p)", module);
    _CTX_set_module(module);
    Capsule_CLEAR(LOOP_API);
    Capsule_CLEAR(PROMISE_API);
    nsmodule_clear(module);
    Freelist_Mem_Clear(ReadBuffer);
}

static PyMethodDef nsmodule_methods[] = {
    NS_GETADDRINFO_METHODDEF
    NS_GETNAMEINFO_METHODDEF
    NS_OPEN_CONNECTION_METHODDEF
    NS_OPEN_UNIX_CONNECTION_METHODDEF
    {NULL},
};

static PyModuleDef_Slot nsmodule_slots[] = {
    {Py_mod_exec, nsmodule_init},
    {Py_mod_exec, nsmodule_init_types},
    {Py_mod_exec, nsmodule_create_api},
    {0, NULL},
};

static struct PyModuleDef nsmodule_def = {
    PyModuleDef_HEAD_INIT,
    "promisedio.ns",
    NULL,
    sizeof(_modulestate),
    nsmodule_methods,
    nsmodule_slots,
    nsmodule_traverse,
    nsmodule_clear,
    nsmodule_free,
};

PyMODINIT_FUNC
PyInit_ns(void)
{
    return PyModuleDef_Init(&nsmodule_def);
}
