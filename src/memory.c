// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "memory.h"

static Py_ssize_t mem_allocs;

Py_ssize_t
get_mem_allocs()
{
    return mem_allocs;
}

#ifdef DEBUG_MEM

void
mem_debug_after_malloc(void *ptr)
{
    if (ptr) {
        mem_allocs++;
    }
}

void
mem_debug_before_free(void *ptr)
{
    if (ptr) {
        mem_allocs--;
    }
}

#endif

void
handle__on_close(uv_handle_t *handle)
{
    ACQUIRE_GIL
    Mem_Free(handle);
    RELEASE_GIL
}
