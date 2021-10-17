// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_MODULE_H
#define PROMISEDIO_CORE_MODULE_H

#include "base.h"
#include "internal/pycore_moduleobject.h"

Py_LOCAL_INLINE(void *)
Module_GetState(PyObject *mod) {
    assert(PyModule_Check(mod));
    return ((PyModuleObject *)mod)->md_state;
}

#define _modulestate a__modulestate
#define _state a__state
#define _STATE_var _modulestate *_state
#define _STATE_get(obj) ((_modulestate *)((obj)->_state))
#define _STATE_getmodule(module) ((_modulestate *)Module_GetState(module))
#define _STATE_set(obj) _STATE_var = _STATE_get(obj);
#define _STATE_setmodule(module) _STATE_var = _STATE_getmodule(module);
#define _STATE_save(obj) (obj)->_state = _state
#define S(some) _state->some

#endif //PROMISEDIO_CORE_MODULE_H
