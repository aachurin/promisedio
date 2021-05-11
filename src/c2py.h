// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef C2PY2_H
#define C2PY2_H

#include "evalmacros.h"
#include "memory.h"

#if SIZEOF_LONG == 8
#define c2py_from_uint64_t(o) PyLong_FromUnsignedLong(o)
#else
#define c2py_from_uint64_t(o) PyLong_FromUnsignedLongLong(o)
#endif

#define c2py_from_double(o) PyFloat_FromDouble(o)
#define c2py_from_uv_timespec_t(o) PyFloat_FromDouble(o.tv_sec + 1e-9*o.tv_nsec)

#define CLEAR(name) C, name, ~, ~
//#define INIT(name) C, name, ~, ~
#define MEMBER(name, type, ...) M, name, type, IF_ELSE(__VA_ARGS__)(~, CLEAR(name))(~)
#define READONLY(name, field, type) R, name, field, type
#define WRITEONLY(name, field, type) W, name, field, type
#define READWRITE(name, field, type) RW, name, field, type

#define _c2py_func_M(...)
#define _c2py_func_C(...)
#define _c2py_func_R(pytype, name, field, type)                         \
    static PyObject* pytype##_get_##name(pytype *self, void *closure) { \
        return c2py_from_##type(self->field);                           \
    }
#define _c2py_func_W(pytype, name, field, type)                                     \
    static int PyType##_set_##field(pytype *self, PyObject *value, void *closure) { \
        return c2py_to_##type(&(self->field), value);                               \
    }
#define _c2py_func_RW(pytype, name, field, type) \
    _c2py_func_R(pytype, name, field, type)      \
    _c2py_func_W(pytype, name, field, type)

#define _c2py_getset_M(...)
#define _c2py_getset_C(...)
#define _c2py_getset_R(pytype, name, ...) {#name, (getter) pytype##_get_##name, NULL, NULL, NULL},
#define _c2py_getset_W(pytype, name, ...) {#name, NULL, (setter) pytype##_set_##name, NULL, NULL},
#define _c2py_getset_RW(pytype, name, ...) {#name, (getter) pytype##_get_##name, (setter) pytype##_set_##name, NULL, NULL},

#define _c2py_init_M(...)
#define _c2py_init_C(pytype, name, ...) self->name = NULL;
#define _c2py_init_R(...)
#define _c2py_init_W(...)
#define _c2py_init_RW(...)

#define _c2py_clear_M(...)
#define _c2py_clear_C(pytype, name, ...) Py_XDECREF(self->name);
#define _c2py_clear_R(...)
#define _c2py_clear_W(...)
#define _c2py_clear_RW(...)

#define _c2py_member_M(pytype, name, type, ...) type name;
#define _c2py_member_C(...)
#define _c2py_member_R(...)
#define _c2py_member_W(...)
#define _c2py_member_RW(...)

#define _c2py_isgetset_M(...)
#define _c2py_isgetset_C(...)
#define _c2py_isgetset_R(...) 1
#define _c2py_isgetset_W(...) 1
#define _c2py_isgetset_RW(...) 1

#define _c2py_hasgetset(...) IF_ELSE(HAS_ARGS(__VA_ARGS__))(EVAL(_c2py_map(_c2py_isgetset_, ~, __VA_ARGS__)))()
#define _c2py_init(...) IF_ELSE(HAS_ARGS(__VA_ARGS__))(EVAL(_c2py_map(_c2py_init_, ~, __VA_ARGS__)))()
#define _c2py_clear(...) IF_ELSE(HAS_ARGS(__VA_ARGS__))(EVAL(_c2py_map(_c2py_clear_, ~, __VA_ARGS__)))()
#define _c2py_members(...) IF_ELSE(HAS_ARGS(__VA_ARGS__))(EVAL(_c2py_map(_c2py_member_, ~, __VA_ARGS__)))()
#define _c2py_funcs(pytype, ...) IF_ELSE(_c2py_hasgetset(__VA_ARGS__))(EVAL(_c2py_map(_c2py_func_, pytype, __VA_ARGS__)))()
#define _c2py_getsets(pytype, ...)                              \
    IF_ELSE(_c2py_hasgetset(__VA_ARGS__))(                      \
        static PyGetSetDef pytype##_getsetlist[] = {            \
            EVAL(_c2py_map(_c2py_getset_, pytype, __VA_ARGS__)) \
            {NULL}                                              \
        };                                                      \
    )()                                                         \

#define __c2py_map() _c2py_map
#define _c2py_map(m, pytype, kind, name, field, type, ...)  \
  m ## kind (pytype, name, field, type)                     \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(                           \
    DEFER2(__c2py_map)()(m, pytype, __VA_ARGS__)            \
  )(                                                        \
    /* Do nothing, just terminate */                        \
  )

#define _c2py_header(pytype, ...)                           \
    typedef struct pytype##_s {                             \
        PyObject_HEAD                                       \
        _c2py_members(__VA_ARGS__)                          \
    } pytype;                                               \
    pytype* pytype##_New();                                 \

#define _c2py_impl(pytype, ...)                             \
    _c2py_funcs(pytype, __VA_ARGS__)                        \
    _c2py_getsets(pytype, __VA_ARGS__)                      \
    static void                                             \
    pytype##_dealloc(pytype *self) {                        \
        _c2py_clear(__VA_ARGS__)                            \
        Mem_Del(self);                                      \
    }                                                       \
    static PyTypeObject pytype##_Type = {                   \
        PyVarObject_HEAD_INIT(NULL, 0)                      \
        .tp_name = #pytype,                                 \
        .tp_basicsize = sizeof(pytype),                     \
        .tp_itemsize = 0,                                   \
        .tp_flags = Py_TPFLAGS_DEFAULT,                     \
        IF_ELSE(_c2py_hasgetset(__VA_ARGS__))(              \
            .tp_getset = pytype##_getsetlist,               \
        )()                                                 \
        .tp_dealloc = (destructor) pytype##_dealloc         \
    };                                                      \
    pytype*                                                 \
    pytype##_New()                                          \
    {                                                       \
        pytype *self = Mem_New(pytype, &pytype##_Type);     \
        if (self == NULL)                                   \
            return NULL;                                    \
        _c2py_init(__VA_ARGS__)                             \
        return self;                                        \
    }

#define Generate_PyType_Header(pytype, ...) \
    _c2py_header(pytype, __VA_ARGS__)

#define Generate_PyType_Impl(pytype, ...) \
    _c2py_impl(pytype, __VA_ARGS__)

#define Generate_PyType(pytype, ...)  \
    _c2py_header(pytype, __VA_ARGS__) \
    _c2py_impl(pytype, __VA_ARGS__)

#endif
