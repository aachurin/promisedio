// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef TIMER_H
#define TIMER_H

#include "common.h"
#include "promise.h"

int Timer_Timeout1(Promise *promise, double timeout);
Promise * Timer_Timeout(double timeout);

#endif
