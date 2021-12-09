// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_BASE_H
#define PROMISEDIO_CORE_BASE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifdef BUILD_DEBUG_LOG
#define LOG(...)                                                        \
PySys_FormatStderr("%s:%d, %s -- ", __FILE__, __LINE__, __func__),      \
PySys_FormatStderr(__VA_ARGS__),                                        \
PySys_WriteStderr("\n")
#define LOGС(cond, ...)                                                 \
if (cond) {                                                             \
    PySys_FormatStderr("%s:%d, %s -- ", __FILE__, __LINE__, __func__),  \
    PySys_FormatStderr(__VA_ARGS__),                                    \
    PySys_WriteStderr("\n");                                            \
}
#else
#define LOG(...)
#define LOGC(...)
#endif

#ifdef BUILD_DEBUG_MEM
#define MEMLOG(action, ptr, type)                                       \
    PySys_FormatStderr("#%s\t(%p, %s)", action, ptr, type),             \
    PySys_FormatStderr(" -- %s:%d, %s", __FILE__, __LINE__, __func__),  \
    PySys_WriteStderr("\n")
#else
#define MEMLOG(...)
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

#define Py_IsMainInterp() (PyInterpreterState_Get() == PyInterpreterState_Main())
#define STRINGIFY(X) STRINGIFY2(X)
#define STRINGIFY2(X) #X
#define CAT(X, Y) CAT_2(X,Y)
#define CAT_2(X, Y) X##Y
#define CAT3(A, B, C) CAT(A,CAT(B,C))
#define PYSRC(NAME) STRINGIFY(CAT3(pysource/PY_MAJOR_VERSION.PY_MINOR_VERSION.PY_MICRO_VERSION, _, NAME))

#ifndef NOGIL
#define BEGIN_ALLOW_THREADS Py_BEGIN_ALLOW_THREADS
#define END_ALLOW_THREADS Py_END_ALLOW_THREADS
#else
#define BEGIN_ALLOW_THREADS
#define END_ALLOW_THREADS
#endif

#endif //PROMISEDIO_CORE_BASE_H
