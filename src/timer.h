// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef TIMER_H
#define TIMER_H

#include "common.h"
#include "promise.h"
#include <uv.h>

Promise * Timer_Timeout(uint64_t timeout);
PyObject * Timer_Start(PyObject *func, uint64_t timeout, uint64_t repeat, int unref);
int Timer_Stop(PyObject *ob);
int Timer_module_init();

#endif
