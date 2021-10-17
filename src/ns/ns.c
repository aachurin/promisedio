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
#include "loop/loop_api.h"
#include "promise/promise_api.h"
#include "ns.h"
#include "clinic/ns.c.h"

/*[clinic input]
module nsmodule
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=b40868e825e69d87]*/

#define READ_BUFFER_SIZE 65536

typedef char ReadBuffer[READ_BUFFER_SIZE];

typedef struct {
    uv_loop_t *loop;
    PyObject *GAIError;
    PyObject *LimitOverrunError;
    PyObject *IncompleteReadError;
    PyObject *StreamType;
    PyObject *MemoryBIO;
    PyObject *SSLWantReadError;
    PyObject *SSLWantWriteError;
    Freelist_TYPE_HEAD(ReadBuffer)
    Capsule_MOUNT_POINT(PROMISE_API_ID)
    Capsule_MOUNT_POINT(LOOP_API_ID)
} _modulestate;

static PyObject *
make_ipv4_addr(const struct sockaddr_in *addr)
{
    char buf[INET_ADDRSTRLEN];
    int err = uv_ip4_name(addr, buf, sizeof(buf));
    if (err) {
        PyErr_SetUVError(PyExc_OSError, err);
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
        PyErr_SetUVError(PyExc_OSError, err);
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
        _STATE_setreq(req)
        if (status < 0) {
            Request_RejectUVError(req, S(GAIError), status);
            goto finally;
        }
        PyObject *result = PyList_New(0);
        if (!result) {
            Request_RejectPyExc(req);
            goto finally;
        }
        struct addrinfo *next;
        for (next = res; next; next = next->ai_next) {
            // we use native python makesockaddr
            PyObject *addr = makesockaddr(-1, next->ai_addr, next->ai_addrlen, next->ai_protocol);
            if (!addr) {
            error:
                Request_RejectPyExc(req);
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
        Request_Resolve(req, result);
        Py_DECREF(result);

    finally:
        uv_freeaddrinfo(res);
        Request_Close(req);
    RELEASE_GIL
}

PyObject *
Ns_GetAddrInfo(_STATE_var, const char *node, const char *service, int family, int type, int proto, int flags)
{
    Request_SETUP(uv_getaddrinfo_t, req, promise);
    if (req) {
        struct addrinfo hints = {
            .ai_family = family,
            .ai_socktype = type,
            .ai_protocol = proto,
            .ai_flags = flags,
        };
        UV_CALL(uv_getaddrinfo, S(loop), req, getaddrinfo_callback, node, service, &hints) {
            Request_Close(req);
            PyErr_SetUVError(S(GAIError), error);
            Py_DECREF(promise);
            return NULL;
        }
    }
    return promise;
}

/*[clinic input]
nsmodule.getaddrinfo
    node: cstring(accept={NoneType})
    service: object
    family: int(c_default="AF_UNSPEC") = 0
    type: int = 0
    proto: int = 0
    flags: int = 0
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_getaddrinfo_impl(PyObject *module, const char *node,
                          PyObject *service, int family, int type, int proto,
                          int flags)
/*[clinic end generated code: output=04ae94cf3eae47ed input=220dadae351baefb]*/
{
    char buf[30];
    const char *service_ptr;
    _STATE_setmodule(module)
    if (PyLong_Check(service)) {
        long value = PyLong_AsLong(service);
        if (value == -1 && PyErr_Occurred())
            return NULL;
        PyOS_snprintf(buf, sizeof(buf), "%ld", value);
        service_ptr = buf;
    } else if (!cstring_optional_converter(service, &service_ptr)) {
        return NULL;
    }
    return (PyObject *) Ns_GetAddrInfo(_state, node, service_ptr, family, type, proto, flags);
}

static void
getnameinfo_callback(uv_getnameinfo_t *req, int status, const char *hostname, const char *service)
{
    ACQUIRE_GIL
        _STATE_setreq(req)

        if (status < 0) {
            Request_RejectUVError(req, S(GAIError), status);
            goto finally;
        }

        PyObject *name = sock_decode_hostname(hostname);
        if (!name) {
            Request_RejectPyExc(req);
            goto finally;
        }

        PyObject *ret = Py_BuildValue("Os", name, service);
        Py_DECREF(name);
        if (!ret) {
            Request_RejectPyExc(req);
            goto finally;
        }

        Request_Resolve(req, ret);
        Py_DECREF(ret);

    finally:
        Request_Close(req);

    RELEASE_GIL
}

PyObject *
Ns_GetNameInfo(_STATE_var, const struct sockaddr *addr, int flags)
{
    Request_SETUP(uv_getnameinfo_t, req, promise);
    if (req) {
        UV_CALL(uv_getnameinfo, S(loop), req, getnameinfo_callback, addr, flags) {
            Request_Close(req);
            PyErr_SetUVError(S(GAIError), error);
            Py_DECREF(promise);
            return NULL;
        }
    }
    return promise;
}

/*[clinic input]
nsmodule.getnameinfo
    sockaddr: inet_addr
    flags: int
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_getnameinfo_impl(PyObject *module, sockaddr_any *sockaddr,
                          int flags)
/*[clinic end generated code: output=875ade476ac42633 input=90fb8c8fbccfbb46]*/
{
    _STATE_setmodule(module)
    return (PyObject *) Ns_GetNameInfo(_state, (struct sockaddr *) sockaddr, flags);
}

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
} streambuffer;

typedef struct {
    Chain_ROOT(Promise)
} backlog;

static void
streambuffer_init(streambuffer *buffer, Py_ssize_t chunk_min_size)
{
    Chain_INIT(buffer);
    buffer->chunk_min_size = (chunk_min_size > 0) ? chunk_min_size : (Py_ssize_t) (512 - sizeof(streambufferchunk));
    buffer->nbytes = 0;
    buffer->offset = 0;
}

static int
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
        return 0;

    Py_ssize_t chunk_size = n < buffer->chunk_min_size ? buffer->chunk_min_size : n;
    chunk = Mem_Malloc(sizeof(streambufferchunk) + chunk_size);
    if (!chunk) {
        return -1;
    } else {
        chunk->size = n;
        memcpy(chunk->data, data + offset, n);
        Chain_APPEND(buffer, chunk);
    }
    return 0;
}

static PyObject *
streambuffer_pull(streambuffer *buffer, Py_ssize_t n)
{
    if (n <= 0 || n > buffer->nbytes) {
        n = buffer->nbytes;
    }

    PyObject *result = PyBytes_FromStringAndSize(NULL, n);
    if (result == NULL) {
        return NULL;
    }

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
            Mem_Free(it);
            target += chunk_size;
            n -= chunk_size;
            offset = 0;
        }
    }

    buffer->offset = offset;
    return result;
}

static void
streambuffer_clear(streambuffer *buffer)
{
    streambufferchunk *it;
    while ((it = Chain_HEAD(buffer))) {
        Chain_HEAD(buffer) = Chain_NEXT(it);
        Mem_Free(it);
    }
    buffer->nbytes = 0;
    buffer->offset = 0;
}

enum sslproto_state {
    SSL_DO_HANDSHAKE = 0,
    SSL_WRAPPED = 1,
    SSL_SHUTDOWN = 2
};

#define STREAM_HANDLE_FIELDS(base)  \
    int status;                     \
    Stream *wrapper;                \
    streambuffer buffer;            \
    PyObject *connect_promise;      \
    backlog write_backlog;          \
    backlog read_backlog;           \
    Py_ssize_t read_limit;          \
    Py_ssize_t read_trigger;        \
    PyObject *error;                \
    int sslproto;                   \
    enum sslproto_state ssl_state;  \
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
} TCPHandle;

typedef struct {
    STREAM_HANDLE_FIELDS(uv_udp_t)
} UDPHandle;

typedef struct {
    STREAM_HANDLE_FIELDS(uv_pipe_t)
} PipeHandle;

typedef struct stream_s {
    PyObject_HEAD
    StreamHandle *handle;
} Stream;

Py_LOCAL_INLINE(void) stream_set_exception(StreamHandle *handle, PyObject *exc);

/*[clinic input]
class nsmodule.Stream "Stream *" "&StreamType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=7cb05718284e0c51]*/

Py_LOCAL_INLINE(int)
ns_stream_closed(Stream *stream)
{
    return stream->handle ? 0 : 1;
}

Py_LOCAL_INLINE(int)
check_stream_alive(Stream *stream)
{
    if (ns_stream_closed(stream)) {
        PyErr_SetUVError(PyExc_OSError, UV_EBADF);
        return -1;
    }
    return 0;
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_getsockname(Stream *stream)
{
    if (check_stream_alive(stream))
        return NULL;

    struct sockaddr_storage sa;
    int sa_len = sizeof(sa);
    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return NULL;
    }
    int ret;
    Py_BEGIN_ALLOW_THREADS
        ret = uv_tcp_getsockname((uv_tcp_t *) &stream->handle->base, (struct sockaddr *) &sa, &sa_len);
    Py_END_ALLOW_THREADS
    if (ret < 0) {
        PyErr_SetUVError(PyExc_OSError, ret);
        return NULL;
    }
    return makesockaddr(-1, ((struct sockaddr *) &sa), sa_len, 0);
}

/*[clinic input]
nsmodule.Stream.getsockname

@doc[Stream.getsockname]
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_getsockname_impl(Stream *self)
/*[clinic end generated code: output=f22759caf219b4c7 input=1b7602faf59fa15f]*/
{
    return ns_stream_getsockname(self);
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_getpeername(Stream *stream)
{
    if (check_stream_alive(stream))
        return NULL;

    struct sockaddr_storage sa;
    int sa_len = sizeof(sa);
    if (stream->handle->base.type != UV_TCP) {
        PyErr_SetString(PyExc_TypeError, "Can be used with TCP handle only");
        return NULL;
    }
    int ret;
    Py_BEGIN_ALLOW_THREADS
        ret = uv_tcp_getpeername((uv_tcp_t *) &stream->handle->base, (struct sockaddr *) &sa, &sa_len);
    Py_END_ALLOW_THREADS
    if (ret < 0) {
        PyErr_SetUVError(PyExc_OSError, ret);
        return NULL;
    }
    return makesockaddr(-1, ((struct sockaddr *) &sa), sa_len, 0);
}

/*[clinic input]
nsmodule.Stream.getpeername

@doc[Stream.getpeername]
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_getpeername_impl(Stream *self)
/*[clinic end generated code: output=0f4fe2701a0fe59b input=aea9bb36f3f44b9c]*/
{
    return ns_stream_getpeername(self);
}

static void
stream_write_callback(uv_write_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _STATE_set(handle)
        if (status < 0) {
            Request_RejectUVError(req, PyExc_OSError, status);
        } else {
            Request_Resolve(req, Py_None);
        }
        Py_XDECREF(handle->wrapper);
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(int)
stream_write(StreamHandle *handle, PyObject *promise)
{
    _STATE_set(handle)
    uv_write_t *req = Request_New(uv_write_t, promise);
    if (!req) {
        return -1;
    }
    PyObject *data = Promise_GetCtx(promise);
    uv_buf_t buf = {
        .base = PyBytes_AS_STRING(data),
        .len = PyBytes_GET_SIZE(data)
    };
    UV_CALL(uv_write, req, &handle->base, &buf, 1, stream_write_callback) {
        PyErr_SetUVError(PyExc_OSError, error);
        Request_Close(req);
        return -1;
    }
    Py_XINCREF(handle->wrapper);
    return 0;
}

Py_LOCAL_INLINE(int)
sslproto_is_transient_error(_STATE_var)
{
    return PyErr_ExceptionMatches(S(SSLWantReadError)) || PyErr_ExceptionMatches(S(SSLWantWriteError));
}

Py_LOCAL_INLINE(int)
sslproto_write(StreamHandle *handle, PyObject *promise)
{
    _STATE_set(handle)
    PyObject *appdata = Promise_GetCtx(promise);
    PyObject *buffer = PyMemoryView_FromObject(appdata);
    if (!buffer)
        return -1;
    _Py_IDENTIFIER(write);
    PyObject *write_ret = _PyObject_CallMethodIdOneArg(handle->sslobj, &PyId_write, buffer);
    Py_DECREF(buffer);
    if (!write_ret) {
        if (sslproto_is_transient_error(_state)) {
            PyErr_Clear();
            return 1;
        } else {
            return -1;
        }
    }
    Py_DECREF(write_ret);
    _Py_IDENTIFIER(read);
    PyObject *ssldata = _PyObject_CallMethodIdNoArgs(handle->outbio, &PyId_read);
    if (!ssldata) {
        return -1;
    }
    if (PyBytes_GET_SIZE(ssldata) > 0) {
        Py_DECREF(appdata);
        Promise_SetCtx(promise, ssldata);
        return stream_write(handle, promise);
    } else {
        Py_DECREF(ssldata);
    }
    return 0;
}

Py_LOCAL_INLINE(int)
sslproto_write_backlog(StreamHandle *handle, PyObject *promise)
{
    _STATE_set(handle)
    PyObject *err = NULL;
    PyObject *it = (PyObject *) Chain_HEAD(&handle->write_backlog);
    while (it) {
        PyObject *next = (PyObject *) Chain_NEXT((Promise *) it);
        if (err) {
        error:
            Promise_Reject(it, err);
        } else {
            int ret = sslproto_write(handle, (PyObject *) it);
            if (ret < 0) {
                err = fetch_current_exception();
                goto error;
            } else if (ret > 0) {
                break;
            }
            // To make sure GC won't fail
            Chain_NEXT((Promise *) it) = NULL;
        }
        Py_DECREF(it);
        it = next;
        Chain_HEAD(&handle->write_backlog) = (Promise *) it;
    }
    if (err) {
        PyErr_SetObject((PyObject *) Py_TYPE(err), err);
        Py_DECREF(err);
        return -1;
    }
    int ret = sslproto_write(handle, promise);
    if (ret < 0) {
        return -1;
    } else if (ret > 0) {
        Chain_APPEND(&handle->write_backlog, (Promise *) Py_NewRef(promise));
        return 0;
    }
    return 0;
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_write(Stream *stream, PyObject *data)
{
    if (check_stream_alive(stream))
        return NULL;

    StreamHandle *handle = stream->handle;

    _STATE_set(handle)

    PyObject *promise = Promise_New();
    if (!promise)
        return NULL;

    Promise_SetCtx(promise, Py_NewRef(data));

    if (handle->sslproto) {
        if (sslproto_write_backlog(handle, promise)) {
            Py_DECREF(promise);
            return NULL;
        }
    } else if (stream_write(handle, promise)) {
        Py_DECREF(promise);
        return NULL;
    }

    return promise;
}

/*[clinic input]
nsmodule.Stream.write
    data: object

@doc[Stream.write]
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_write_impl(Stream *self, PyObject *data)
/*[clinic end generated code: output=164f7482511025e5 input=e8870d467dd04b04]*/
{
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError, "bytes expected");
        return NULL;
    }
    return (PyObject *) ns_stream_write(self, data);
}


Py_LOCAL_INLINE(void) stream_complete_connection(StreamHandle *handle);

static void
sslproto_feed_ssl_callback(uv_write_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _STATE_set(handle)
        if (status < 0) {
            stream_set_exception(handle, PyErr_UVError(PyExc_OSError, status));
        }
        Py_XDECREF(handle->wrapper);
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(void)
sslproto_feed_ssl(StreamHandle *handle, char *buffer, Py_ssize_t size)
{
    _Py_IDENTIFIER(write);
    _Py_IDENTIFIER(do_handshake);

    _STATE_set(handle)
    int err = 0;
    if (buffer) {
        PyObject *pybuffer = PyMemoryView_FromMemory(buffer, size, PyBUF_READ);
        if (!pybuffer) {
            return stream_set_exception(handle, NULL);
        } else {
            // should always success
            PyObject *ret = _PyObject_CallMethodIdOneArg(handle->inbio, &PyId_write, pybuffer);
            Py_DECREF(pybuffer);
            if (!ret) {
                return stream_set_exception(handle, NULL);
            } else {
                Py_DECREF(ret);
            }
        }
    }
    switch (handle->ssl_state) {
        case SSL_DO_HANDSHAKE: {
            PyObject *ret = _PyObject_CallMethodIdNoArgs(handle->sslobj, &PyId_do_handshake);
            if (!ret) {
                err = -1;
                goto finally;
            }
            stream_complete_connection(handle);
            handle->ssl_state = SSL_WRAPPED;
            Py_DECREF(ret);
            break;
        }
        case SSL_WRAPPED: {
            break;
        }
        case SSL_SHUTDOWN: {
            break;
        }
    }

finally:
    if (err && !sslproto_is_transient_error(_state)) {
        return stream_set_exception(handle, NULL);
    }
    _Py_IDENTIFIER(read);
    PyObject *ssldata = _PyObject_CallMethodIdNoArgs(handle->outbio, &PyId_read);
    if (!ssldata) {
        return stream_set_exception(handle, NULL);
    }
    if (PyBytes_GET_SIZE(ssldata)) {
        uv_write_t *req = Request_New(uv_write_t, ssldata);
        if (!req) {
            stream_set_exception(handle, NULL);
        } else {
            uv_buf_t buf = {.base = PyBytes_AS_STRING(ssldata), .len = PyBytes_GET_SIZE(ssldata)};
            UV_CALL(uv_write, req, &handle->base, &buf, 1, sslproto_feed_ssl_callback) {
                Request_Close(req);
                stream_set_exception(handle, PyErr_UVError(PyExc_OSError, error));
            } else {
                Py_XINCREF(handle->wrapper);
            }
        }
    }
    Py_DECREF(ssldata);
}

static void
stream_alloc_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    StreamHandle *stream_handle = Handle_Get(handle, StreamHandle);
    _STATE_set(stream_handle)
    buf->base = (char *) Freelist_Malloc(ReadBuffer);
    buf->len = sizeof(ReadBuffer);
}

Py_LOCAL_INLINE(void)
stream_feed_data(StreamHandle *handle, char *buffer, Py_ssize_t nread)
{
    if (nread) {
        if (streambuffer_push(&handle->buffer, buffer, nread)) {
            handle->status = UV_ENOMEM;
            uv_read_stop(&handle->base);
            stream_set_exception(handle, PyObject_CallNoArgs(PyExc_MemoryError));
        }
    }
//    LOG("stream_feed_default(%p): buffer=%p, nread=%ld", handle, buffer, nread);
//    STATE_set(handle);
//    PyObject *buffer_keeper = NULL;
//    if (handle->ssl) {
//        stream_feed_ssl(handle, &buffer, &nread, &buffer_keeper);
//        // read/write ssl
//        // set new buffer and nbytes readed
//    }
//    handle->in.nbytes += nread;
//    PyObject *promise = handle->promise;
//    if (promise) {
//        if (stream_feed_eof(handle)) {
//            stream_release(handle);
//        } else {
//            StreamReadArgs *args = Promise_DATA(handle->promise, StreamReadArgs);
//            args->feed_cb(handle, buffer, nread);
//            if (Promise_IsResolved(promise)) {
//                stream_release(handle);
//            }
//        }
//    } else {
//        stream_add_new_chunk(handle, buffer, nread, 0);
//        if (handle->in.nbytes >= handle->read_limit) {
//            stream_stop_reading(handle);
//        }
//    }
//    Py_XDECREF(buffer_keeper);
}

static void
stream_read_callback(uv_stream_t *stream, Py_ssize_t nread, const uv_buf_t *buf)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(stream, StreamHandle);
        _STATE_set(handle)
        char *buffer = buf->base;
        if (nread < 0) {
            LOG("stream_read_callback(%p, %ld): error", handle, nread);
            handle->status = (int) nread;
            buffer = NULL;
            nread = 0;
        }
        if (handle->sslproto) {
            sslproto_feed_ssl(handle, buffer, nread);
        } else {
            stream_feed_data(handle, buffer, nread);
        }
        if (!err) {
            stream_run_readop(handle);
        }
        Freelist_Free(ReadBuffer, buf->base);
    RELEASE_GIL
}

Py_LOCAL_INLINE(void)
stream_stop_reading(StreamHandle *handle)
{
    if (handle->status == 1) {
        LOG("stream_stop_reading(%p): stop reading", handle);
        handle->status = 0;
        uv_read_stop(&handle->base);
    }
}

Py_LOCAL_INLINE(void)
stream_start_reading(StreamHandle *handle)
{
    if (handle->status == 0) {
        LOG("stream_start_reading(%p): start reading", handle);
        handle->status = 1;
        uv_read_start(&handle->base, stream_alloc_callback, stream_read_callback);
    }
}

Py_LOCAL_INLINE(void)
sslproto_do_handshake(StreamHandle *handle)
{
    handle->ssl_state = SSL_DO_HANDSHAKE;
    sslproto_feed_ssl(handle, NULL, 0);
}

Py_LOCAL_INLINE(void)
stream_complete_connection(StreamHandle *handle)
{
    _STATE_set(handle)
    Stream *wrapper = (Stream *) Object_New((PyTypeObject *) S(StreamType));
    if (!wrapper) {
        Promise_RejectPyExc(handle->connect_promise);
    } else {
        wrapper->handle = handle;
        handle->wrapper = wrapper;
        Promise_Resolve(handle->connect_promise, (PyObject *) wrapper);
        Py_DECREF(wrapper);
    }
    Py_CLEAR(handle->connect_promise);
}

static void
stream_connect_callback(uv_connect_t *req, int status)
{
    ACQUIRE_GIL
        StreamHandle *handle = Handle_Get(req->handle, StreamHandle);
        _STATE_set(handle)
        if (status < 0) {
            Request_RejectUVError(req, PyExc_OSError, status);
            Handle_Close(handle);
            goto finally;
        }
        handle->connect_promise = Py_NewRef(Request_PROMISE(req));
        stream_start_reading(handle);
        if (handle->sslproto) {
            sslproto_do_handshake(handle);
        } else {
            stream_complete_connection(handle);
        }
    finally:
        Request_Close(req);
    RELEASE_GIL
}

Py_LOCAL_INLINE(Py_ssize_t)
stream_scan_block(char *data, Py_ssize_t data_size, char c)
{
    char *ptr = memchr(data, c, data_size);
    if (ptr) {
        LOG("stream_scan_block(%p): result size %ld", data, ptr - data + 1);
        return ptr - data + 1;
    }
    return 0;
}

//static Py_ssize_t
//stream_scan_buffer(StreamHandle *handle, char c, char *data, Py_ssize_t data_size)
//{
//    Py_ssize_t offset = handle->in.offset;
//    Py_ssize_t scan_size = 0;
//    InputChunk *it;
//    CHAIN_FOREACH(it, &handle->in) {
//        Py_ssize_t chunk_size = it->size - offset;
//        Py_ssize_t block_size = stream_scan_block(it->data + offset, chunk_size, c);
//        if (block_size) {
//            return scan_size + block_size;
//        }
//        scan_size += chunk_size;
//        offset = 0;
//    }
//    if (data_size) {
//        Py_ssize_t block_size = stream_scan_block(data, data_size, c);
//        if (block_size) {
//            return scan_size + block_size;
//        }
//    }
//    return 0;
//}

//Py_LOCAL_INLINE(void)
//stream_reject_exception(StreamHandle *handle)
//{
//    _STATE_set(handle)
//    if (handle->read_promise) {
//        Promise_Reject(handle->read_promise, handle->error);
//        Py_CLEAR(handle->read_promise);
//    }
//    if (handle->write_promise) {
//        Promise_Reject(handle->write_promise, handle->error);
//        Py_CLEAR(handle->write_promise);
//    }
//    if (!handle->wrapper) {
//        Handle_Close(handle);
//    }
//}

Py_LOCAL_INLINE(void)
stream_set_exception(StreamHandle *handle, PyObject *exc)
{
    _STATE_set(handle);
    if (!exc) {
        exc = fetch_current_exception();
    }
    Py_XSETREF(handle->error, exc);
    if (handle->connect_promise) {
        Promise_Reject(handle->connect_promise, exc);
    } else {
        // TODO:
    }
}

typedef int (*streamfeed_cb)(StreamHandle *handle, PyObject *promise);

typedef struct {
    streamfeed_cb cb;
    Py_ssize_t n;
    char c;
} streamreadargs;

//Py_LOCAL_INLINE(int)
//stream_feed_eof(StreamHandle *handle)
//{
//    STATE_set(handle);
//    if (!handle->in.nbytes && handle->status < 0) {
//        if (handle->status == UV_EOF) {
//            LOG("stream_feed_eof(%p): EOF", handle);
//            PyObject *result = PyBytes_FromStringAndSize(NULL, 0);
//            Promise_Resolve(handle->promise, result);
//            Py_DECREF(result);
//        } else {
//            LOG("stream_feed_eof(%p): error %d", handle, handle->s.status);
//            Promise_RejectUvErr(handle->promise, PyExc_OSError, handle->status);
//        }
//        return 1;
//    }
//    return 0;
//}


static void
stream_feed_n(StreamHandle *handle, PyObject *promise)
{
    _STATE_set(handle);
    if (handle->buffer.nbytes) {
        streamreadargs *args = Promise_DATA(promise, streamreadargs);
        stream_feed_buffer(handle, args->n);
        return 1;
    } else {
        stream_start_reading(handle);
        return 0;
    }
}

//static void
//stream_feed_exactly(StreamHandle *handle, char *data, Py_ssize_t data_size)
//{
//    LOG("stream_feed_exactly(%p): data=%p, data_size=%ld, buffer_size=%ld", handle, data, data_size, handle->s.nbytes);
//
//    STATE_set(handle);
//    StreamReadArgs *args = Promise_DATA(handle->promise, StreamReadArgs);
//
//    if (handle->status < 0 && handle->in.nbytes < args->n) {
//        PyObject *result = stream_feed_buffer_and_pull_data(handle, args->n, data, data_size);
//        if (!result) {
//            Promise_RejectPyExc(handle->promise);
//        } else {
//            PyObject *exc = new_exception(_state->IncompleteReadError, "end of stream");
//            PyObject_SetAttrString(exc, "partial", result);
//            Promise_Reject(handle->promise, exc);
//            Py_DECREF(exc);
//            Py_DECREF(result);
//        }
//        return;
//    }
//
//    if (handle->in.nbytes >= args->n) {
//        stream_feed_buffer_and_resolve_promise(handle, args->n, data, data_size);
//    } else {
//        stream_add_new_chunk(handle, data, data_size, 0);
//        stream_start_reading(handle);
//    }
//}

//static void
//stream_feed_until(StreamHandle *handle, char *data, Py_ssize_t data_size)
//{
//    LOG("stream_feed_until(%p): data=%p, data_size=%ld, buffer_size=%ld", handle, data, data_size, handle->s.nbytes);
//
//    STATE_set(handle);
//    Py_ssize_t read_until_size = 0;
//    StreamReadArgs *args = Promise_DATA(handle->promise, StreamReadArgs);
//    if (!args->n) {
//        read_until_size = stream_scan_buffer(handle, args->c, data, data_size);
//        args->n = 1;
//    } else if (data_size) {
//        read_until_size = stream_scan_block(data, data_size, args->c);
//        if (read_until_size)
//            read_until_size += handle->in.nbytes - data_size;
//    }
//
//    if (handle->status < 0 && !read_until_size) {
//        PyObject *result = stream_feed_buffer_and_pull_data(handle, 0, data, data_size);
//        if (!result) {
//            Promise_RejectPyExc(handle->promise);
//        } else {
//            PyObject *exc = new_exception(_state->IncompleteReadError, "end of stream");
//            PyObject_SetAttrString(exc, "partial", result);
//            Promise_Reject(handle->promise, exc);
//            Py_DECREF(exc);
//            Py_DECREF(result);
//        }
//        return;
//    }
//
//    if (read_until_size) {
//        stream_feed_buffer_and_resolve_promise(handle, read_until_size, data, data_size);
//    } else {
//        stream_add_new_chunk(handle, data, data_size, 0);
//        if (handle->in.nbytes >= handle->read_limit && handle->read_hard_limit) {
//            Promise_RejectString(handle->promise, state->LimitOverrunError,
//                                 "separator is not found, and buffer exceed the limit");
//            stream_stop_reading(handle);
//        } else {
//            stream_start_reading(handle);
//        }
//    }
//}

Py_LOCAL_INLINE(void)
stream_feed(StreamHandle *handle)
{
    _STATE_set(handle);
    Promise *it;
    while ((it = Chain_HEAD(&handle->read_backlog))) {
        PyObject *promise = (PyObject *) it;
        Promise *next = Chain_NEXT(it);
        streamreadargs *args = Promise_DATA(promise, streamreadargs);
        if (args->cb(handle, promise)) {
            Py_DECREF(promise);
            Chain_HEAD(&handle->read_backlog) = next;
        } else {
            stream_start_reading(handle);
            break;
        }
    }
}

Py_LOCAL_INLINE(PyObject *)
stream_read(StreamHandle *handle, streamfeed_cb cb, Py_ssize_t n, char c)
{
    _STATE_set(handle);
    PyObject *promise = Promise_New();
    if (!promise)
        return NULL;
    *Promise_DATA(promise, streamreadargs) = (streamreadargs) {cb, n, c};
    int can_feed = Chain_HEAD(&handle->read_backlog) == NULL;
    Py_INCREF(handle->wrapper); // keep stream object alive
    Py_INCREF(promise);
    Chain_APPEND(&handle->read_backlog, (Promise *) promise);
    if (can_feed) {
        stream_feed(handle);
    }
    return promise;
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_read(Stream *stream, Py_ssize_t n)
{
    if (check_stream_alive(stream)) {
        return NULL;
    }
    if (n == 0) {
        PyErr_SetUVError(PyExc_OSError, UV_EINVAL);
        return NULL;
    }
    return stream_read(stream->handle, stream_feed_n, n, 0);
}

/*[clinic input]
nsmodule.Stream.read
    n: ssize_t = -1
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_read_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=67f310d3fd0ce2c4 input=82d70be72d0dad10]*/
{
    return (PyObject *) ns_stream_read(self, n);
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_readexactly(Stream *stream, Py_ssize_t n)
{
    if (check_stream_alive(stream)) {
        return NULL;
    }
    if (n <= 0) {
        PyErr_SetUVError(PyExc_OSError, UV_EINVAL);
        return NULL;
    }
    if (stream->handle->read_hard_limit && n > stream->handle->read_limit) {
        PyErr_SetString(PyExc_ValueError, "Can't read more bytes than the buffer limit");
        return NULL;
    }
    return stream_read(stream, stream_feed_exactly, n, 0);
}

/*[clinic input]
nsmodule.Stream.readexactly
    n: ssize_t
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_readexactly_impl(Stream *self, Py_ssize_t n)
/*[clinic end generated code: output=81a83586675535cc input=9a7e64bb204b3f52]*/
{
    return (PyObject *) ns_stream_readexactly(self, n);
    Py_RETURN_NONE;
}

Py_LOCAL_INLINE(PyObject *)
ns_stream_readuntil(Stream *stream, char c)
{
    if (check_stream_alive(stream)) {
        return NULL;
    }
    return stream_read(stream, stream_feed_until, 0, c);
    Py_RETURN_NONE;
}

/*[clinic input]
nsmodule.Stream.readuntil
    c: char

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_readuntil_impl(Stream *self, char c)
/*[clinic end generated code: output=1ae79d7be62b8b65 input=33648528e6a9566a]*/
{
    return (PyObject *) ns_stream_readuntil(self, c);
}

Py_LOCAL_INLINE(int)
ns_stream_get_error(Stream *stream)
{
    if (ns_stream_closed(stream)) {
        return UV_EBADF;
    }
    switch (stream->handle->status) {
        case UV_EOF:
        case 0:
        case 1:
            return 0;
        default:
            return stream->handle->status;
    }
}

/*[clinic input]
nsmodule.Stream.get_error -> int

[clinic start generated code]*/

Py_LOCAL_INLINE(int)
nsmodule_Stream_get_error_impl(Stream *self)
/*[clinic end generated code: output=49ffcf5ea91c5b80 input=ff5900c88fdf44b4]*/
{
    return ns_stream_get_error(self);
}

Py_LOCAL_INLINE(int)
ns_stream_at_eof(Stream *stream)
{
    if (ns_stream_closed(stream)) {
        return 1;
    }
    return stream->handle->status == UV_EOF;
}

/*[clinic input]
nsmodule.Stream.at_eof

@doc[Stream.at_eof]
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_at_eof_impl(Stream *self)
/*[clinic end generated code: output=5194fdf70fc0bf12 input=9a8c4082e9840c9d]*/
{
    if (check_stream_alive(self)) {
        return NULL;
    }
    if (ns_stream_at_eof(self)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static void
stream_dealloc(Stream *self)
{
    if (self->handle) {
        self->handle->wrapper = NULL;
        Handle_Close(self->handle);
    }
    Object_Del(self);
}

static PyObject *
stream_repr(Stream *self)
{
    const char *stream_type = "UNBOUND";
    if (self->handle) {
        stream_type = uv_handle_type_name(self->handle->base.type);
    }
    if (!stream_type) {
        stream_type = "UNKNOWN";
    }
    return PyUnicode_FromFormat("<handle object at %p, type %s>", self, stream_type);
}

PyDoc_STRVAR(stream_doc,
             "Stream provides a duplex communication channel.\n");

static PyMethodDef stream_methods[] = {
    NSMODULE_STREAM_GETSOCKNAME_METHODDEF
    NSMODULE_STREAM_GETPEERNAME_METHODDEF
    NSMODULE_STREAM_WRITE_METHODDEF
    NSMODULE_STREAM_READ_METHODDEF
    NSMODULE_STREAM_READEXACTLY_METHODDEF
    NSMODULE_STREAM_READUNTIL_METHODDEF
    NSMODULE_STREAM_GET_ERROR_METHODDEF
    NSMODULE_STREAM_AT_EOF_METHODDEF
    {NULL}
};

static PyType_Slot stream_slots[] = {
    {Py_tp_doc, (char *) stream_doc},
    {Py_tp_methods, stream_methods},
    {Py_tp_repr, stream_repr},
    {Py_tp_dealloc, stream_dealloc},
    {0, 0},
};

static PyType_Spec stream_spec = {
    "promisedio.ns.Stream",
    sizeof(Stream),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    stream_slots
};

Py_LOCAL_INLINE(int)
sslproto_init_stream(StreamHandle *handle, StreamConfig *config, int server_socket)
{
    _STATE_set(handle)
    PyObject *inbio = NULL, *outbio = NULL;
    inbio = PyObject_CallNoArgs(S(MemoryBIO));
    if (!inbio)
        goto fail;
    outbio = PyObject_CallNoArgs(S(MemoryBIO));
    if (!outbio)
        goto fail;
    _Py_IDENTIFIER(wrap_bio);
    PyObject *ssl = _PyObject_CallMethodId(config->sslcontext, &PyId_wrap_bio, "OOis", inbio, outbio, server_socket,
                                           config->server_hostname);
    if (!ssl)
        goto fail;
    handle->sslobj = ssl;
    handle->inbio = inbio;
    handle->outbio = outbio;
    return 0;

fail:
    Py_XDECREF(inbio);
    Py_XDECREF(outbio);
    return -1;
}

Py_LOCAL_INLINE(int)
stream_handle_init(StreamHandle *handle, StreamConfig *config, int server_socket)
{
    Py_ssize_t read_limit = (config->buffer_limit <= 0) ? 65536 : config->buffer_limit;
    handle->wrapper = NULL;
    handle->connect_promise = NULL;
    handle->read_limit = read_limit;
    handle->read_trigger = (read_limit > 256) ? read_limit / 3 * 2 : read_limit;
    handle->sslproto = 0;
    Chain_INIT(&handle->read_backlog);
    Chain_INIT(&handle->write_backlog);
    streambuffer_init(&handle->buffer, config->chunk_min_size);
    if (config->sslcontext) {
        if (sslproto_init_stream(handle, config, server_socket)) {
            return -1;
        }
        handle->sslproto = 1;
    }
    return 0;
}

static void
stream_handle_finalizer(StreamHandle *handle)
{
    streambuffer_clear(&handle->buffer);
    Stream *wrapper = handle->wrapper;
    if (wrapper) {
        wrapper->handle = NULL;
        handle->wrapper = NULL;
    }
    Py_XDECREF(handle->connect_promise);
    Py_XDECREF(handle->error);
    // read_backlog and write_backlog should be empty here, but let's play it safe
    Promise *it;
    Chain_PULL(it, &handle->write_backlog) {
        Py_DECREF(it);
    }
    Chain_PULL(it, &handle->read_backlog) {
        Py_DECREF(it);
    }
    if (handle->sslproto) {
        Py_XDECREF(handle->sslobj);
        Py_XDECREF(handle->inbio);
        Py_XDECREF(handle->outbio);
    }
}

PyObject *
ns_tcp_connect(_STATE_var, const struct sockaddr *addr, const TCPStreamConfig *config)
{
    Request_SETUP(uv_connect_t, req, promise);
    if (!req) {
        return NULL;
    }

    TCPHandle *handle = Handle_New(TCPHandle, stream_handle_finalizer);
    if (!handle) {
    cleanup:
        Py_DECREF(promise);
        Request_Close(req);
        return NULL;
    }

    stream_handle_init((StreamHandle *) handle, (StreamConfig *) config, 0);
    uv_tcp_init(S(loop), &handle->base);
    uv_tcp_nodelay(&handle->base, config->tcp_nodelay > 0);
    uv_tcp_keepalive(&handle->base, config->tcp_keepalive > 0, config->tcp_keepalive);
    UV_CALL(uv_tcp_connect, req, &handle->base, addr, stream_connect_callback) {
        Handle_Close(handle);
        PyErr_SetUVError(PyExc_OSError, error);
        goto cleanup;
    }

    return promise;
}

static int
nsmodule_init(PyObject *module)
{
    LOG("nsmodule_init(%p)", module);

    _STATE_setmodule(module)

    Capsule_LOAD("promisedio.promise", PROMISE_API_ID);
    Capsule_LOAD("promisedio.loop", LOOP_API_ID);
    S(loop) = Loop_Get();
    return 0;
}

static int
nsmodule_init_types(PyObject *module)
{
    LOG("nsmodule_init_types(%p)", module);
    _STATE_setmodule(module)

    S(GAIError) = PyErr_NewException("promisedio.ns.GAIError", PyExc_OSError, NULL);
    if (!S(GAIError))
        return -1;
    S(LimitOverrunError) = PyErr_NewException("promisedio.ns.LimitOverrunError", PyExc_Exception, NULL);
    if (!S(LimitOverrunError))
        return -1;
    S(IncompleteReadError) = PyErr_NewException("promisedio.ns.IncompleteReadError", PyExc_EOFError, NULL);
    if (!S(IncompleteReadError))
        return -1;
    S(StreamType) = PyType_FromModuleAndSpec(module, &stream_spec, NULL);
    if (S(StreamType) == NULL)
        return -1;

    PyObject *d = PyModule_GetDict(module);
    if (PyDict_SetItemString(d, "GAIError", S(GAIError)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "LimitOverrunError", S(LimitOverrunError)) < 0)
        return -1;
    if (PyDict_SetItemString(d, "IncompleteReadError", S(IncompleteReadError)) < 0)
        return -1;

    Freelist_TYPE_INIT(ReadBuffer, 1);

    // SSL
    int ret = -1;
    PyObject *ssl = PyImport_ImportModule("_ssl");
    if (!ssl)
        goto finally;
    S(MemoryBIO) = PyObject_GetAttrString(ssl, "MemoryBIO");
    if (!S(MemoryBIO))
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

static int
nsmodule_create_api(PyObject *module)
{
    LOG("nsmodule_create_api(%p)", module);
    static void *c_api[] = {
    };
    Capsule_CREATE(module, NS_API_ID, c_api);
    return 0;
}

static int
nsmodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _STATE_setmodule(module)
    Capsule_TRAVERSE(LOOP_API_ID);
    Capsule_TRAVERSE(PROMISE_API_ID);
    Py_VISIT(S(GAIError));
    Py_VISIT(S(LimitOverrunError));
    Py_VISIT(S(IncompleteReadError));
    Py_VISIT(S(StreamType));
    // SSL
    Py_VISIT(S(MemoryBIO));
    Py_VISIT(S(SSLWantReadError));
    Py_VISIT(S(SSLWantWriteError));
    return 0;
}

static int
nsmodule_clear(PyObject *module)
{
    _STATE_setmodule(module)
    Capsule_CLEAR(LOOP_API_ID);
    Capsule_CLEAR(PROMISE_API_ID);
    Py_CLEAR(S(GAIError));
    Py_CLEAR(S(LimitOverrunError));
    Py_CLEAR(S(IncompleteReadError));
    Py_CLEAR(S(StreamType));
    // SSL
    Py_CLEAR(S(MemoryBIO));
    Py_CLEAR(S(SSLWantReadError));
    Py_CLEAR(S(SSLWantWriteError));
    return 0;
}

static void
nsmodule_free(void *module)
{
    LOG("nsmodule_free(%p)", module);
    _STATE_setmodule(module)
    nsmodule_clear(module);
    Freelist_Type_Clear(ReadBuffer);
}

static PyMethodDef nsmodule_methods[] = {
    NSMODULE_GETADDRINFO_METHODDEF
    NSMODULE_GETNAMEINFO_METHODDEF
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