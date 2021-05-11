// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef COMMON_H
#define COMMON_H

#include <Python.h>

//#define DEBUG_OUTPUT stderr

#include <Python.h>

#if PY_VERSION_HEX < 0x03090000
#define PyObject_CallOneArg(a, b) PyObject_CallFunctionObjArgs(a, b, NULL)
#define _PyObject_CallMethodIdOneArg(a, b, c) _PyObject_CallMethodIdObjArgs(a, b, c, NULL)
#endif

#ifdef DEBUG_OUTPUT
    #define LOG(...) LOGC(__VA_ARGS__); fprintf(DEBUG_OUTPUT, "\n")
    #define LOGC(...) fprintf(DEBUG_OUTPUT, __VA_ARGS__)
#else
    #define LOGC(...)
    #define LOG(...)
#endif

#define ACQUIRE_GIL \
    { PyGILState_STATE _gstate = PyGILState_Ensure();
#define RELEASE_GIL \
    PyGILState_Release(_gstate); }
#define BEGIN_ALLOW_THREADS \
    Py_BEGIN_ALLOW_THREADS
#define END_ALLOW_THREADS \
    Py_END_ALLOW_THREADS

#ifdef MS_WINDOWS
/* Windows uses long long for offsets */
typedef long long Py_off_t;
#define PyLong_AsOff_t     PyLong_AsLongLong
#define PyLong_FromOff_t   PyLong_FromLongLong
#define PY_OFF_T_MAX       LLONG_MAX
#define PY_OFF_T_MIN       LLONG_MIN
#define PY_OFF_T_COMPAT    long long
#define PY_PRIdOFF         "lld"
#else
/* Other platforms use off_t */
typedef off_t Py_off_t;
#if (SIZEOF_OFF_T == SIZEOF_SIZE_T)
#define PyLong_AsOff_t     PyLong_AsSsize_t
#define PyLong_FromOff_t   PyLong_FromSsize_t
#define PY_OFF_T_MAX       PY_SSIZE_T_MAX
#define PY_OFF_T_MIN       PY_SSIZE_T_MIN
#define PY_OFF_T_COMPAT    Py_ssize_t
#define PY_PRIdOFF         "zd"
#elif (SIZEOF_OFF_T == SIZEOF_LONG_LONG)
#define PyLong_AsOff_t     PyLong_AsLongLong
#define PyLong_FromOff_t   PyLong_FromLongLong
#define PY_OFF_T_MAX       LLONG_MAX
#define PY_OFF_T_MIN       LLONG_MIN
#define PY_OFF_T_COMPAT    long long
#define PY_PRIdOFF         "lld"
#elif (SIZEOF_OFF_T == SIZEOF_LONG)
#define PyLong_AsOff_t     PyLong_AsLong
#define PyLong_FromOff_t   PyLong_FromLong
#define PY_OFF_T_MAX       LONG_MAX
#define PY_OFF_T_MIN       LONG_MIN
#define PY_OFF_T_COMPAT    long
#define PY_PRIdOFF         "ld"
#else
# error off_t does not match either size_t, long, or long long!
#endif

#endif

typedef union {
    float as_float;
    int as_int;
    long as_long;
    long long as_longlong;
    size_t as_size;
    Py_ssize_t as_ssize;
    Py_off_t as_off;
    void *as_ptr;
} unified_t;

#endif
