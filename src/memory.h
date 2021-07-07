// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"
#include "promise.h"

#ifdef DEBUG_MEM

void mem_debug_after_malloc(void *);
void mem_debug_before_free(void *);

static inline void *
Mem_Malloc(size_t size) {
    LOGC("PyMem_Malloc(%zu) -> ", size);
    void *ptr = PyMem_Malloc(size);
    mem_debug_after_malloc(ptr);
    LOG("%p", ptr);
    return ptr;
}

static inline void
Mem_Free(void *ptr)
{
    LOG("PyMem_Free(%p)", ptr);
    mem_debug_before_free(ptr);
    PyMem_Free(ptr);
}

static inline PyObject *
Object_New(PyTypeObject *tp)
{
    LOGC("PyObject_New(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
    PyObject *obj = _PyObject_New(tp);
    mem_debug_after_malloc(obj);
    LOG("%p", obj);
    return obj;
}

#define Object_New(type, typeobj) ((type *) Object_New(typeobj))
#define Object_Del Mem_Free

static inline PyObject *
Object_GC_New(PyTypeObject *tp)
{
    LOGC("PyObject_GC_New(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
    PyObject *obj = _PyObject_GC_New(tp);
    mem_debug_after_malloc(obj);
    LOG("%p", obj);
    return obj;
}

#define Object_GC_New(type, typeobj) ((type *) Object_GC_New(typeobj))

static inline void
Object_GC_Del(PyObject *obj)
{
    LOG("PyObject_GC_Del(%p)", obj);
    mem_debug_before_free(obj);
    PyObject_GC_Del(obj);
}

#else

#define Mem_Malloc PyMem_Malloc
#define Mem_Free PyMem_Free
#define Object_New PyObject_New
#define Object_Del PyMem_Free
#define Object_GC_New PyObject_GC_New
#define Object_GC_Del PyObject_GC_Del

#endif

#ifndef DISABLE_FREELISTS

typedef void (*freelistdealloc)(void *ptr);
typedef struct {
    void *ptr;
    char *name;
    size_t obj_size;
    Py_ssize_t size;
    Py_ssize_t limit;
    freelistdealloc dealloc;
} freelist_t;

#define CLEAR_FREELIST(type)                        \
    static void clear_##type##_freelist() {         \
        void *next, *ptr = type##__freelist.ptr;    \
        while (ptr) {                               \
            next = *((void **) ptr);                \
            type##__freelist.dealloc(ptr);          \
            ptr = next;                             \
        }                                           \
        type##__freelist.size = 0;                  \
    }

#define INIT_GC_FREELIST(type, maxsize)             \
    static freelist_t type##__freelist = {          \
        .limit=(maxsize),                           \
        .dealloc=PyObject_GC_Del                    \
    };                                              \
    CLEAR_FREELIST(type)

#define INIT_OBJ_FREELIST(type, maxsize)            \
    static freelist_t type##__freelist = {          \
        .limit=(maxsize),                           \
        .dealloc=PyObject_Del                       \
    };                                              \
    CLEAR_FREELIST(type)

#define INIT_STRUCT_FREELIST(type, maxsize)         \
    static freelist_t type##__freelist = {          \
        .name=#type,                                \
        .limit=(maxsize),                           \
        .obj_size=sizeof(type),                     \
        .dealloc=PyMem_Free                         \
    };                                              \
    CLEAR_FREELIST(type)

static inline void *
freelist_pop(freelist_t *freelist)
{
    void *ptr = freelist->ptr;
    if (ptr) {
        freelist->ptr = *((void **) ptr);
        --freelist->size;
    }
    return ptr;
}

static inline void
freelist_push(freelist_t *freelist, void *ptr)
{
    if (freelist->size >= freelist->limit) {
        freelist->dealloc(ptr);
    } else {
        *((void **) ptr) = freelist->ptr;
        freelist->ptr = ptr;
        ++freelist->size;
    }
}

static inline void *
Freelist_Malloc(freelist_t *freelist)
{
    LOGC("Freelist_Malloc(%s, %zu) -> ", freelist->name, freelist->obj_size);
    void *ptr = freelist_pop(freelist);
    if (!ptr) {
        ptr = PyMem_Malloc(freelist->obj_size);
    }
#ifdef DEBUG_MEM
    mem_debug_after_malloc(ptr);
#endif
    LOG("%p", ptr);
    return ptr;
}

#define Freelist_Malloc(type) ((type *) Freelist_Malloc(&type##__freelist))

static inline void
Freelist_Free(freelist_t *freelist, void *ptr)
{
    LOG("Freelist_Free(%s(%p))", freelist->name, ptr);
#ifdef DEBUG_MEM
    mem_debug_before_free(ptr);
#endif
    freelist_push(freelist, ptr);
}

#define Freelist_Free(type, ptr) Freelist_Free(&type##__freelist, ptr)

static inline PyObject *
Freelist_New(freelist_t *freelist, PyTypeObject *tp)
{
    LOGC("Freelist_New(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
    void *ptr = freelist_pop(freelist);
    if (ptr) {
        PyObject_INIT(ptr, tp);
    } else {
        ptr = _PyObject_New(tp);
    }
#ifdef DEBUG_MEM
    mem_debug_after_malloc(ptr);
#endif
    LOG("%p", ptr);
    return (PyObject *) ptr;
}

#define Freelist_New(type, typeobj) ((type *) Freelist_New(&type##__freelist, typeobj))

static inline void
Freelist_Del(freelist_t *freelist, PyObject *ob)
{
    LOG("Freelist_Del(%s(%p))", ob->ob_type->tp_name, ob);
#ifdef DEBUG_MEM
    mem_debug_before_free(ob);
#endif
    freelist_push(freelist, ob);
}

#define Freelist_Del(type, obj) Freelist_Del(&type##__freelist, (PyObject *) (obj))

static inline PyObject *
Freelist_GC_New(freelist_t *freelist, PyTypeObject *tp)
{
    LOGC("Freelist_GC_New(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
    void *ptr = freelist_pop(freelist);
    if (ptr) {
        PyObject_INIT(ptr, tp);
    } else {
        ptr = _PyObject_GC_New(tp);
    }
#ifdef DEBUG_MEM
    mem_debug_after_malloc(ptr);
#endif
    LOG("%p", ptr);
    return (PyObject *) ptr;
}

#define Freelist_GC_New(type, typeobj) ((type *) Freelist_GC_New(&type##__freelist, typeobj))

static inline void
Freelist_GC_Del(freelist_t *freelist, PyObject *ob)
{
    LOG("Freelist_GC_Del(%s(%p))", ob->ob_type->tp_name, ob);
#ifdef DEBUG_MEM
    mem_debug_before_free(ob);
#endif
    freelist_push(freelist, ob);
}

#define Freelist_GC_Del(type, obj) Freelist_GC_Del(&type##__freelist, (PyObject *) (obj))

#else

#define INIT_GC_FREELIST(type, maxsize)
#define INIT_OBJ_FREELIST(type, maxsize)
#define INIT_STRUCT_FREELIST(type, maxsize)

#define Freelist_Malloc(type) Mem_Malloc(sizeof(type))
#define Freelist_Free(type, ptr) Mem_Free(ptr)
#define Freelist_New Object_New
#define Freelist_Del(type, obj) Object_Del(obj)
#define Freelist_GC_New Object_GC_New
#define Freelist_GC_Del(type, obj) Object_GC_Del((PyObject *) obj)

#endif

static inline uv_req_t *
Request_New(Promise *promise, size_t size)
{
    LOGC("Request_New(%zu): ", size);
    uv_req_t *req = (uv_req_t *) Mem_Malloc(size);
    if (!req) {
        PyErr_NoMemory();
        return NULL;
    }
    req->data = XOWNED(promise);
    return req;
}

#define Request_New(type, promise) ((type *) Request_New(promise, sizeof(type)))

static inline void
Request_Close(uv_req_t *req)
{
    LOGC("Request_Close(%p): ", req);
    Py_XDECREF(req->data);
    Mem_Free(req);
}

#define Request_Close(req) Request_Close((uv_req_t *) (req))
#define Request_PROMISE(ob) ((Promise *)((uv_req_t *) (ob))->data)

#define handle_container(type, ptr) ((type *) ((ptr)->data))
#define container_of(ptr, type, member) ((type *) ((char *) (ptr) - offsetof(type, member)))

typedef void (*finalizer)(uv_handle_t *handle);

#define HANDLE_BASE(handle_type) \
    finalizer _finalizer;        \
    handle_type handle;

typedef struct {
    HANDLE_BASE(uv_handle_t);
} handle_base_t;

static inline void *
Handle_New(size_t size, size_t base_offset, finalizer cb)
{
    LOGC("Handle_New(%zu): ", size);
    void *ptr = Mem_Malloc(size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    handle_base_t *base = (handle_base_t *)((char *) ptr + base_offset);
    base->_finalizer = cb;
    base->handle.data = ptr;
    return ptr;
}

#define Handle_New(type, cb) \
    (type *) Handle_New(sizeof(type), offsetof(type, _finalizer), (finalizer) (cb))

void handle__on_close(uv_handle_t *handle);

static inline void
close_handle(uv_handle_t *handle)
{
    LOG("Handle_Close(%p)", handle->data);
    if (handle && !uv_is_closing(handle)) {
        handle_base_t *base = container_of(handle, handle_base_t, handle);
        if (base->_finalizer) {
            base->_finalizer(handle->data);
        }
        uv_close(handle, handle__on_close);
    }
}

#define Handle_Close(h) close_handle((uv_handle_t *) &((h)->handle))

Py_ssize_t get_mem_allocs();

#endif
