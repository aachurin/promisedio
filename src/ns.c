// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "ns.h"
#include "memory.h"
#include "loop.h"
#include "utils.h"
#include "chain.h"
#include "python/socketmodule.h"

typedef struct stream_s Stream;

typedef struct chunk_s Chunk;
typedef struct chunk_s {
    Chain_NODE_PROTOCOL(Chunk)
    size_t size;
    char data[0];
} Chunk;

#define STREAM_HANDLE(base)     \
    Stream *stream;             \
    size_t limit;               \
    size_t nbytes;              \
    Py_off_t offset;            \
    Chain_PROTOCOL(Chunk)       \
    HANDLE_BASE(base)

typedef struct {
    STREAM_HANDLE(uv_stream_t)
} stream_handle_t;

typedef struct {
    STREAM_HANDLE(uv_tcp_t)
} tcp_handle_t;

typedef struct {
    STREAM_HANDLE(uv_udp_t)
} udp_handle_t;

typedef struct {
    STREAM_HANDLE(uv_pipe_t)
} pipe_handle_t;

typedef struct stream_s {
    PyObject_HEAD
    stream_handle_t *handle;
} Stream;

#include "clinic/ns.c.h"

/*[clinic input]
module ns
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=486bb829615864d0]*/

static PyObject *gaierror;

static PyObject *
make_ipv4_addr(const struct sockaddr_in *addr)
{
    char buf[INET_ADDRSTRLEN];
    int err = uv_ip4_name(addr, buf, sizeof(buf));
    if (err) {
        set_uv_error(PyExc_OSError, err);
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
        set_uv_error(PyExc_OSError, err);
        return NULL;
    }
    return PyUnicode_FromString(buf);
}
#endif

// Using Python socketmodule native code
#include "python/socketmodule.c"

static void
getaddrinfo_callback(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    ACQUIRE_GIL

    Promise *promise = Request_PROMISE(req);
    if (status < 0) {
        promise_reject_uv_error(promise, gaierror, status);
        goto finally;
    }

    PyObject *result = PyList_New(0);
    if (!result) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    struct addrinfo *next;
    for (next = res; next; next = next->ai_next) {
        // we use native python makesockaddr
        PyObject *addr = makesockaddr(-1, next->ai_addr, next->ai_addrlen, next->ai_protocol);
        if (!addr) {
            error:
            promise_reject_py_exc(promise);
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

    promise_resolve(promise, result);
    Py_DECREF(result);

    finally:
    uv_freeaddrinfo(res);
    Request_Close(req);

    RELEASE_GIL
}

Promise *
ns_getaddrinfo(const char *node, const char *service, int family, int type, int proto, int flags)
{
    INIT_DEFAULT_LOOP(loop)
    INIT_PROMISE_AND_REQUEST(uv_getaddrinfo_t, req, promise);

    struct addrinfo hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;
    hints.ai_flags = flags;

    UV_REQUEST(uv_getaddrinfo, loop, req, getaddrinfo_callback, node, service, &hints) {
        Request_Close(req);
        set_uv_error(gaierror, error);
        Py_DECREF(promise);
        return NULL;
    }

    return promise;
}

static void
getnameinfo_callback(uv_getnameinfo_t *req, int status, const char *hostname, const char *service)
{
    ACQUIRE_GIL

    Promise *promise = Request_PROMISE(req);
    if (status < 0) {
        promise_reject_uv_error(promise, gaierror, status);
        goto finally;
    }

    PyObject *name = sock_decode_hostname(hostname);
    if (!name) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    PyObject *ret = Py_BuildValue("Os", name, service);
    Py_DECREF(name);
    if (!ret) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    promise_resolve(promise, ret);
    Py_DECREF(ret);

    finally:
    Request_Close(req);

    RELEASE_GIL
}

Promise *
ns_getnameinfo(const struct sockaddr *addr, int flags)
{
    INIT_DEFAULT_LOOP(loop)
    INIT_PROMISE_AND_REQUEST(uv_getnameinfo_t, req, promise);

    UV_REQUEST(uv_getnameinfo, loop, req, getnameinfo_callback, addr, flags) {
        Request_Close(req);
        set_uv_error(gaierror, error);
        Py_DECREF(promise);
        return NULL;
    }

    return promise;
}
//
//static void
//stream_handle_finalizer(uv_stream_t *handle)
//{
//    stream_handle_t *stream_handle = container_of(handle, stream_handle_t, base);
//    if (stream_handle->stream) {
//        stream_handle->stream->handle = NULL;
//    }
//}
//
//static void
//stream_connect_callback(uv_connect_t *req, int status)
//{
////    ACQUIRE_GIL
////
////    Promise *promise = Request_PROMISE(req);
////    if (status < 0) {
////        promise_reject_uv_error(promise, status);
////        goto finally;
////    }
////
////    Stream *stream = stream_new();
////    if (!stream) {
////        promise_reject_py_exc(promise);
////        goto finally;
////    }
////
////    stream_handle_t *handle = container_of(req->handle, stream_handle_t, base);
////    stream->handle = handle;
////    handle->in_buffer = 0;
////    handle->out_buffer = 0;
////    handle->in_offset = 0;
////    handle->out_offset = 0;
////    handle->in_size = 0;
////    handle->out_size = 0;
////
////    finally:
////    Request_Close(req);
////    RELEASE_GIL
//}
//
//static inline void
//init_stream_handle(stream_handle_t *handle, Py_ssize_t buffer_limit)
//{
//    handle->stream = NULL;
//    handle->limit = buffer_limit < 0 ? 65536 : buffer_limit;
//    handle->nbytes = 0;
//    handle->offset = 0;
//}
//
//Promise *
//ns_tcp_connect(const struct sockaddr *addr, int tcp_nodelay, int tcp_keepalive, Py_ssize_t buffer_limit)
//{
//    INIT_DEFAULT_LOOP(loop)
//    INIT_PROMISE_AND_REQUEST(uv_connect_t, req, promise);
//
//    tcp_handle_t *handle = Handle_New(tcp_handle_t, stream_handle_finalizer);
//    if (!handle) {
//        cleanup:
//        Py_DECREF(promise);
//        Request_Close(req);
//        return NULL;
//    }
//    init_stream_handle((stream_handle_t *) handle, buffer_limit);
//    uv_tcp_init(loop, &handle->base);
//    if (tcp_nodelay) {
//        uv_tcp_nodelay(&handle->base, 1);
//    }
//    if (tcp_keepalive >= 0) {
//        uv_tcp_keepalive(&handle->base, 1, tcp_keepalive);
//    }
//    UV_REQUEST(uv_tcp_connect, req, &handle->base, addr, stream_connect_callback) {
//        Handle_Close(handle);
//        set_uv_error(PyExc_OSError, error);
//        goto cleanup;
//    }
//    return promise;
//}
//
//static int
//check_handle(Stream *self)
//{
//    if (!self->handle) {
////        PyErr_SetString()
//    }
//}
//
///*[clinic input]
//class ns.Stream "Stream *" "&StreamType"
//[clinic start generated code]*/
///*[clinic end generated code: output=da39a3ee5e6b4b0d input=b74efb85c489e03b]*/
//
///*[clinic input]
//ns.Stream.get_write_queue_size
//
//@doc[Stream.get_write_queue_size]
//[clinic start generated code]*/
//
//static inline PyObject *
//ns_Stream_get_write_queue_size_impl(Stream *self)
///*[clinic end generated code: output=f24d350b5fb3cc4c input=fb76ae54c22e64b7]*/
//{
//    if (check_handle(self)) {
//        return NULL;
//    }
//    return PyLong_FromSize_t(uv_stream_get_write_queue_size(&self->handle->base));
//}
//
//static int
//stream_traverse(Stream *self, visitproc visit, void *arg)
//{
////    Py_VISIT(self->next);
//    return 0;
//}
//
//static int
//stream_clear(Stream *self)
//{
////    Py_CLEAR(self->next);
//    return 0;
//}
//
//static void
//stream_dealloc(Stream *self)
//{
////    PyObject_GC_UnTrack(self);
//    stream_clear(self);
////    Freelist_GC_Del(Stream, self);
//}
//
//static PyMethodDef StreamType_methods[] = {
//    NS_STREAM_GET_WRITE_QUEUE_SIZE_METHODDEF
//    {NULL}
//};
//
//PyDoc_STRVAR(StreamType__doc__,
//    "Stream provides a duplex communication channel.\n");
//
//static PyTypeObject StreamType = {
//    PyVarObject_HEAD_INIT(NULL, 0)
//    .tp_name = "Stream",
//    .tp_doc = StreamType__doc__,
//    .tp_basicsize = sizeof(Stream),
//    .tp_itemsize = 0,
//    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
//    .tp_dealloc = (destructor) stream_dealloc,
//    .tp_traverse = (traverseproc) stream_traverse,
//    .tp_clear = (inquiry) stream_clear,
////    .tp_repr = stream_repr,
//    .tp_methods = StreamType_methods
//};

int
ns_module_init(PyObject *module)
{
    gaierror = PyErr_NewException("promisedio.gaierror", PyExc_OSError,NULL);
    if (gaierror == NULL)
        return -1;
    Py_INCREF(gaierror);
//    if (PyType_Ready(&StreamType) < 0)
//        return -1;
    PyModule_AddObject(module, "gaierror", gaierror);
    return 0;
}
