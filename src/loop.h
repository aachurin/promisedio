// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef LOOP_H
#define LOOP_H

#include <uv.h>

typedef struct {
    int use_python_default_sigint;
} LoopConfig;

LoopConfig * loop_get_config();
int loop_run();
int loop_close();
uv_loop_t* loop_get();
int loop_is_created();

#endif
