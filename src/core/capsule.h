// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_CAPSULE_H
#define PROMISEDIO_CORE_CAPSULE_H

#include "base.h"

#include PYSRC(capsule.h)

Py_LOCAL_INLINE(void *)
Capsule_load(PyObject *module, const char *api_id)
{
    if (!module) {
        PyErr_Format(PyExc_ImportError, "could not import \"%s\"", api_id);
        return NULL;
    }
    PyObject *object = PyObject_GetAttrString(module, api_id);
    if (!object) {
        return NULL;
    }
    void *return_value = NULL;
    if (PyCapsule_IsValid(object, api_id)) {
        return_value = ((PyCapsule *) object)->pointer;
    } else {
        PyErr_Format(PyExc_AttributeError, "capsule \"%s\" is not valid", api_id);
    }
    Py_DECREF(object);
    return return_value;
}

Py_LOCAL_INLINE(void *)
Capsule_load_one(const char *module_name, const char *api_id, int func_id)
{
    PyObject *_module = PyImport_ImportModule(module_name);
    void *ret = NULL;
    void **_api = Capsule_load(_module, api_id);
    if (_api) {
        ret = _api[func_id];
    }
    Py_XDECREF(_module);
    return ret;
}

#define Capsule_CREATE(module, api_id, api_obj)                                 \
    do {                                                                        \
        PyObject *_api = PyCapsule_New(&(api_obj), STRINGIFY(api_id), NULL);    \
        if (PyModule_AddObject(module, STRINGIFY(api_id), _api) < 0) {          \
            Py_XDECREF(_api);                                                   \
            return -1;                                                          \
        }                                                                       \
    } while(0)

#define Capsule_MOUNT_POINT(api_id)     \
    PyObject *CAT(api_id,__module);     \
    void *CAT(api_id,__state);          \
    void **CAT(api_id,__api);

#define Capsule_TRAVERSE(api_id) Py_VISIT(_state->CAT(api_id,__module))
#define Capsule_CLEAR(api_id) Py_CLEAR(_state->CAT(api_id,__module))
#define Capsule_LOAD(module_name, api_id)                       \
    do {                                                        \
        PyObject *_module = PyImport_ImportModule(module_name); \
        void *_api = Capsule_load(_module, STRINGIFY(api_id));  \
        if (!_api) {                                            \
            Py_XDECREF(_module);                                \
            return -1;                                          \
        }                                                       \
        _state->CAT(api_id,__module) = _module;                 \
        _state->CAT(api_id,__state) = Module_GetState(_module); \
        _state->CAT(api_id,__api) = _api;                       \
    } while (0)

#define Capsule_LOAD_FUNC(module_name, api_id, func_id) Capsule_load_one(module_name, STRINGIFY(api_id), func_id)

#define ARGS(...) __VA_ARGS__

#define Capsule_METH(api_id, index, ret, args, ...) \
    (*(ret (*) (void*, args))(_state->CAT(api_id,__api)[index]))(_state->CAT(api_id,__state), __VA_ARGS__)

#define Capsule_METH_NOARGS(api_id, index, ret) \
    (*(ret (*) (void*))(_state->CAT(api_id,__api)[index]))(_state->CAT(api_id,__state))

#define Capsule_FUNC(api_id, index, ret, args, ...) \
    (*(ret (*) (args))(_state->CAT(api_id,__api)[index]))(__VA_ARGS__)

#define Capsule_FUNC_NOARGS(api_id, index, ret) \
    (*(ret (*) ())(_state->CAT(api_id,__api)[index]))()

#endif //PROMISEDIO_CORE_CAPSULE_H
