// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "memory.h"
#include "promise.h"

static freelist_t generic_freelists[192] = {[0 ... 191] = {.dealloc = PyMem_Free}};
static struct {
    freelist_t *root;
    size_t allocs;
} freelist_context = {0};

size_t
mem_alloc_count()
{
    return freelist_context.allocs;
}

#define max(a,b) (((a)>(b))?(a):(b))

void
mem_clear_freelists()
{
    freelist_t *it = freelist_context.root;
    while (it) {
        Py_ssize_t threshold = max(it->limit >> 3, 64);
        if (it->minsize1 > threshold) {
            if (it->minsize1 <= it->limit) {
                it->limit -= it->minsize1 - (threshold >> 1);
            } else if (it->minsize1 >= it->minsize2) {
                while (it->size > it->limit) {
                    void *ptr = it->ptr;
                    it->ptr = *((void **) ptr);
                    --it->size;
                }
            }
        }
        it->minsize2 = it->minsize1;
        it->minsize1 = it->size;
        it = it->next;
    }
}

static inline void *
freelist_pop(freelist_t *freelist)
{
    void *ptr = freelist->ptr;
    if (ptr) {
        freelist->ptr = *((void **) ptr);
        if ((--freelist->size) < freelist->minsize1) {
            freelist->minsize1 = freelist->size;
        }
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
        freelist->size++;
    }
}

static inline void
freelist_after_alloc(freelist_t *freelist)
{
    ++freelist->limit;
    if (!freelist->chained) {
        freelist->chained = 1;
        freelist->next = freelist_context.root;
        freelist_context.root = freelist;
    }
}

void *
mem__alloc(size_t size) {
    size_t alloc_size = size + sizeof(void *);
    LOGC("mem__alloc(%zu) -> ", alloc_size);
    assert (alloc_size <= 1536);
    size_t index = (alloc_size >> 3) - 1;
    freelist_t *freelist = &generic_freelists[index];
    void *ptr = freelist_pop(freelist);
    if (ptr == NULL) {
        ptr = PyMem_Malloc(((alloc_size + 7) >> 3) << 3); // use pymalloc
        if (ptr == NULL)
            return NULL;
        freelist_after_alloc(freelist);
    }
    ++freelist_context.allocs;
    *(void **) ptr = freelist;
    ptr += sizeof(void *);
    LOG("%p", ptr);
    return ptr;
}

void
mem__free(void *ptr)
{
    LOG("mem__free(%p)", ptr);
    --freelist_context.allocs;
    ptr -= sizeof(void *);
    freelist_t *freelist = *(freelist_t **) ptr;
    freelist_push(freelist, ptr);
}

PyObject *
mem__new(PyTypeObject *tp)
{
    LOGC("mem__new(%s, %zu): ", tp->tp_name, _PyObject_SIZE(tp));
    PyObject *op = (PyObject *) Mem_ALLOC(_PyObject_SIZE(tp));
    if (op == NULL) {
        return PyErr_NoMemory();
    }
    PyObject_INIT(op, tp);
    return op;
}

PyObject *
mem__gc_new(freelist_t *freelist, PyTypeObject *tp)
{
    LOGC("mem__gc_new(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
    void *ptr = freelist_pop(freelist);
    if (ptr != NULL) {
        PyObject_INIT(ptr, tp);
    } else {
        ptr = _PyObject_GC_New(tp);
        freelist_after_alloc(freelist);
    }
    if (ptr != NULL) {
        ++freelist_context.allocs;
    }
    LOG("%p", ptr);
    return (PyObject *) ptr;
}

void
mem__gc_del(freelist_t *freelist, PyObject *ob)
{
    LOG("mem__gc_del(%s(%p))", ob->ob_type->tp_name, ob);
    --freelist_context.allocs;
    freelist_push(freelist, ob);
}

uv_req_t *
request__new(Promise *promise, size_t size)
{
    LOGC("request__new(%zu): ", size);
    uv_req_t *req = (uv_req_t *) Mem_ALLOC(size);
    if (req == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    Py_XINCREF(promise);
    req->data = promise;
    return req;
}

void
request__close(uv_req_t *req)
{
    LOGC("request__close(%p, %d): ", req, req->type);
    Py_XDECREF(req->data);
    Mem_FREE(req);
}

uv_handle_t *
handle__new(size_t size, finalizer cb)
{
    LOGC("handle__new(%zu): ", size);
    uv_handle_t *handle = (uv_handle_t *) Mem_ALLOC(size);
    if (handle == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    handle->data = cb;
    return handle;
}

void
handle__on_close(uv_handle_t *handle)
{
    ACQUIRE_GIL
    Mem_FREE(handle);
    RELEASE_GIL
}

void
mem_debug_info()
{
    fprintf(stderr, "%-25s%10s%10s%10s\n", "Type", "size", "limit", "minsize");
    for (int i = 0; i < 192; ++i) {
        int index = (i + 1) << 3;
        if (generic_freelists[i].chained) {
            fprintf(stderr, "%-25d%10zd%10zd%10zd\n",
                   index, generic_freelists[i].size, generic_freelists[i].limit, generic_freelists[i].minsize1);
        }
    }
    freelist_t *it = freelist_context.root;
    while (it) {
        if (it->name) {
            fprintf(stderr, "%-25s%10zd%10zd%10zd\n", it->name, it->size, it->limit, it->minsize1);
        }
        it = it->next;
    }
}
