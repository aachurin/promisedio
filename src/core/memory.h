// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_MEMORY_H
#define PROMISEDIO_CORE_MEMORY_H

#include "base.h"

#ifdef BUILD_DEBUG_MEM
Py_LOCAL_INLINE(void *)
Py_Malloc(size_t n)
{
    void *ptr = PyMem_Malloc(n);
    MEMLOG("MALLOC", ptr, "RAW");
    return ptr;
}

Py_LOCAL_INLINE(void)
Py_Free(void *op)
{
    MEMLOG("FREE", op, "RAW");
    PyMem_Free(op);
}

Py_LOCAL_INLINE(PyObject *)
Py_New(PyTypeObject *tp)
{
    PyObject *ptr = _PyObject_New(tp);
    MEMLOG("MALLOC", ptr, Py_TYPE(ptr)->tp_name);
    return ptr;
}

Py_LOCAL_INLINE(PyObject *)
Py_GC_New(PyTypeObject *tp)
{
    PyObject *ptr = _PyObject_GC_New(tp);
    MEMLOG("MALLOC", ptr, Py_TYPE(ptr)->tp_name);
    return ptr;
}

#else
#define Py_Malloc(op) PyMem_Malloc(op)
#define Py_Free(op) PyMem_Free(op)
#define Py_New(op) _PyObject_New(op)
#define Py_GC_New(op) _PyObject_GC_New(op)
#endif

Py_LOCAL_INLINE(void)
Py_Delete(void *op)
{
    // For heap types only
    MEMLOG("FREE", op, Py_TYPE(op)->tp_name);
    PyTypeObject *tp = Py_TYPE(op);
    PyMem_Free(op);
    Py_DECREF(tp);
}

Py_LOCAL_INLINE(void)
Py_GC_Delete(void *op)
{
    // For heap types only
    MEMLOG("FREE", op, Py_TYPE(op)->tp_name);
    PyTypeObject *tp = Py_TYPE(op);
    PyObject_GC_Del(op);
    Py_DECREF(tp);
}

#ifdef BUILD_DEBUG_MEM

#define PyTrack_DECREF(op)                                  \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("DECREF", _tmp, Py_TYPE(_tmp)->tp_name);         \
    Py_DECREF(_tmp);                                        \
} while (0)

#define PyTrack_XDECREF(op)                                 \
do {                                                        \
    PyObject *_tmp1 = _PyObject_CAST(op);                   \
    if (_tmp1 != NULL) {                                    \
        PyTrack_DECREF(_tmp1);                              \
    }                                                       \
} while (0)

#define PyTrack_INCREF(op)                                  \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("INCREF", _tmp, Py_TYPE(_tmp)->tp_name);         \
    Py_INCREF(_tmp);                                        \
} while (0)

#define PyTrack_XINCREF(op)                                 \
do {                                                        \
    PyObject *_tmp1 = _PyObject_CAST(op);                   \
    if (_tmp1 != NULL) {                                    \
        PyTrack_INCREF(_tmp1);                              \
    }                                                       \
} while (0)

#define PyTrack_CLEAR(op)                                   \
do {                                                        \
    PyObject *_tmp1 = _PyObject_CAST(op);                   \
    if (_tmp1 != NULL) {                                    \
        (op) = NULL;                                        \
        PyTrack_DECREF(_tmp1);                              \
    }                                                       \
} while (0)

#define PyTrack_SETREF(op, op2)                             \
do {                                                        \
    PyObject *_tmp1 = _PyObject_CAST(op);                   \
    (op) = (op2);                                           \
    PyTrack_DECREF(_tmp1);                                  \
} while (0)

#define PyTrack_XSETREF(op, op2)                            \
do {                                                        \
    PyObject *_tmp2 = _PyObject_CAST(op);                   \
    (op) = (op2);                                           \
    PyTrack_XDECREF(_tmp2);                                 \
} while (0)

#define PyTrack_MarkAllocated(op)                           \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    if (_tmp) {                                             \
        MEMLOG("ALLOC", _tmp, Py_TYPE(_tmp)->tp_name);      \
    }                                                       \
} while (0)

#define PyTrack_MarkDeleted(op)                             \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("DELETE", _tmp, Py_TYPE(_tmp)->tp_name);         \
} while (0)

#define PyTrack_MarkEnter(op)                               \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("ENTER", _tmp, Py_TYPE(_tmp)->tp_name);          \
} while (0)

#define PyTrack_MarkBeginResize(op)                         \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("RESIZE", _tmp, Py_TYPE(_tmp)->tp_name);         \
} while (0)

#define PyTrack_MarkEndResize(op)                           \
do {                                                        \
    PyObject *_tmp = _PyObject_CAST(op);                    \
    MEMLOG("RESIZD", _tmp,  Py_TYPE(_tmp)->tp_name);        \
} while (0)

#else

#define PyTrack_DECREF(op) Py_DECREF(op)
#define PyTrack_XDECREF(op) Py_XDECREF(op)
#define PyTrack_INCREF(op) Py_INCREF(op)
#define PyTrack_XINCREF(op) Py_XINCREF(op)
#define PyTrack_CLEAR(op) Py_CLEAR(op)
#define PyTrack_SETREF(op, op2) Py_SETREF(op, op2)
#define PyTrack_XSETREF(op, op2) Py_XSETREF(op, op2)
#define PyTrack_MarkAllocated(op)
#define PyTrack_MarkDeleted(op)
#define PyTrack_MarkEnter(op)
#define PyTrack_MarkBeginResize(op)
#define PyTrack_MarkEndResize(op)
#endif

#ifndef BUILD_DISABLE_FREELISTS

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
} freelist_gc_head;

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
} freelist_obj_head;

typedef struct {
    void *ptr;
    Py_ssize_t size;
    Py_ssize_t limit;
    size_t obj_size;
} freelist_mem_head;

#define Freelist_GC_HEAD(name) freelist_gc_head name##__gc_freelist;
#define Freelist_OBJ_HEAD(name) freelist_obj_head name##__obj_freelist;
#define Freelist_MEM_HEAD(name) freelist_mem_head name##__mem_freelist;

#define Freelist_MEM_INIT(name, maxsize) \
_ctx->name##__mem_freelist =             \
    (freelist_mem_head) {                \
    .size=0,                             \
    .ptr=NULL,                           \
    .limit=(maxsize),                    \
    .obj_size=sizeof(name)               \
}

#define Freelist_OBJ_INIT(name, maxsize) \
_ctx->name##__obj_freelist =             \
    (freelist_obj_head) {                \
    .ptr=NULL,                           \
    .size=0,                             \
    .limit=(maxsize)                     \
}

#define Freelist_GC_INIT(name, maxsize) \
_ctx->name##__gc_freelist =             \
    (freelist_gc_head) {                \
    .ptr=NULL,                          \
    .size=0,                            \
    .limit=(maxsize)                    \
}

#define Freelist_CLEAR(fl, dealloc)     \
    void *next, *ptr = (fl)->ptr;       \
    while (ptr) {                       \
        next = *((void **) ptr);        \
        dealloc(ptr);                   \
        ptr = next;                     \
    }                                   \
    (fl)->size = 0;

Py_LOCAL_INLINE(void)
Freelist_Mem_Clear(freelist_mem_head *fl)
{
    Freelist_CLEAR(fl, PyMem_Free)
}

Py_LOCAL_INLINE(void)
Freelist_Obj_Clear(freelist_obj_head *fl)
{
    Freelist_CLEAR(fl, PyMem_Free)
}

Py_LOCAL_INLINE(void)
Freelist_GC_Clear(freelist_gc_head *fl)
{
    Freelist_CLEAR(fl, PyObject_GC_Del);
}

#define Freelist_Mem_Clear(name) Freelist_Mem_Clear(&(_ctx->name##__mem_freelist))
#define Freelist_Obj_Clear(name) Freelist_Obj_Clear(&(_ctx->name##__obj_freelist))
#define Freelist_GC_Clear(name) Freelist_GC_Clear(&(_ctx->name##__gc_freelist))

Py_LOCAL_INLINE(void *)
freelist__pop(freelist_mem_head *fl)
{
    void *ptr = fl->ptr;
    if (ptr) {
        fl->ptr = *((void **) ptr);
        --fl->size;
    }
    return ptr;
}

Py_LOCAL_INLINE(void)
freelist__push(freelist_mem_head *fl, void *ptr)
{
    *((void **) ptr) = fl->ptr;
    fl->ptr = ptr;
    ++fl->size;
}

Py_LOCAL_INLINE(void *)
Freelist_Malloc(freelist_mem_head *fl)
{
    void *ptr = freelist__pop(fl);
    if (!ptr) {
        ptr = PyMem_Malloc(fl->obj_size);
    }
    return ptr;
}

#define Freelist_Malloc(name) Freelist_Malloc(&_ctx->name##__mem_freelist)

Py_LOCAL_INLINE(void)
Freelist_Free(freelist_mem_head *fl, void *ptr)
{
    if (fl->size >= fl->limit) {
        Py_Free(ptr);
    } else {
        freelist__push(fl, ptr);
    }
}

#define Freelist_Free(name, ptr) Freelist_Free(&_ctx->name##__mem_freelist, ptr)

Py_LOCAL_INLINE(PyObject *)
Freelist_New(freelist_obj_head *fl, PyTypeObject *tp)
{
    void *ptr = freelist__pop((freelist_mem_head *) fl);
    if (ptr) {
        PyObject_Init(ptr, tp);
    } else {
        ptr = Py_New(tp);
    }
    return (PyObject *) ptr;
}

#define Freelist_New(name) Freelist_New(&_ctx->name##__obj_freelist)

Py_LOCAL_INLINE(void)
Freelist_Delete(freelist_obj_head *fl, PyObject *obj)
{
    if (fl->size >= fl->limit) {
        Py_Delete(obj);
    } else {
        freelist__push((freelist_mem_head *) fl, obj);
        Py_DECREF(Py_TYPE(obj));
    }
}

#define Freelist_Delete(name, ob) Freelist_Delete(&_ctx->name##__obj_freelist, _PyObject_CAST(ob))

Py_LOCAL_INLINE(PyObject *)
Freelist_GC_New(freelist_gc_head *fl, PyTypeObject *tp)
{
    void *ptr = freelist__pop((freelist_mem_head *) fl);
    if (ptr) {
        PyObject_Init(ptr, tp);
    } else {
        ptr = Py_GC_New(tp);
    }
    return (PyObject *) ptr;
}

#define Freelist_GC_New(name) Freelist_GC_New(&_ctx->name##__gc_freelist, _ctx->name)

Py_LOCAL_INLINE(void)
Freelist_GC_Delete(freelist_gc_head *fl, PyObject *obj)
{
    if (fl->size >= fl->limit) {
        Py_GC_Delete(obj);
    } else {
        freelist__push((freelist_mem_head *) fl, obj);
        Py_DECREF(Py_TYPE(obj));
    }
}

#define Freelist_GC_Delete(name, ob) Freelist_GC_Delete(&_ctx->name##__gc_freelist, _PyObject_CAST(ob))

#define Freelist_GC_Limit(name, value) (&(_ctx->name##__gc_freelist))->limit = (value)
#define Freelist_Obj_Limit(name, value) (&(_ctx->name##__obj_freelist))->limit = (value)
#define Freelist_Mem_Limit(name, value) (&(_ctx->name##__mem_freelist))->limit = (value)

#else

#define TOUCH(x) (void)(x)

#define Freelist_MEM_HEAD(name) size_t name##__mem_freelist;
#define Freelist_OBJ_HEAD(name)
#define Freelist_GC_HEAD(name)

#define Freelist_MEM_INIT(name, maxsize) _ctx->name##__mem_freelist = sizeof(name)
#define Freelist_OBJ_INIT(name, maxsize) TOUCH(_ctx)
#define Freelist_GC_INIT(name, maxsize) TOUCH(_ctx)

#define Freelist_Malloc(name) Py_Malloc(_ctx->name##__mem_freelist)
#define Freelist_Free(name, ptr) Py_Free(ptr), TOUCH(_ctx)
#define Freelist_New(name) Py_New(state->name)
#define Freelist_Delete(name, obj) Py_Delete(obj), TOUCH(_ctx)
#define Freelist_GC_New(name) Py_GC_New(_ctx->name)
#define Freelist_GC_Delete(name, obj) Py_GC_Delete(obj), TOUCH(_ctx)

#define Freelist_GC_Clear(name) TOUCH(_ctx)
#define Freelist_Obj_Clear(name) TOUCH(_ctx)
#define Freelist_Mem_Clear(name) TOUCH(_ctx)

#define Freelist_GC_Limit(name, value) TOUCH(_ctx)
#define Freelist_Obj_Limit(name, value) TOUCH(_ctx)
#define Freelist_Mem_Limit(name, value) TOUCH(_ctx)

#endif

#endif
