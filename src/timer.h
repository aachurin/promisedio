// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef TIMER_H
#define TIMER_H

#include "common.h"
#include "promise.h"

Promise * timer_timeout(uint64_t timeout);
PyObject * timer_start(PyObject *func, uint64_t timeout, uint64_t repeat, int unref);
int timer_stop(PyObject *ob);
int timer_module_init();

#endif
