
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_TIMER_API
#define PROMISEDIO_TIMER_API

#include "timer/timer.h"

#define TIMER_API timer_api_46888e8a8b7183eec5ac36247a1a84cb

#define TIMER_TIMEOUT_ID 0
#define Timer_Timeout(...) \
  (*(Promise * (*) (void*, uint64_t timeout))(_ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__api[TIMER_TIMEOUT_ID]))( \
    _ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__ctx, __VA_ARGS__)

#define TIMER_START_ID 1
#define Timer_Start(...) \
  (*(PyObject * (*) (void*, PyObject *func, uint64_t timeout, uint64_t repeat, int unref))(_ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__api[TIMER_START_ID]))( \
    _ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__ctx, __VA_ARGS__)

#define TIMER_STOP_ID 2
#define Timer_Stop(...) \
  (*(int (*) (void*, PyObject *ob))(_ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__api[TIMER_STOP_ID]))( \
    _ctx->timer_api_46888e8a8b7183eec5ac36247a1a84cb__ctx, __VA_ARGS__)

#endif
