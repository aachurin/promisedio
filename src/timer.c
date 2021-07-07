// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "timer.h"
#include "memory.h"
#include "loop.h"
#include "utils.h"

typedef struct {
    Promise *promise;
    HANDLE_BASE(uv_timer_t)
} timeout_handle_t;

static void
timeout_callback(uv_timer_t *handle)
{
    ACQUIRE_GIL
    promise_resolve((Promise *) handle_container(timeout_handle_t, handle)->promise, Py_None);
    close_handle((uv_handle_t *) handle);
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
    Promise *promise = promise_new();
    if (!promise)
        return NULL;
    timeout_handle_t *handle = Handle_New(timeout_handle_t, timeout_finalizer);
    if (!handle) {
        Py_DECREF(promise);
        return NULL;
    }
    handle->promise = promise;
    uv_timer_init(loop_get(), &handle->handle);
    uv_timer_start(&handle->handle, (uv_timer_cb) timeout_callback, timeout, 0);
    return OWNED(promise);
}

typedef struct timer_handle_s timer_handle_t;

typedef struct {
    PyObject_HEAD
    timer_handle_t *handle;
} Timer;

typedef struct timer_handle_s {
    Timer *timer;
    PyObject *func;
    HANDLE_BASE(uv_timer_t)
} timer_handle_t;

static void
timer_dealloc(Timer *ob)
{
    if (ob->handle) {
        ob->handle->timer = NULL;
    }
    Object_Del(ob);
}

DefinePyType(TimerType, Timer, timer_dealloc);

static void
timer_callback(uv_timer_t *handle)
{
    ACQUIRE_GIL
    PyObject *ret = PyObject_CallNoArgs(handle_container(timer_handle_t, handle)->func);
    if (ret == NULL) {
        promise_print_unhandled_exception();
    } else {
        Py_DECREF(ret);
    }
    close_handle((uv_handle_t *) handle);
    RELEASE_GIL
}

static void
interval_callback(uv_timer_t *handle)
{
    ACQUIRE_GIL
    PyObject *tmp = PyObject_CallNoArgs(handle_container(timer_handle_t, handle)->func);
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
    Timer *timer = Object_New(Timer, &TimerType);
    if (!timer)
        return NULL;
    timer_handle_t *handle = Handle_New(timer_handle_t, timer_finalizer);
    if (!handle) {
        Py_DECREF(timer);
        return NULL;
    }
    Py_INCREF(func);
    handle->func = func;
    handle->timer = timer;
    timer->handle = handle;
    uv_timer_init(loop_get(), &handle->handle);
    if (unref) {
        uv_unref((uv_handle_t *) &handle->handle);
    }
    if (repeat) {
        uv_timer_start(&handle->handle, (uv_timer_cb) interval_callback, timeout, repeat);
    } else {
        uv_timer_start(&handle->handle, (uv_timer_cb) timer_callback, timeout, 0);
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
    Handle_Close(((Timer *) ob)->handle);
    return 0;
}

int
timer_module_init()
{
    if (PyType_Ready(&TimerType) < 0)
        return -1;
    return 0;
}
