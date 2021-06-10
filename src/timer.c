// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "timer.h"
#include "memory.h"
#include "loop.h"
#include "utils.h"

typedef struct {
    uv_timer_t base;
    Promise *promise;
} timeout_handle_t;

static void
timeout_callback(timeout_handle_t *handle)
{
    ACQUIRE_GIL
    promise_resolve((Promise *) handle->promise, Py_None);
    Handle_CLOSE(handle);
    RELEASE_GIL
}

static void
timeout_finalizer(timeout_handle_t *handle)
{
    Py_XDECREF(handle->promise);
}

Promise *
timer_timeout(uint64_t timeout)
{
    Promise *promise;
    timeout_handle_t *handle;
    if ((promise = promise_new()) == NULL)
        return NULL;
    if ((handle = Handle_NEW(timeout_handle_t, timeout_finalizer)) == NULL) {
        Py_DECREF(promise);
        return NULL;
    }
    Py_INCREF(promise);
    handle->promise = promise;
    uv_timer_init(loop_get(), (uv_timer_t *) handle);
    uv_timer_start((uv_timer_t *) handle, (uv_timer_cb) timeout_callback, timeout, 0);
    return promise;
}

typedef struct timer_handle_s timer_handle_t;

typedef struct {
    PyObject_HEAD
    timer_handle_t *handle;
} Timer;

typedef struct timer_handle_s {
    uv_timer_t base;
    Timer *timer;
    PyObject *func;
} timer_handle_t;

static void
timer_dealloc(Timer *ob)
{
    if (ob->handle) {
        ob->handle->timer = NULL;
    }
    Mem_DEL(ob);
}

DefinePyType(TimerType, Timer, timer_dealloc);

static inline Timer*
Timer_New()
{
    return Mem_NEW(Timer, &TimerType);
}

static void
timer_callback(timer_handle_t *handle)
{
    ACQUIRE_GIL
    PyObject *ret = PyObject_CallNoArgs(handle->func);
    if (ret == NULL) {
        promise_print_unhandled_exception();
    } else {
        Py_DECREF(ret);
    }
    Handle_CLOSE(handle);
    RELEASE_GIL
}

static void
interval_callback(timer_handle_t *handle)
{
    ACQUIRE_GIL
    PyObject *tmp = PyObject_CallNoArgs(handle->func);
    if (tmp == NULL) {
        promise_print_unhandled_exception();
    } else {
        Py_DECREF(tmp);
    }
    RELEASE_GIL
}

static void
timer_finalizer(timer_handle_t *handle)
{
    if (handle->timer) {
        handle->timer->handle = NULL;
    }
    Py_DECREF(handle->func);
}

PyObject *
timer_start(PyObject *func, uint64_t timeout, uint64_t repeat, int unref)
{
    Timer *timer = Timer_New();
    if (timer == NULL) {
        return NULL;
    }
    timer_handle_t *handle = Handle_NEW(timer_handle_t, timer_finalizer);
    if (handle == NULL) {
        Py_DECREF(timer);
        return NULL;
    }
    Py_INCREF(func);
    handle->func = func;
    handle->timer = timer;
    timer->handle = handle;
    uv_timer_init(loop_get(), (uv_timer_t *) handle);
    if (unref) {
        uv_unref((uv_handle_t *) handle);
    }
    if (repeat) {
        uv_timer_start((uv_timer_t *) handle, (uv_timer_cb) interval_callback, timeout, repeat);
    } else {
        uv_timer_start((uv_timer_t *) handle, (uv_timer_cb) timer_callback, timeout, 0);
    }
    return (PyObject *) timer;
}

int
timer_stop(PyObject *ob)
{
    if (Py_TYPE(ob) != &TimerType) {
        PyErr_SetString(PyExc_TypeError, "Timer object expected");
        return -1;
    }
    Handle_CLOSE(((Timer *) ob)->handle);
    return 0;
}

int
timer_module_init()
{
    if (PyType_Ready(&TimerType) < 0)
        return -1;
    return 0;
}
