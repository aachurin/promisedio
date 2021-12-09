// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_HANDLE_H
#define PROMISEDIO_CORE_HANDLE_H

#include "base.h"
#include "memory.h"
#include "module.h"
#include "uvh.h"

#define container_of(ptr, type) ((type *) ((char *) (ptr) - offsetof(type, base)))
#define Handle_Get(ptr, type) ((type *)((ptr)->data))

typedef struct {
    void *_ctx;
    char base[0];
} Request;

Py_LOCAL_INLINE(uv_req_t *)
Request_New(void *_ctx, PyObject *promise, size_t size)
{
    Request *ptr = (Request *) Py_Malloc(size + sizeof(Request));
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    LOG("(%p, %zu) -> %p", promise, size, &ptr->base);
    ptr->_ctx = _ctx;
    uv_req_t *req = (uv_req_t *) &ptr->base;
    PyTrack_XINCREF(promise);
    req->data = promise;
    return req;
}

#define Request_New(type, promise) ((type *) Request_New(_ctx, (PyObject *) (promise), sizeof(type)))
#define Request_OBJ(req) ((PyObject *)((req)->data))
#define Request_PROMISE(req) ((Promise *)((req)->data))
#define _CTX_setreq(req) _CTX_setstored((Request *) container_of(req, Request));

Py_LOCAL_INLINE(void)
Request_Close(uv_req_t *req)
{
    LOG("(%p)", req);
    PyTrack_XDECREF(req->data);
    Py_Free(container_of(req, Request));
}

#define Request_Close(req) Request_Close((uv_req_t *) (req))

typedef void (*finalizer)(uv_handle_t *handle);

#define HANDLE_BASE(handle_type) \
    finalizer _finalizer;        \
    void *_ctx;                  \
    handle_type base;

typedef struct {
    HANDLE_BASE(uv_handle_t);
} HandleBase;

Py_LOCAL_INLINE(void *)
Handle_New(void *_ctx, size_t size, size_t base_offset, finalizer cb)
{
    void *ptr = Py_Malloc(size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    LOG("(%zu) -> %p", size, ptr);
    HandleBase *base = (HandleBase *) ((char *) ptr + base_offset);
    base->_finalizer = cb;
    base->_ctx = _ctx;
    base->base.data = ptr;
    return ptr;
}

#define Handle_New(type, cb) \
    (type *) Handle_New(_ctx, sizeof(type), offsetof(type, _finalizer), (finalizer) (cb))

#define Handle_Free(h) Py_Free(h)

static void
handle__on_close(uv_handle_t *handle)
{
    ACQUIRE_GIL
        HandleBase *base = container_of(handle, HandleBase);
        if (base->_finalizer) {
            base->_finalizer(handle->data);
        }
        Py_Free(handle->data);
    RELEASE_GIL
}

Py_LOCAL_INLINE(void)
Handle_Close_UV(uv_handle_t *handle)
{
    if (handle && !uv_is_closing(handle)) {
        LOG("(%p)", handle->data);
        BEGIN_ALLOW_THREADS
        uv_close(handle, handle__on_close);
        END_ALLOW_THREADS
    }
}

#define Handle_Close(h) Handle_Close_UV((uv_handle_t *) &((h)->base))

#define Request_SETUP(req_type, req, promise)   \
    req_type *(req) = NULL;                     \
    Promise *(promise) = Promise_New();         \
    if (promise) {                              \
        (req) = Request_New(req_type, promise); \
        if (!(req)) {                           \
            Py_DECREF(promise);                 \
            (promise) = NULL;                   \
        }                                       \
    }

#define Loop_SETUP(loop)                        \
    uv_loop_t *(loop) = Loop_Get();             \
    if (!loop) {                                \
        return NULL;                            \
    }

#endif //PROMISEDIO_CORE_HANDLE_H
