// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef LOOP_H
#define LOOP_H

#include <uv.h>

typedef struct {
    int use_python_default_sigint;
} LoopConfig;

LoopConfig * Loop_GetConfig();
int Loop_Run();
int Loop_Close();
uv_loop_t* Loop_Get();
int Loop_IsCreated();

#endif
