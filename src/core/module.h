// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_MODULE_H
#define PROMISEDIO_CORE_MODULE_H

#include "base.h"
#include "internal/pycore_moduleobject.h"

#define _modulestate a__modulestate
#define _ctx a__ctx
#define _ctx_var _modulestate *_ctx

Py_LOCAL_INLINE(void *)
_CTX__getmodule(PyObject *module)
{
    return ((PyModuleObject *) module)->md_state;
}

Py_LOCAL_INLINE(void *)
_CTX__gettype(PyTypeObject *obj)
{
    return _CTX__getmodule(((PyHeapTypeObject *) obj)->ht_module);
}

#define _CTX_get_module(module) ((_modulestate *) _CTX__getmodule(module))
#define _CTX_set_module(module) _ctx_var = _CTX_get_module(module)
#define _CTX_get_type(type) ((_modulestate *) _CTX__gettype(type))
#define _CTX_set_type(type) _ctx_var = _CTX_get_type(type)
#define _CTX_get(ptr) ((_modulestate *)((ptr)->_ctx))
#define _CTX_set(ptr) _ctx_var = _CTX_get(ptr)
#define _CTX_save(ptr) (ptr)->_ctx = _ctx;
#define S(some) _ctx->some

#endif //PROMISEDIO_CORE_MODULE_H
