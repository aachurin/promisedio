// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "error.h"

int
error_module_init(PyObject *module)
{
    #define XX(code, _) if (PyModule_AddIntConstant(module, #code, UV_ ## code)) return -1;
    UV_ERRNO_MAP(XX)
    #undef XX
    return 0;
}
