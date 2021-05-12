// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "memory.h"
#include "promise.h"

static freelist_t generic_freelists[128] = {[0 ... 127] = {.dealloc = PyMem_Free}};
static struct {
    freelist_t *root;
    size_t allocs;
} freelist_context = {0};

size_t
Mem_AllocCount()
{
    return freelist_context.allocs;
}

#define max(a,b) (((a)>(b))?(a):(b))

void
Mem_ClearFreelists()
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
_Mem_Alloc(size_t size) {
    size_t alloc_size = size + sizeof(void *);
    LOGC("_Mem_Alloc(%zu) -> ", alloc_size);
    assert (alloc_size <= 512);
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
_Mem_Free(void *ptr)
{
    LOG("Mem__Free(%p)", ptr);
    --freelist_context.allocs;
    ptr -= sizeof(void *);
    freelist_t *freelist = *(freelist_t **) ptr;
    freelist_push(freelist, ptr);
}

PyObject *
_Mem_New(PyTypeObject *tp)
{
    LOGC("_Mem_New(%s, %zu): ", tp->tp_name, _PyObject_SIZE(tp));
    PyObject *op = (PyObject *) Mem_Alloc(_PyObject_SIZE(tp));
    if (op == NULL) {
        return PyErr_NoMemory();
    }
    PyObject_INIT(op, tp);
    return op;
}

PyObject *
_Mem_GC_New(freelist_t *freelist, PyTypeObject *tp)
{
    LOGC("_Mem_GC_New(%s, %zu) -> ", tp->tp_name, _PyObject_SIZE(tp));
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
_Mem_GC_Del(freelist_t *freelist, PyObject *ptr)
{
    LOG("_Mem_GC_Del(%s(%p))", ptr->ob_type->tp_name, ptr);
    --freelist_context.allocs;
    freelist_push(freelist, ptr);
}

uv_req_t *
_Request_New(Promise *promise, size_t size)
{
    LOGC("_Request_New(%zu): ", size);
    uv_req_t *req = (uv_req_t *) Mem_Alloc(size);
    if (req == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    Py_XINCREF(promise);
    req->data = promise;
    return req;
}

void
_Request_Close(uv_req_t *req)
{
    LOGC("_Request_Close(%p, %d): ", req, req->type);
    Py_XDECREF(req->data);
    Mem_Free(req);
}

uv_handle_t *
_Handle_New(size_t size)
{
    LOGC("_Handle_New(%zu): ", size);
    uv_handle_t *handle = (uv_handle_t *) Mem_Alloc(size);
    if (handle == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    return handle;
}

#define try_free_handle(handle)                                 \
    if ((handle)->flags & UV_HANDLE_FREE) {                     \
        LOG("try_free_handle(%p) -> ok", handle);               \
        Mem_Free(handle);                                       \
    } else {                                                    \
        LOG("try_free_handle(%p) -> wait", handle);             \
        Handle_SetFreeOnClose(handle);                          \
    }

void
_Handle_Release(uv_handle_t *handle) {
    int closing = uv_is_closing(handle);
    try_free_handle(handle);
    if (!closing) {
        Handle_Close(handle);
    }
}

void
_Handle_OnClose(uv_handle_t *handle)
{
    ACQUIRE_GIL
    try_free_handle(handle);
    RELEASE_GIL
}

void
Mem_DebugInfo()
{
    fprintf(stderr, "%-25s%10s%10s%10s\n", "Type", "size", "limit", "minsize");
    for (int i = 0; i < 64; ++i) {
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
