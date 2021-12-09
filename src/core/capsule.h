// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_CAPSULE_H
#define PROMISEDIO_CORE_CAPSULE_H

#include "base.h"

#include PYSRC(capsule.h)

Py_LOCAL_INLINE(void *)
Capsule_Load(PyObject *module, const char *api_id)
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

#define Capsule_CREATE(module, api_id)                                                                                                     \
    do {                                                                                                                                   \
        static void *c_api[] = api_id##_CAPSULE;                                                                                           \
        PyObject *_api = PyCapsule_New(&(c_api), STRINGIFY(api_id), NULL);                                                                 \
        if (PyModule_AddObject(module, STRINGIFY(api_id), _api) < 0) {                                                                     \
            Py_XDECREF(_api);                                                                                                              \
            return -1;                                                                                                                     \
        }                                                                                                                                  \
    } while(0)

#define Capsule_MOUNT(api_id)                                                                                                              \
    PyObject *CAT(api_id,__module);                                                                                                        \
    void *CAT(api_id,__ctx);                                                                                                               \
    void **CAT(api_id,__api);

#define Capsule_VISIT(api_id) Py_VISIT(_ctx->CAT(api_id,__module))
#define Capsule_CLEAR(api_id) Py_CLEAR(_ctx->CAT(api_id,__module))
#define Capsule_LOAD(module_name, api_id)                                                                                                  \
    do {                                                                                                                                   \
        PyObject *_module = PyImport_ImportModule(module_name);                                                                            \
        void *_api = Capsule_Load(_module, STRINGIFY(api_id));                                                                             \
        if (!_api) {                                                                                                                       \
            Py_XDECREF(_module);                                                                                                           \
            return -1;                                                                                                                     \
        }                                                                                                                                  \
        _ctx->CAT(api_id,__module) = _module;                                                                                              \
        _ctx->CAT(api_id,__ctx) = _CTX__getmodule(_module);                                                                                \
        _ctx->CAT(api_id,__api) = _api;                                                                                                    \
    } while (0)

Py_LOCAL_INLINE(void *)
Capsule_GetFunc(const char *module_name, const char *api_id, int func_id)
{
    PyObject *_module = PyImport_ImportModule(module_name);
    void *ret = NULL;
    void **_api = Capsule_Load(_module, api_id);
    if (_api) {
        ret = _api[func_id];
    }
    Py_XDECREF(_module);
    return ret;
}

#define Capsule_GetFunc(module_name, api_id, func_id) Capsule_GetFunc(module_name, STRINGIFY(api_id), func_id)
#define CAPSULE_API(api_key, op) static op

#endif //PROMISEDIO_CORE_CAPSULE_H
