// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef GENERAL_TYPES_H
#define GENERAL_TYPES_H

#include <Python.h>
#include "common.h"
#include "promise.h"
#include "c2py.h"

Generate_PyType_Header(
    UnifiedValue,
    MEMBER(value, unified_t)
)

Generate_PyType_Header(
    OperationContext,
    MEMBER(owner, PyObject *),
    MEMBER(promise, Promise *),
    MEMBER(param1, unified_t),
    MEMBER(param2, unified_t),
    MEMBER(param3, unified_t)
)

int GeneralTypes_module_init();

#endif