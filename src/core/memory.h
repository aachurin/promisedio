// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_MEMORY_H
#define PROMISEDIO_CORE_MEMORY_H

#include "base.h"

#ifdef DEBUG_MEM

Py_LOCAL_INLINE(void *)
Mem_Malloc(size_t size) {
    void *ptr = PyMem_Malloc(size);
    LOG("PyMem_Malloc(%zu) -> %p", size, ptr);
    return ptr;
}

Py_LOCAL_INLINE(void)
Mem_Free(void *ptr)
{
    LOG("PyMem_Free(%p)", ptr);
    PyMem_Free(ptr);
}

Py_LOCAL_INLINE(PyObject *)
Object_New(PyTypeObject *tp)
{
    PyObject *obj = _PyObject_New(tp);
    LOG("PyObject_New(%s, %zu) -> %p", tp->tp_name, _PyObject_SIZE(tp), obj);
    return obj;
}

#define Object_Del Mem_Free

Py_LOCAL_INLINE(PyObject *)
Object_GC_New(PyTypeObject *tp)
{
    PyObject *obj = _PyObject_GC_New(tp);
    LOG("PyObject_GC_New(%s, %zu) -> %p", tp->tp_name, _PyObject_SIZE(tp), obj);
    return obj;
}

Py_LOCAL_INLINE(void)
Object_GC_Del(PyObject *obj)
{
    LOG("PyObject_GC_Del(%p)", obj);
    PyObject_GC_Del(obj);
}

#else

#define Mem_Malloc PyMem_Malloc
#define Mem_Free PyMem_Free
#define Object_New _PyObject_New
#define Object_Del PyMem_Free
#define Object_GC_New _PyObject_GC_New
#define Object_GC_Del PyObject_GC_Del

#endif
#ifndef WITHOUT_FREELISTS

typedef void (*freelistdealloc)(void *ptr);

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
    freelistdealloc dealloc;
    PyTypeObject *type;
} GCFreelist;

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
    freelistdealloc dealloc;
    PyTypeObject *type;
} ObjFreelist;

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
    freelistdealloc dealloc;
    size_t obj_size;
} TypeFreelist;

#define Freelist_GC_HEAD(name) GCFreelist name##__gc_fl;
#define Freelist_OBJ_HEAD(name) ObjFreelist name##__obj_fl;
#define Freelist_TYPE_HEAD(name) TypeFreelist name##__tp_fl;

#define Freelist_GC_INIT(name, maxsize)     \
    _state->name##__gc_fl = (GCFreelist) {  \
        .ptr=NULL,                          \
        .size=0,                            \
        .limit=(maxsize),                   \
        .type=_state->name,                 \
        .dealloc=PyObject_GC_Del            \
    }

#define Freelist_OBJ_INIT(name, maxsize)        \
    _state->name##__obj_fl = (ObjFreelist) {    \
        .ptr=NULL,                              \
        .size=0,                                \
        .limit=(maxsize),                       \
        .type=_state->name,                     \
        .dealloc=PyObject_Del                   \
    }

#define Freelist_TYPE_INIT(name, maxsize)  \
    _state->name##__tp_fl = (TypeFreelist) {        \
        .size=0,                                    \
        .ptr=NULL,                                  \
        .limit=(maxsize),                           \
        .dealloc=PyMem_Free,                        \
        .obj_size=sizeof(name)                      \
    }

Py_LOCAL_INLINE(void)
clear_freelist(ObjFreelist *freelist)
{
    void *next, *ptr = freelist->ptr;
    while (ptr) {
        next = *((void **) ptr);
        freelist->dealloc(ptr);
        ptr = next;
    }
    freelist->size = 0;
}

#define Freelist_GC_Clear(name) clear_freelist((ObjFreelist *) &(_state->name##__gc_fl))
#define Freelist_Obj_Clear(name) clear_freelist(&(_state->name##__obj_fl))
#define Freelist_Type_Clear(name) clear_freelist((ObjFreelist *) &(_state->name##__tp_fl))

Py_LOCAL_INLINE(void *)
freelist_pop(ObjFreelist *freelist)
{
    void *ptr = freelist->ptr;
    if (ptr) {
        freelist->ptr = *((void **) ptr);
        --freelist->size;
    }
    return ptr;
}

Py_LOCAL_INLINE(void)
freelist_push(ObjFreelist *freelist, void *ptr)
{
    if (freelist->size >= freelist->limit) {
        freelist->dealloc(ptr);
    } else {
        *((void **) ptr) = freelist->ptr;
        freelist->ptr = ptr;
        ++freelist->size;
    }
}

Py_LOCAL_INLINE(void *)
Freelist_Malloc(TypeFreelist *freelist)
{
    void *ptr = freelist_pop((ObjFreelist *) freelist);
    if (!ptr) {
        ptr = PyMem_Malloc(freelist->obj_size);
    }
    LOG("Freelist_Malloc(%zu) -> %p", freelist->obj_size, ptr);
    return ptr;
}

#define Freelist_Malloc(name) (Freelist_Malloc(&(_state->name##__tp_fl)))

Py_LOCAL_INLINE(void)
Freelist_Free(TypeFreelist *freelist, void *ptr)
{
    LOG("Freelist_Free(%p)", ptr);
    freelist_push((ObjFreelist *) freelist, ptr);
}

#define Freelist_Free(name, ptr) Freelist_Free(&(_state->name##__tp_fl), ptr)

Py_LOCAL_INLINE(PyObject *)
Freelist_New(ObjFreelist *freelist)
{
    void *ptr = freelist_pop(freelist);
    if (ptr) {
        PyObject_INIT(ptr, freelist->type);
    } else {
        ptr = _PyObject_New(freelist->type);
    }
    LOG("Freelist_New(%s, %zu) -> %p", freelist->type->tp_name, _PyObject_SIZE(freelist->type), ptr);
    return (PyObject *) ptr;
}

#define Freelist_New(name) (Freelist_New(&(_state->name##__obj_fl)))

Py_LOCAL_INLINE(void)
Freelist_Del(ObjFreelist *freelist, PyObject *ob)
{
    LOG("Freelist_Del(%s(%p))", ob->ob_type->tp_name, ob);
    freelist_push(freelist, ob);
}

#define Freelist_Del(name, obj) Freelist_Del(&(_state->name##__tp_fl), (PyObject *) (obj))

Py_LOCAL_INLINE(PyObject *)
Freelist_GC_New(GCFreelist *freelist)
{
    void *ptr = freelist_pop((ObjFreelist *) freelist);
    if (ptr) {
        PyObject_INIT(ptr, freelist->type);
    } else {
        ptr = _PyObject_GC_New(freelist->type);
    }
    LOG("Freelist_GC_New(%s, %zu) -> %p", freelist->type->tp_name, _PyObject_SIZE(freelist->type), ptr);
    return (PyObject *) ptr;
}

#define Freelist_GC_New(name) (Freelist_GC_New(&(_state->name##__gc_fl)))

Py_LOCAL_INLINE(void)
Freelist_GC_Del(GCFreelist *freelist, PyObject *ob)
{
    LOG("Freelist_GC_Del(%s(%p))", ob->ob_type->tp_name, ob);
    freelist_push((ObjFreelist *) freelist, ob);
}

#define Freelist_GC_Del(name, obj) Freelist_GC_Del(&(_state->name##__gc_fl), (PyObject *) (obj))

#define Freelist_GC_SetLimit(name, value) (&(_state->name##__gc_fl))->limit = (value)
#define Freelist_Obj_SetLimit(name, value) (&(_state->name##__obj_fl))->limit = (value)
#define Freelist_Type_SetLimit(name, value) (&(_state->name##__tp_fl))->limit = (value)

#else

#define TOUCH(x) (void)(x)

#define Freelist_GC_HEAD(name) PyTypeObject* name##__gc_fl;
#define Freelist_OBJ_HEAD(name) PyTypeObject* name##__obj_fl;
#define Freelist_TYPE_HEAD(name) size_t name##__tp_fl;

#define Freelist_GC_INIT(name, maxsize) _state->name##__gc_fl = _state->name
#define Freelist_OBJ_INIT(name, maxsize) _state->name##__obj_fl = _state->name
#define Freelist_TYPE_INIT(name, maxsize) _state->name##__tp_fl = sizeof(name)

#define Freelist_Malloc(name) (Mem_Malloc(_state->name##__tp_fl))
#define Freelist_Free(name, ptr) { Mem_Free(ptr); TOUCH(_state); }
#define Freelist_New(name) Object_New(state->name##__obj_fl)
#define Freelist_Del(name, obj) { Object_Del(obj); TOUCH(_state); }
#define Freelist_GC_New(name) Object_GC_New(_state->name##__gc_fl)
#define Freelist_GC_Del(name, obj) { Object_GC_Del((PyObject *) (obj)); TOUCH(_state); }

#define Freelist_GC_Clear(name) TOUCH(_state)
#define Freelist_Obj_Clear(name) TOUCH(_state)
#define Freelist_Type_Clear(name) TOUCH(_state)

#define Freelist_GC_SetLimit(name, value) TOUCH(_state)
#define Freelist_Obj_SetLimit(name, value) TOUCH(_state)
#define Freelist_Type_SetLimit(name, value) TOUCH(_state)

#endif

#endif
