// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_LOOP_API_H
#define PROMISEDIO_LOOP_API_H

#include "loop.h"

#define Loop_Get() \
    Capsule_METH_NOARGS(LOOP_API_ID, LOOP_GET_API_ID, uv_loop_t*)

#define Loop_SetSig() \
    Capsule_FUNC_NOARGS(LOOP_API_ID, LOOP_SETSIG_API_ID, void)

#endif
