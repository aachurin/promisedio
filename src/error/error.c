// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "core/base.h"

int
errormodule_init(PyObject *module)
{
    LOG("(%p)", module);
#define XX(code, _) if (PyModule_AddIntConstant(module, #code, UV_ ## code)) return -1;
    UV_ERRNO_MAP(XX)
#undef XX
    return 0;
}

static void
errormodule_free(void *module)
{
    LOG("(%p)", module);
}

static PyModuleDef_Slot errormodule_slots[] = {
    {Py_mod_exec, errormodule_init},
    {0, NULL}
};

static struct PyModuleDef errormodule_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "promisedio.error",
    .m_slots = errormodule_slots,
    .m_free = errormodule_free
};

PyMODINIT_FUNC
PyInit_error(void)
{
    return PyModuleDef_Init(&errormodule_def);
}
