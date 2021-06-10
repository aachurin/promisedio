// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef MEMORY_H
#define MEMORY_H

#include <Python.h>
#include <uv.h>
#include "common.h"
#include "promise.h"

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

#define Mem_ALLOC(size) mem__alloc(size)
#define Mem_FREE(ptr) mem__free(ptr)
#define Mem_NEW(type, typeobj) ((type *) mem__new(typeobj))
#define Mem_DEL(ob) mem__free(ob)
#define Mem_GC_NEW(type, typeobj) ((type *) mem__gc_new(&type##__gc_freelist, typeobj))
#define Mem_GC_DEL(type, obj) mem__gc_del(&type##__gc_freelist, (PyObject *) (obj))
#define Mem_GC_FREELIST(type) static freelist_t type##__gc_freelist = {.dealloc=PyObject_GC_Del, .name=#type}
#define Request_NEW(promise, type) (type *) request__new(promise, sizeof(type))
#define Request_CLOSE(req) request__close((uv_req_t *) (req))
#define Request_PROMISE(ob) ((Promise *)((uv_req_t *) (ob))->data)
#define Handle_NEW(type, cb) (type *) handle__new(sizeof(type), (finalizer) (cb))
#define Handle_CLOSE(handle)                         \
    do {                                             \
        uv_handle_t *tmp = (uv_handle_t *) (handle); \
        if (tmp && !uv_is_closing(tmp)) {            \
            if (tmp->data) {                         \
                ((finalizer) (tmp->data))(tmp);      \
            }                                        \
            uv_close(tmp, handle__on_close);         \
        }                                            \
    } while (0)

typedef void (*finalizer)(uv_handle_t *handle);

void * mem__alloc(size_t size);
void mem__free(void *ptr);
PyObject * mem__new(PyTypeObject *tp);
PyObject * mem__gc_new(freelist_t *freelist, PyTypeObject *tp);
void mem__gc_del(freelist_t *freelist, PyObject *ob);
uv_req_t * request__new(Promise *promise, size_t size);
void request__close(uv_req_t *req);
uv_handle_t * handle__new(size_t size, finalizer cb);
void handle__on_close(uv_handle_t *handle);
void mem_clear_freelists();
size_t mem_alloc_count();
void mem_debug_info();

#endif
