// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef MEMORY_H
#define MEMORY_H

#include <Python.h>
#include <uv.h>
#include "common.h"
#include "promise.h"

enum {
    UV_HANDLE_FREE = 0x80000000
};

typedef void (*freelistdealloc)(void *ptr);
typedef struct freelist_s freelist_t;
typedef struct freelist_s {
    freelist_t *next;
    void *ptr;
    char* name;
    int chained;
    Py_ssize_t size;
    Py_ssize_t limit;
    Py_ssize_t minsize1;
    Py_ssize_t minsize2;
    freelistdealloc dealloc;
} freelist_t;

#define Mem_Alloc(size) \
    _Mem_Alloc(size)

#define Mem_Free(ptr) \
    _Mem_Free(ptr)

#define Mem_New(type, typeobj) \
    ((type *) _Mem_New(typeobj))

#define Mem_Del(ob) \
    _Mem_Free(ob)

#define Mem_GC_New(type, typeobj) \
    ((type *) _Mem_GC_New(&type##__gc_freelist, typeobj))

#define Mem_GC_Del(type, obj) \
    _Mem_GC_Del(&type##__gc_freelist, (PyObject *) (obj))

#define Mem_GC_FreeList(type) \
    static freelist_t type##__gc_freelist = {.dealloc=PyObject_GC_Del, .name=#type}

#define Request_New(promise, type) \
    (type *) _Request_New(promise, sizeof(type))

#define Request_Close(req) \
    _Request_Close((uv_req_t *) (req))

#define Request_Promise(ob) \
    ((Promise *)((uv_req_t *) (ob))->data)

#define Handle_New(type)  \
    (type *) _Handle_New(sizeof(type))

#define Handle_SetFreeOnClose(handle) \
    ((uv_handle_t *) (handle))->flags |= UV_HANDLE_FREE

#define Handle_Close(handle) \
    uv_close((uv_handle_t *) (handle), _Handle_OnClose)

#define Handle_Release(ptr)                          \
    if ((ptr) != NULL) {                             \
        uv_handle_t *handle = (uv_handle_t *) (ptr); \
        (ptr) = NULL;                                \
        _Handle_Release(handle);                     \
    }

void * _Mem_Alloc(size_t size);
void _Mem_Free(void *ptr);
PyObject * _Mem_New(PyTypeObject *tp);
PyObject * _Mem_GC_New(freelist_t *freelist, PyTypeObject *tp);
void _Mem_GC_Del(freelist_t *freelist, PyObject *ob);
uv_req_t * _Request_New(Promise *promise, size_t size);
void _Request_Close(uv_req_t *req);
uv_handle_t * _Handle_New(size_t size);
void _Handle_Release(uv_handle_t *handle);
void _Handle_OnClose(uv_handle_t *handle);
void Mem_ClearFreelists();
size_t Mem_AllocCount();
void Mem_DebugInfo();

#endif
