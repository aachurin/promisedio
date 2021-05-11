// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "general_types.h"

Generate_PyType_Impl(UnifiedValue)
Generate_PyType_Impl(OperationContext, CLEAR(owner), CLEAR(promise))

int
GeneralTypes_module_init()
{
    if (PyType_Ready(&UnifiedValue_Type) < 0) {
        return -1;
    }
    if (PyType_Ready(&OperationContext_Type) < 0) {
        return -1;
    }
    return 0;
}
