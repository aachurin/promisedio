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
    void *_state;
    char base[0];
} Request;

Py_LOCAL_INLINE(uv_req_t *)
Request_New(void *_state, PyObject *promise, size_t size)
{
    Request *ptr = (Request *) Mem_Malloc(size + sizeof(Request));
    LOG("Request_New(%zu) -> %p", size, &ptr->base);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    _STATE_save(ptr);
    uv_req_t *req = (uv_req_t *) &ptr->base;
    req->data = Py_XNewRef(promise);
    return req;
}

#define Request_New(type, promise) ((type *) Request_New(_state, (PyObject *) (promise), sizeof(type)))
#define Request_PROMISE(req) ((PyObject *)((req)->data))
#define Request_DATA(req, tp) Promise_DATA(Request_PROMISE(req), tp)
#define Request_GetCtx(req) Promise_GetCtx(Request_PROMISE(req))
#define Request_SetCtx(req, value) Promise_SetCtx(Request_PROMISE(req), value)

#define _STATE_setreq(req) _STATE_set((Request *) container_of(req, Request));

Py_LOCAL_INLINE(void)
Request_Close(uv_req_t *req)
{
    LOG("Request_Close(%p)", req);
    Py_XDECREF(req->data);
    Mem_Free(container_of(req, Request));
}

#define Request_Close(req) Request_Close((uv_req_t *) (req))

typedef void (*finalizer)(uv_handle_t *handle);

#define HANDLE_BASE(handle_type) \
    finalizer _finalizer;        \
    void *_state;                 \
    handle_type base;

typedef struct {
    HANDLE_BASE(uv_handle_t);
} HandleBase;

Py_LOCAL_INLINE(void *)
Handle_New(void *_state, size_t size, size_t base_offset, finalizer cb)
{
    void *ptr = Mem_Malloc(size);
    LOG("Handle_New(%zu) -> %p", size, ptr);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    HandleBase *base = (HandleBase *) ((char *) ptr + base_offset);
    base->_finalizer = cb;
    base->_state = _state;
    base->base.data = ptr;
    return ptr;
}

#define Handle_New(type, cb) \
    (type *) Handle_New(_state, sizeof(type), offsetof(type, _finalizer), (finalizer) (cb))

Py_LOCAL_INLINE(void)
handle__on_close(uv_handle_t *handle)
{
    ACQUIRE_GIL
        Mem_Free(handle->data);
    RELEASE_GIL
}

Py_LOCAL_INLINE(void)
Handle_Close_UV(uv_handle_t *handle)
{
    LOG("Handle_Close(%p)", handle->data);
    if (handle && !uv_is_closing(handle)) {
        HandleBase *base = container_of(handle, HandleBase);
        if (base->_finalizer) {
            base->_finalizer(handle->data);
        }
        uv_close(handle, handle__on_close);
    }
}

#define Handle_Close(h) Handle_Close_UV((uv_handle_t *) &((h)->base))

#define Request_SETUP(req_type, req, promise)   \
    req_type *(req) = NULL;                     \
    PyObject *(promise) = Promise_New();        \
    if (promise) {                              \
        (req) = Request_New(req_type, promise); \
        if (!(req)) {                           \
            Py_DECREF(promise);                 \
            (promise) = NULL;                   \
        }                                       \
    }

#define Request_Resolve(req, value) Promise_Resolve(Request_PROMISE(req), value)
#define Request_RejectPyExc(req) Promise_RejectPyExc(Request_PROMISE(req))
#define Request_RejectUVError(req, exc, uverr) Promise_RejectUVError(Request_PROMISE(req), exc, uverr)
#define Request_RejectString(req, exc, msg) Promise_RejectString(Request_PROMISE(req), exc, msg)

#endif //PROMISEDIO_CORE_HANDLE_H
