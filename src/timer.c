// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "timer.h"
#include "memory.h"
#include "loop.h"
#include <uv.h>

static void
timeout_callback(uv_timer_t *timer)
{
    ACQUIRE_GIL
    Promise_Resolve((Promise *) timer->data, Py_None);
    Py_DECREF(timer->data);
        Handle_Close(timer);
    RELEASE_GIL
}

int
Timer_Timeout1(Promise *promise, double timeout)
{
    uv_timer_t *timer;
    if ((timer = Handle_New(uv_timer_t)) == NULL)
        return -1;
    uv_timer_init(Loop_Get(), timer);
    Handle_SetFreeOnClose(timer);
    Py_INCREF(promise);
    timer->data = promise;
    uv_timer_start(timer, timeout_callback, (long long) (timeout * 1000), 0);
    return 0;
}

Promise *
Timer_Timeout(double timeout)
{
    Promise *promise;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if (Timer_Timeout1(promise, timeout)) {
        Py_DECREF(promise);
        return NULL;
    }
    return promise;
}
