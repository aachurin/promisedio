// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISE_H
#define PROMISE_H

#define Promise_RETURN_RESOLVED(value) \
    return (PyObject *) Promise_NewResolved(value)

#define Promise_RETURN_NONE() \
    return (PyObject *) Promise_NewResolved(NULL)

#define Promise_CALLBACK(promise, fulfilled, rejected, context) \
    Promise_Callback(promise,                                   \
        (promisecallback) fulfilled,                            \
        (promisecallback) rejected,                             \
        (PyObject *) context)

typedef PyObject * (*promisecallback)(PyObject* context, PyObject *value);
typedef struct promise_obj Promise;

struct promise_obj {
    PyObject_HEAD
    Promise *next;              // next promise in the chain
    Promise *head;              // own promise chain head
    Promise *tail;              // own promise chain tail
    PyObject *fulfilled;
    PyObject *rejected;
    PyObject *finally;
    PyObject *coro;
    PyObject *value;
    PyObject *context;
    unsigned int flags;
};

typedef struct {
    PyObject_HEAD
    Promise *promise;
} Deferred;

typedef struct {
    PyObject_HEAD
    Promise *promise;
} Promiseiter;

Promise * Promise_New();
Promise * Promise_NewResolved(PyObject *value);
Promise * Promise_Then(Promise *self);
int Promise_Resolve(Promise *self, PyObject *value);
int Promise_Reject(Promise *self, PyObject *value);
int Promise_RejectWithPyErr(Promise *self);
void Promise_Callback(Promise *self, promisecallback fulfilled, promisecallback rejected, PyObject *context);
Deferred * Deferred_New();
void PromiseChain_Clear();
int PromiseChain_Process();
int Promise_ExecAsync(PyObject* coro);
void Promise_PrintUnhandledException();
int Promise_module_init();

#endif
