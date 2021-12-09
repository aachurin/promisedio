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

Py_LOCAL_INLINE(void *)
_CTX__get(PyObject *obj)
{
    return _CTX__gettype(Py_TYPE(obj));
}

#define _CTX_getmodule(module) ((_modulestate *) _CTX__getmodule(module))
#define _CTX_gettype(type) ((_modulestate *) _CTX__gettype(type))
#define _CTX_get(obj) ((_modulestate *) _CTX__get(obj))
#define _CTX_getstored(ptr) ((_modulestate *)((ptr)->_ctx))
#define _CTX_setstored(ptr) _ctx_var = _CTX_getstored(ptr)
#define _CTX_setmodule(module) _ctx_var = _CTX_getmodule(module)
#define _CTX_settype(type) _ctx_var = _CTX_gettype(type)
#define _CTX_set(obj) _ctx_var = _CTX_get(obj)
#define S(some) _ctx->some

#endif //PROMISEDIO_CORE_MODULE_H
