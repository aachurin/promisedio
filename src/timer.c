// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "timer.h"
#include "memory.h"
#include "loop.h"

typedef struct {
    uv_timer_t base;
    Promise *promise;
} timeout_handle;

static void
timeout_callback(timeout_handle *handle)
{
    ACQUIRE_GIL
    Promise_Resolve((Promise *) handle->promise, Py_None);
    Handle_Close(handle);
    RELEASE_GIL
}

static void
timeout_finalizer(timeout_handle *handle)
{
    Py_XDECREF(handle->promise);
}

Promise *
Timer_Timeout(uint64_t timeout)
{
    Promise *promise;
    timeout_handle *handle;
    if ((promise = Promise_New()) == NULL)
        return NULL;
    if ((handle = Handle_New(timeout_handle, timeout_finalizer)) == NULL) {
        Py_DECREF(promise);
        return NULL;
    }
    Py_INCREF(promise);
    handle->promise = promise;
    uv_timer_init(Loop_Get(), (uv_timer_t *) handle);
    uv_timer_start((uv_timer_t *) handle, (uv_timer_cb) timeout_callback, timeout, 0);
    return promise;
}

typedef struct timer_handle_s timer_handle;

typedef struct {
    PyObject_HEAD
    timer_handle *handle;
} Timer;

typedef struct timer_handle_s {
    uv_timer_t base;
    Timer *timer;
    PyObject *func;
} timer_handle;

static void
timer_dealloc(Timer *ob)
{
    if (ob->handle) {
        ob->handle->timer = NULL;
    }
    Mem_Del(ob);
}

static PyTypeObject TimerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Timer",
    .tp_basicsize = sizeof(Timer),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor) timer_dealloc
};

static inline Timer*
Timer_New()
{
    return Mem_New(Timer, &TimerType);
}

static void
timer_callback(timer_handle *handle)
{
    ACQUIRE_GIL
    PyObject *tmp = PyObject_CallNoArgs(handle->func);
    if (tmp == NULL) {
        Promise_PrintUnhandledException();
    } else {
        Py_DECREF(tmp);
    }
    Handle_Close(handle);
    RELEASE_GIL
}

static void
interval_callback(timer_handle *handle)
{
    ACQUIRE_GIL
    PyObject *tmp = PyObject_CallNoArgs(handle->func);
    if (tmp == NULL) {
        Promise_PrintUnhandledException();
    } else {
        Py_DECREF(tmp);
    }
    RELEASE_GIL
}

static void
timer_finalizer(timer_handle *handle)
{
    if (handle->timer) {
        handle->timer->handle = NULL;
    }
    Py_DECREF(handle->func);
}

PyObject *
Timer_Start(PyObject *func, uint64_t timeout, uint64_t repeat, int unref)
{
    Timer *timer = Timer_New();
    if (timer == NULL) {
        return NULL;
    }
    timer_handle *handle = Handle_New(timer_handle, timer_finalizer);
    if (handle == NULL) {
        Py_DECREF(timer);
        return NULL;
    }
    Py_INCREF(func);
    handle->func = func;
    handle->timer = timer;
    timer->handle = handle;
    uv_timer_init(Loop_Get(), (uv_timer_t *) handle);
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
Timer_Stop(PyObject *ob)
{
    if (Py_TYPE(ob) != &TimerType) {
        PyErr_SetString(PyExc_TypeError, "Timer object expected");
        return -1;
    }
    Handle_Close(((Timer *) ob)->handle);
    return 0;
}

int
Timer_module_init()
{
    if (PyType_Ready(&TimerType) < 0)
        return -1;
    return 0;
}
