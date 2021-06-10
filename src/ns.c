// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <sys/socket.h>
#include <sys/un.h>
#include "ns.h"
#include "memory.h"
#include "loop.h"
#include "utils.h"

static PyObject *gaierror;

static PyObject *
make_ipv4_addr(const struct sockaddr_in *addr)
{
    char buf[INET_ADDRSTRLEN] = {0};
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
    char buf[INET6_ADDRSTRLEN] = {0};
    int err = uv_ip6_name(addr, buf, sizeof(buf));
    if (err) {
        set_uv_error(PyExc_OSError, err);
        return NULL;
    }
    return PyUnicode_FromString(buf);
}
#endif

static PyObject *
makesockaddr(struct sockaddr *addr, size_t addrlen)
{
    if (addrlen == 0) {
        Py_RETURN_NONE;
    }
    switch (addr->sa_family) {
        case AF_INET:
        {
            const struct sockaddr_in *a = (const struct sockaddr_in *)addr;
            PyObject *addrobj = make_ipv4_addr(a);
            PyObject *ret = NULL;
            if (addrobj) {
                ret = Py_BuildValue("Oi", addrobj, ntohs(a->sin_port));
                Py_DECREF(addrobj);
            }
            return ret;
        }

#if defined(AF_UNIX)
        case AF_UNIX:
        {
            struct sockaddr_un *a = (struct sockaddr_un *) addr;
#ifdef __linux__
            size_t linuxaddrlen = addrlen - offsetof(struct sockaddr_un, sun_path);
            if (linuxaddrlen > 0 && a->sun_path[0] == 0) {  /* Linux abstract namespace */
                return PyBytes_FromStringAndSize(a->sun_path, linuxaddrlen);
            }
            else
#endif
            {
                /* regular NULL-terminated string */
                return PyUnicode_DecodeFSDefault(a->sun_path);
            }
        }
#endif

#ifdef ENABLE_IPV6
        case AF_INET6:
        {
            const struct sockaddr_in6 *a = (const struct sockaddr_in6 *)addr;
            PyObject *addrobj = make_ipv6_addr(a);
            PyObject *ret = NULL;
            if (addrobj) {
                ret = Py_BuildValue("OiII",
                                    addrobj,
                                    ntohs(a->sin6_port),
                                    ntohl(a->sin6_flowinfo),
                                    a->sin6_scope_id);
                Py_DECREF(addrobj);
            }
            return ret;
        }
#endif

        default:
            /* If we don't know the address family, don't raise an
               exception -- return it as an (int, bytes) tuple. */
            return Py_BuildValue("iy#",
                                 addr->sa_family,
                                 addr->sa_data,
                                 sizeof(addr->sa_data));

    }
}

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
        PyObject *addr = makesockaddr(next->ai_addr, next->ai_addrlen);
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
    Request_CLOSE(req);
    RELEASE_GIL
}

Promise *
ns_getaddrinfo(const char *node, const char *service, int family, int type, int proto, int flags)
{
    Promise *promise = promise_new();
    if (!promise)
        return NULL;

    uv_getaddrinfo_t *req = Request_NEW(promise, uv_getaddrinfo_t);
    if (!req) {
        Py_DECREF(promise);
        return NULL;
    }

    struct addrinfo hints;
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;
    hints.ai_flags = flags;

    BEGIN_UV_CALL(uv_getaddrinfo, req, getaddrinfo_callback, node, service, &hints)
        set_uv_error(gaierror, error);
        Py_DECREF(promise);
        return NULL;
    END_UV_CALL

    return promise;
}

static PyObject*
decode_hostname(const char *name)
{
#ifdef MS_WINDOWS
    return PyUnicode_DecodeMBCS(name, strlen(name), "surrogatepass");
#else
    /* Decode from UTF-8 */
    return PyUnicode_FromString(name);
#endif
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

    PyObject *name = decode_hostname(hostname);
    if (!name) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    PyObject *ret = Py_BuildValue("Ns", name, service);
    if (!ret) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    promise_resolve(promise, ret);
    Py_DECREF(ret);

    finally:
    Request_CLOSE(req);
    RELEASE_GIL
}

typedef struct {
    unsigned int flowinfo;
    unsigned int scope_id;
    int flags;
} getnameinfo_state_t;

static void
getnameinfo_getaddrinfo_callback(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    ACQUIRE_GIL
    Promise *promise = Request_PROMISE(req);
    if (status < 0) {
        promise_reject_uv_error(promise, gaierror, status);
        goto finally;
    }

    if (res->ai_next) {
        promise_reject_string(promise, gaierror,
                              "sockaddr resolved to multiple addresses");
        goto finally;
    }

    uv_getnameinfo_t *nreq = Request_NEW(promise, uv_getnameinfo_t);
    if (!nreq) {
        promise_reject_py_exc(promise);
        goto finally;
    }

    getnameinfo_state_t *state = Promise_DATA(promise, getnameinfo_state_t);
#ifdef ENABLE_IPV6
    if (res->ai_family == AF_INET6) {
        struct sockaddr_in6 *sin6;
        sin6 = (struct sockaddr_in6 *) res->ai_addr;
        sin6->sin6_flowinfo = htonl(state->flowinfo);
        sin6->sin6_scope_id = state->scope_id;
    }
#endif

    BEGIN_UV_CALL(uv_getnameinfo, nreq, getnameinfo_callback, res->ai_addr, state->flags)
        promise_reject_uv_error(promise, gaierror, error);
    END_UV_CALL

    finally:
    uv_freeaddrinfo(res);
    Request_CLOSE(req);
    RELEASE_GIL
}

Promise *
ns_getnameinfo(const char *node, const char *service, unsigned int flowinfo, unsigned int scope_id, int flags)
{
    Promise *promise = promise_new();
    if (!promise)
        return NULL;

    uv_getaddrinfo_t *req = Request_NEW(promise, uv_getaddrinfo_t);
    if (!req) {
        Py_DECREF(promise);
        return NULL;
    }

    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_NUMERICHOST;
    getnameinfo_state_t *state = Promise_DATA(promise, getnameinfo_state_t);
    state->flowinfo = flowinfo;
    state->scope_id = scope_id;
    state->flags = flags;

    BEGIN_UV_CALL(uv_getaddrinfo, req, getnameinfo_getaddrinfo_callback, node, service, &hints)
        set_uv_error(gaierror, error);
        Py_DECREF(promise);
        return NULL;
    END_UV_CALL

    return promise;
}

int
ns_module_init(PyObject *module)
{
    gaierror = PyErr_NewException("promisedio.gaierror", PyExc_OSError,NULL);
    if (gaierror == NULL)
        return -1;
    Py_INCREF(gaierror);
    PyModule_AddObject(module, "gaierror", gaierror);
    return 0;
}
