// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

// Rules:
// 1) Struct names and typedef's in camelcase: Promise, Deferred.
// 2) Functions: module_snake_case() or type_snake_case()
// 3) Constant definitions: UPPER_CASE
// 4) Other definitions: as you wish, just not like 1 or 2 (more suitable: Module_UPPER_CASE)

#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <Python.h>

//#define DEBUG_OUTPUT stderr

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

#ifdef MS_WINDOWS
/* Windows uses long long for offsets */
typedef long long Py_off_t;
#define PyLong_AsOff_t     PyLong_AsLongLong
#define PyLong_FromOff_t   PyLong_FromLongLong
#define PY_OFF_T_MAX       LLONG_MAX
#define PY_OFF_T_MIN       LLONG_MIN
#define PY_OFF_T_COMPAT    long long
#define PY_PRIdOFF         "lld"
#define PyLong_FromUint64_t PyLong_FromUnsignedLongLong
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
#if (SIZEOF_SIZE_T == 8)
#define PyLong_FromUint64_t PyLong_FromSize_t
#elif (SIZEOF_LONG_LONG == 8)
#define PyLong_FromUint64_t PyLong_FromUnsignedLongLong
#elif (SIZEOF_LONG == 8)
#define PyLong_FromUint64_t PyLong_FromUnsignedLong
#else
# error uint64_t does not match either size_t, long, or long long!
#endif
#endif

#define OWNED(ob) (Py_INCREF(ob), ob)
#define XOWNED(ob) (Py_XINCREF(ob), ob)

#include "converters.h"

#endif
