// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "common.h"
#include "promise.h"
#include "memory.h"
#include "loop.h"

static volatile int loop_running = 0;
static volatile int loop_initialised = 0;
static volatile int loop_break = 0;
static uv_signal_t sigint_handle_struct;
static uv_signal_t* sigint_handle_ptr = NULL;
static uv_async_t async_event_struct;
static uv_async_t* async_event_ptr = NULL;
static uv_loop_t default_loop_struct;
static uv_loop_t* default_loop_ptr = NULL;

LoopConfig loop_config = {
    .use_python_default_sigint = 0
};

int
Loop_IsCreated()
{
    return default_loop_ptr != NULL;
}

uv_loop_t*
Loop_Get()
{
    if (default_loop_ptr != NULL)
        return default_loop_ptr;

    if (uv_loop_init(&default_loop_struct))
        return NULL;

    default_loop_ptr = &default_loop_struct;
    return default_loop_ptr;
}

LoopConfig *
Loop_GetConfig()
{
    if (loop_initialised) {
        PyErr_SetString(PyExc_RuntimeError, "The loop has been started.");
        return NULL;
    }
    return &loop_config;
}

static void
default_python_sigint_handler(int sig_num)
{
    int save_errno = errno;
    PyErr_SetInterrupt();
    #ifndef HAVE_SIGACTION
    PyOS_setsig(sig_num, signal_int_handler);
    #endif
    errno = save_errno;
    #ifdef MS_WINDOWS
    SetEvent(_PyOS_SigintEvent());
    #endif
    LOG("default_python_sigint_handler()");
    if (loop_running) {
        loop_break = 1;
        uv_async_send(async_event_ptr);
    }
}

static void
on_signal_sigint(uv_signal_t* handle, int signum)
{
    ACQUIRE_GIL
    LOG("on_signal_sigint(%p)", handle);
    loop_break = 1;
    PyErr_SetNone(PyExc_KeyboardInterrupt);
    RELEASE_GIL
}

static void
initialise_loop(uv_loop_t *loop)
{
    LOG("initialise_loop(%p)", loop);
    if (loop_config.use_python_default_sigint) {
        uv_async_init(loop, &async_event_struct, NULL);
        uv_unref((uv_handle_t *) &async_event_struct);
        async_event_ptr = &async_event_struct;
        PyOS_setsig(SIGINT, default_python_sigint_handler);
    } else {
        uv_signal_init(loop, &sigint_handle_struct);
        uv_signal_start(&sigint_handle_struct, on_signal_sigint, SIGINT);
        uv_unref((uv_handle_t *) &sigint_handle_struct);
        sigint_handle_ptr = &sigint_handle_struct;
    }
}

static void
finalise_loop(uv_loop_t *loop)
{
    LOG("finalise_loop(%p)", loop);
    if (async_event_ptr != NULL) {
        uv_close((uv_handle_t *) async_event_ptr, NULL);
        async_event_ptr = NULL;
    }
    if (sigint_handle_ptr != NULL) {
        uv_close((uv_handle_t *) sigint_handle_ptr, NULL);
        sigint_handle_ptr = NULL;
    }
}


static void
loop_close_handle(uv_handle_t* handle, void* arg)
{
    Handle_Close(handle);
}

int
Loop_Close()
{
    if (loop_running) {
        PyErr_SetString(PyExc_RuntimeError, "The loop is running.");
        return -1;
    }
    loop_running = 1;
    uv_loop_t *loop = Loop_Get();
    finalise_loop(loop);
    uv_walk(loop, loop_close_handle, NULL);
    uv_run(loop, UV_RUN_DEFAULT);
    LOG("Loop_Close(%p)", loop);
    uv_loop_close(loop);
    default_loop_ptr = NULL;
    PromiseChain_Clear();
    loop_initialised = 0;
    loop_running = 0;
    return 0;
}

int
Loop_Run()
{
    if (loop_running) {
        PyErr_SetString(PyExc_RuntimeError, "The loop is running.");
        return -1;
    }

    loop_break = 0;
    loop_running = 1;
    uv_loop_t *loop = Loop_Get();
    LOG("Loop_Run() -> %p", loop);

    if (!loop_initialised) {
        loop_initialised = 1;
        initialise_loop(loop);
    }

    int uv_do_more = 1, promise_do_more = 1;
    BEGIN_ALLOW_THREADS
    while (uv_do_more || promise_do_more) {
        ACQUIRE_GIL
        promise_do_more = PromiseChain_Process();
        RELEASE_GIL
        if (promise_do_more < 0) {
            break;
        }
        uv_do_more = uv_run(loop, UV_RUN_ONCE);
        if (loop_break) {
            break;
        }
    }
    END_ALLOW_THREADS

    loop_running = 0;
    if (PyErr_Occurred()) {
        return -1;
    }
    return 0;
}
