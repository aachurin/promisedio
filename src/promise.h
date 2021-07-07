// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISE_H
#define PROMISE_H

#include "common.h"
#include "chain.h"

#define Promise_CALLBACK(promise, fulfilled, rejected, context) \
    promise_callback(promise, (promisecallback) fulfilled, (promisecallback) rejected, (PyObject *) context)

#define Promise_DATA(promise, type) \
        ((type *) &((promise)->data))

typedef PyObject * (*promisecallback)(PyObject *value, void *data);
typedef struct promise_s Promise;

struct promise_s {
    PyObject_HEAD
    Chain_PROTOCOL(Promise)
    Chain_NODE_PROTOCOL(Promise)
    PyObject *fulfilled;
    PyObject *rejected;
    PyObject *coro;
    PyObject *value;
    PyObject *context;
    char data[64];
    unsigned int flags;
};

typedef struct {
    PyObject_HEAD
    Promise *promise;
} Deferred;

typedef struct {
    PyObject_HEAD
    Promise *promise;
} PromiseIter;

Promise * promise_new();
Promise * promise_new_resolved(PyObject *value);
Promise * promise_then(Promise *p);
void promise_resolve(Promise *p, PyObject *value);
void promise_reject(Promise *p, PyObject *value);
int promise_reject_args(Promise *p, PyObject *exc, PyObject *args);
int promise_reject_string(Promise *p, PyObject *exc, const char *msg);
void promise_reject_py_exc(Promise *p);
void promise_callback(Promise *p, promisecallback fulfilled, promisecallback rejected, PyObject *context);
Deferred * deferred_new();
void promise_clear_chain();
int promise_process_chain();
int promise_exec_async(PyObject* coro);
void promise_print_unhandled_exception();
void promise_clear_freelists();
int promise_module_init();

#endif
