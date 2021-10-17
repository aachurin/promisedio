// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_TIMER_API_H
#define PROMISEDIO_TIMER_API_H

#include "timer.h"

#define Timer_Timeout(...) \
    Capsule_METH(TIMER_API_ID, TIMER_TIMEOUT_API_ID, PyObject*, ARGS(uint64_t), __VA_ARGS__)

#define Timer_Start(...) \
    Capsule_METH(TIMER_API_ID, TIMER_START_API_ID, PyObject*, ARGS(PyObject*, uint64_t, uint64_t, int), __VA_ARGS__)

#define Timer_Stop(...) \
    Capsule_METH(TIMER_API_ID, TIMER_STOP_API_ID, int, ARGS(PyObject*), __VA_ARGS__)

#endif
