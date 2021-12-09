// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_ATOMIC_H
#define PROMISEDIO_CORE_ATOMIC_H

#include "core/base.h"
#include "internal/pycore_atomic.h"

// currently, only stdlib
#if defined(HAVE_STD_ATOMIC)
#define _Py_atomic_compare_exchange(ATOMIC_VAL, EXPECTED, NEW_VAL) \
    atomic_compare_exchange_strong(&((ATOMIC_VAL)->_value), &EXPECTED, NEW_VAL)
#endif

typedef _Py_atomic_int _Py_atomic_lock;

Py_LOCAL_INLINE(int)
atomic_try_lock(_Py_atomic_lock *lock)
{
    int expected_value = 0;
    return _Py_atomic_compare_exchange(lock, expected_value, 1);
}

Py_LOCAL_INLINE(void)
atomic_lock(_Py_atomic_lock *lock)
{
    while(1) {
        if (atomic_try_lock(lock)) {
            break;
        }
    }
}

Py_LOCAL_INLINE(void)
atomic_unlock(_Py_atomic_lock *lock)
{
    _Py_atomic_store(lock, 0);
}

#endif
