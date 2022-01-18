// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "core/base.h"
#include "core/chain.h"
#include "core/memory.h"
#include "core/module.h"
#include "core/capsule.h"
#include "promise_internal.h"

typedef struct {
    Chain_ROOT(Promise)
} Promisechain;

typedef struct {
    /* Promise chain */
    Promisechain promisechain;
    /* Types */
    PyTypeObject *PromiseType;
    PyTypeObject *DeferredType;
    PyTypeObject *PromiseiterType;
    PyTypeObject *LockType;
    PyTypeObject *QueueType;
    PyObject *EventType;
    PyObject *NoArgs;
    PyObject *print_stack;
    /* Callback */
    int in_chain_routine;
    unlockloop unlockloop_func;
    void *unlockloop_ctx;
    /* State */
    PyObject *await_event;
    Py_ssize_t promise_count;
    /* Freelists */
    Freelist_GC_HEAD(PromiseType)
    Freelist_GC_HEAD(DeferredType)
    Freelist_GC_HEAD(PromiseiterType)
} _modulestate;

struct promise_s {
    PROMISE_PUBLIC_FIELDS
    PyObject *fulfilled;
    PyObject *rejected;
    PyObject *coro;
    PyObject *value;
    _ctx_var;
    Chain_ROOT(Promise)
};

typedef struct {
    PyObject_HEAD
    Promise *promise;
    _ctx_var;
} Deferred;

typedef struct {
    PyObject_HEAD
    Promise *promise;
    _ctx_var;
} Promiseiter;

typedef struct {
    PyObject_HEAD
    PyObject *ctx;
    int locked;
    _ctx_var;
    Chain_ROOT(Promise)
} Lock;

typedef struct {
    PyObject_HEAD
} NoArgs;

#include "clinic/promise.c.h"

/*[clinic input]
module promise
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=c425861e022a97bb]*/

/*[clinic input]
promise.clearfreelists

Clear freelists
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_clearfreelists_impl(PyObject *module)
/*[clinic end generated code: output=3376cbbe518b4304 input=1a20bd413c302cc1]*/
{
    _CTX_set_module(module);
    Freelist_GC_Clear(PromiseType);
    Freelist_GC_Clear(PromiseiterType);
    Freelist_GC_Clear(DeferredType);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.setfreelistlimits
    promise_limit: Py_ssize_t = -1
    promiseiter_limit: Py_ssize_t = -1
    deferred_limit: Py_ssize_t = -1

Set freelist limits
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_setfreelistlimits_impl(PyObject *module, Py_ssize_t promise_limit,
                               Py_ssize_t promiseiter_limit,
                               Py_ssize_t deferred_limit)
/*[clinic end generated code: output=98c0cac2cb949496 input=0575b1475d7fec22]*/
{
    _CTX_set_module(module);
    if (promise_limit >= 0) {
        Freelist_GC_Limit(PromiseType, promise_limit);
    }
    if (promiseiter_limit >= 0) {
        Freelist_GC_Limit(PromiseiterType, promiseiter_limit);
    }
    if (deferred_limit >= 0) {
        Freelist_GC_Limit(DeferredType, deferred_limit);
    }
    Py_RETURN_NONE;
}

Py_LOCAL(void)
print_unhandled_exception_from_dealloc(PyObject *value)
{
    PySys_WriteStderr("Unhandled promise rejection:\n");
    if (value) {
        PyObject *error_type, *error_value, *error_traceback;
        PyErr_Fetch(&error_type, &error_value, &error_traceback);
        PyObject *exc = PyExceptionInstance_Class(value);
        PyObject *tb = PyException_GetTraceback(value);
        if (!tb)
            tb = Py_NewRef(Py_None);
        PyErr_Display(exc, value, tb);
        PySys_WriteStderr("\n");
        PyException_SetTraceback(value, Py_None);
        Py_DECREF(tb);
        PyErr_Restore(error_type, error_value, error_traceback);
    } else {
        PySys_WriteStderr("lost exception value\n");
    }
}

/* Start loop and set scheduler hook and ctx*/
CAPSULE_API(PROMISE_API, int)
Promise_StartLoop(_ctx_var, unlockloop unlock_func, void *ctx)
{
    if (S(unlockloop_func)) {
        PyErr_SetString(PyExc_RuntimeError, "The loop is already running.");
        return -1;
    }
    S(unlockloop_func) = unlock_func;
    S(unlockloop_ctx) = ctx;
    return 0;
}

/* Set scheduler loop release hook */
CAPSULE_API(PROMISE_API, int)
Promise_StopLoop(_ctx_var, unlockloop unlock_func, void *ctx)
{
    if (S(unlockloop_func) || S(unlockloop_ctx)) {
        if (unlock_func != S(unlockloop_func) || ctx != S(unlockloop_ctx))
            return -1;
        S(unlockloop_func) = NULL;
        S(unlockloop_ctx) = NULL;
        return 0;
    }
    return -1;
}

/* Create new Promise object */
CAPSULE_API(PROMISE_API, Promise *)
Promise_New(_ctx_var)
{
    Promise *self = (Promise *) Freelist_GC_New(PromiseType);
    if (!self)
        return NULL;
    Chain_INIT(self);
    Chain_NODEINIT(self);
    self->value = NULL;
    self->coro = NULL;
    self->flags = PROMISE_INITIAL;
    self->fulfilled = NULL;
    self->rejected = NULL;
    self->ctx = NULL;
    _CTX_save(self);
    PyTrack_MarkAllocated(self);
    PyObject_GC_Track(self);
    S(promise_count)++;
    LOG("promise_count=%zd", S(promise_count));
    return self;
}

/* Internal method for future use. */
CAPSULE_API(PROMISE_API, void)
Promise_Callback(Promise *self, promisecb fulfilled, promisecb rejected)
{
    assert(!(self->flags & (PROMISE_HAS_CALLBACK | PROMISE_FREEZED)));
    self->flags |= PROMISE_C_CALLBACK;
    self->fulfilled = (PyObject *) fulfilled;
    self->rejected = (PyObject *) rejected;
}

CAPSULE_API(PROMISE_API, void)
Promise_PyCallback(Promise *self, PyObject *fulfilled, PyObject *rejected)
{
    assert(!(self->flags & (PROMISE_HAS_CALLBACK | PROMISE_FREEZED)));
    self->flags |= PROMISE_PY_CALLBACK;
    PyTrack_XINCREF(fulfilled);
    PyTrack_XINCREF(rejected);
    self->fulfilled = fulfilled;
    self->rejected = rejected;
}

#define schedule_promise(self, val, flag, invoke_callback)                      \
do {                                                                            \
    PyTrack_INCREF(val);                                                        \
    (self)->value = val;                                                        \
    (self)->flags |= flag;                                                      \
    Py_INCREF(self);                                                            \
    Chain_APPEND(&S(promisechain), self);                                       \
    if (invoke_callback && (!(S(in_chain_routine))) && S(unlockloop_func)) {    \
        S(unlockloop_func)(S(unlockloop_ctx));                                  \
    }                                                                           \
    S(promise_count)--;                                                         \
    LOG("schedule_promise(%p, invoke_callback=%d): promise_count=%zd",          \
        self, invoke_callback, S(promise_count));                               \
} while (0)

/* Create a new resolved promise, steals value reference */
CAPSULE_API(PROMISE_API, Promise *)
Promise_NewResolved(_ctx_var, PyObject *value, PyObject *func)
{
    Promise *promise = Promise_New(_ctx);
    if (promise) {
        if (value == NULL) {
            schedule_promise(promise, S(NoArgs), PROMISE_FULFILLED, 0);
        } else if (value == Py_None) {
            schedule_promise(promise, Py_None, PROMISE_FULFILLED, 0);
        } else {
            schedule_promise(promise, value, PROMISE_FULFILLED, 0);
            Py_DECREF(value);
        }
        if (func) {
            Promise_PyCallback(promise, func, NULL);
        }
        return promise;
    }
    return NULL;
}

/* Create a new promise derived from the given. */
CAPSULE_API(PROMISE_API, Promise *)
Promise_Then(Promise *self)
{
    _CTX_set(self);
    Promise *promise = Promise_New(_ctx);
    if (!promise)
        return NULL;
    self->flags |= PROMISE_VALUABLE;
    if (self->flags & PROMISE_RESOLVED) {
        schedule_promise(promise, self->value, (self->flags & PROMISE_SCHEDULED) | PROMISE_INTERIM, 0);
    } else {
        Py_INCREF(promise);
        promise->flags |= PROMISE_INTERIM;
        Chain_APPEND(self, promise);
    }
    return promise;
}

/* Resolve promise. */
CAPSULE_API(PROMISE_API, void)
Promise_ResolveEx(Promise *self, PyObject *value, int invoke_callback)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED)) {
        _CTX_set(self);
        schedule_promise(self, value, PROMISE_FULFILLED, invoke_callback);
    }
}

/* Resolve promise (callback is suppressed, the best for calling from loop callbacks) */
CAPSULE_API(PROMISE_API, void)
Promise_Resolve(Promise *self, PyObject *value)
{
    return Promise_ResolveEx(self, value, 0);
}

/* Reject promise */
CAPSULE_API(PROMISE_API, void)
Promise_RejectEx(Promise *self, PyObject *value, int invoke_callback)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED)) {
        _CTX_set(self);
        if (!value) {
            value = Py_FetchError();
            assert(value != NULL);
            schedule_promise(self, value, PROMISE_REJECTED, invoke_callback);
            Py_DECREF(value);
        } else {
            schedule_promise(self, value, PROMISE_REJECTED, invoke_callback);
        }
    }
}

/* Reject promise (callback is suppressed, the best for calling from loop callbacks) */
CAPSULE_API(PROMISE_API, void)
Promise_Reject(Promise *self, PyObject *value)
{
    Promise_RejectEx(self, value, 0);
}

CAPSULE_API(PROMISE_API, void)
Promise_RejectArgsEx(Promise *self, PyObject *exc, PyObject *args, int invoke_callback)
{
    assert(!(self->flags & PROMISE_INTERIM));
    PyObject *value = PyObject_Call(exc, args, NULL);
    Promise_RejectEx(self, value, invoke_callback);
    Py_XDECREF(value);
}

CAPSULE_API(PROMISE_API, void)
Promise_RejectArgs(Promise *self, PyObject *exc, PyObject *args)
{
    Promise_RejectArgsEx(self, exc, args, 0);
}

CAPSULE_API(PROMISE_API, void)
Promise_RejectStringEx(Promise *self, PyObject *exc, const char *msg, int invoke_callback)
{
    assert(!(self->flags & PROMISE_INTERIM));
    PyObject *value = Py_NewError(exc, msg);
    Promise_RejectEx(self, value, invoke_callback);
    Py_XDECREF(value);
}

CAPSULE_API(PROMISE_API, void)
Promise_RejectString(Promise *self, PyObject *exc, const char *msg)
{
    Promise_RejectStringEx(self, exc, msg, 0);
}

Py_LOCAL_INLINE(int)
promise_exec_async(_ctx_var, PyObject *coro)
{
    Promise *promise = Promise_New(_ctx);
    if (!promise)
        return -1;
    schedule_promise(promise, Py_None, PROMISE_FULFILLED | PROMISE_VALUABLE, 1);
    PyTrack_INCREF(coro);
    promise->coro = coro;
    Py_DECREF(promise);
    return 0;
}

/*[clinic input]
promise.exec_async as promise_execasync
    coro: object(subclass_of='&PyCoro_Type')

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_execasync_impl(PyObject *module, PyObject *coro)
/*[clinic end generated code: output=df1dbca0537541fc input=f373349fc2e8c9d9]*/
{
    _CTX_set_module(module);
    promise_exec_async(_ctx, coro);
    Py_RETURN_NONE;
}

Py_LOCAL_INLINE(int)
resume_coroutine(_ctx_var, PyObject *coro, PyObject *value, int reject)
{
    _Py_IDENTIFIER(throw);

    PyObject *result;
    Py_INCREF(value);

    while (1) {
        // value leaving circuit
        if (reject) {
            result = _PyObject_CallMethodIdOneArg(coro, &PyId_throw, value);
        } else {
            PySendResult send_result = PyIter_Send(coro, value, &result);
            if (send_result == PYGEN_RETURN) {
                Py_DECREF(value);
                Py_DECREF(result);
                return 0;
            }
        }
        Py_DECREF(value);
        if (!result) {
            if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)
                || PyErr_ExceptionMatches(PyExc_SystemExit)) {
                return -1;
            }
            PyErr_WriteUnraisable(coro);
            return 0;
        }
        if (Py_TYPE(result) != S(PromiseType)) {
            Py_DECREF(result);
            value = Py_NewError(PyExc_RuntimeError, "`await` argument expected to be a promise.");
            if (!value) {
                value = Py_FetchError();
                assert(value != NULL);
            }
            reject = 1;
            continue;
        }
        Promise *promise = (Promise *) result;
        promise->flags |= PROMISE_VALUABLE;
        PyTrack_INCREF(coro);
        promise->coro = coro;
        Py_DECREF(promise);
        break;
    }

    return 0;
}

Py_LOCAL_INLINE(int)
handle_scheduled_promise(_ctx_var, Promise *promise)
{
    // It's a heart of an engine
    assert(!(promise->flags & PROMISE_RESOLVED) && (promise->flags & PROMISE_SCHEDULED));

    LOG("(%p)", promise);

    int exec_status = promise->flags & PROMISE_SCHEDULED;
    promise->flags |= PROMISE_RESOLVING;

    if (promise->flags & PROMISE_HAS_CALLBACK) {
        PyObject *value = NULL;
        PyObject *handler = (promise->flags & PROMISE_FULFILLED) ? promise->fulfilled : promise->rejected;
        if (handler) {
            if (promise->flags & PROMISE_C_CALLBACK) {
                value = ((promisecb) handler)(promise->value, (PyObject *) promise);
            } else if (promise->value == S(NoArgs)) {
                value = PyObject_CallNoArgs(handler);
            } else {
                value = PyObject_CallOneArg(handler, promise->value);
            }
            if (!value) {
                if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)
                    || PyErr_ExceptionMatches(PyExc_SystemExit)) {
                    return -1;
                }
                exec_status = PROMISE_REJECTED;
                value = Py_FetchError();
                assert(value != NULL);
            } else {
                exec_status = PROMISE_FULFILLED;
            }
        }
        if (promise->flags & PROMISE_PY_CALLBACK) {
            PyTrack_CLEAR(promise->fulfilled);
            PyTrack_CLEAR(promise->rejected);
        }
        promise->flags ^= promise->flags & PROMISE_HAS_CALLBACK;

        if (value != NULL) {
            if (Py_TYPE(value) == S(PromiseType)) {
                Promise *new_promise = (Promise *) value;
                if (new_promise == promise) {
                    // The same promise. It's bad but not fatal.
                    Py_DECREF(new_promise);
                } else if (new_promise->flags & PROMISE_RESOLVED) {
                    // Easy-peasy. Just copy exec_status and value.
                    exec_status = new_promise->flags & PROMISE_SCHEDULED;
                    PyTrack_INCREF(new_promise->value);
                    PyTrack_XSETREF(promise->value, new_promise->value);
                    Py_DECREF(new_promise);
                } else {
                    if (new_promise->coro || Py_REFCNT(new_promise) > 2) {
                        LOG("(%p) replace promise", new_promise);
                        Py_XSETREF(new_promise, Promise_Then(new_promise));
                    }
                    if (promise->coro || Py_REFCNT(promise) > 1) {
                        LOG("(%p) re-schedule promise: %p", new_promise, promise);
                        // We must re-schedule promise
                        Py_INCREF(promise);
                        S(promise_count)++;
                        LOG("promise_count=%zd", S(promise_count));
                        // Must clear the promise value as it was already set
                        PyTrack_CLEAR(promise->value);
                        Chain_APPEND(new_promise, promise);
                    } else {
                        LOG("(%p) move chain: %p", new_promise, promise);
                        // We can replace the current promise with a new one by moving the child promises only
                        Chain_MOVE(new_promise, promise);
                    }
                    Py_DECREF(new_promise);
                    return 0;
                }
            } else {
                PyTrack_XSETREF(promise->value, value);
                PyTrack_MarkEnter(value);
            }
        }
    }

    PyTrack_CLEAR(promise->ctx);

    promise->flags ^= promise->flags & PROMISE_SCHEDULED;
    promise->flags |= exec_status | PROMISE_RESOLVED;

    if (Chain_HEAD(promise)) {
        Promise *it;
        PyObject *value = promise->value;
        Chain_PULLALL(it, promise) {
            schedule_promise(it, value, exec_status, 0);
            Py_DECREF(it);
        }
    }

    if (promise->coro) {
        int ret = resume_coroutine(_ctx, promise->coro, promise->value,
                                   exec_status & PROMISE_REJECTED);
        // release coro ASAP
        PyTrack_CLEAR(promise->coro);
        return ret;
    }

    return 0;
}

CAPSULE_API(PROMISE_API, void)
Promise_ClearChain(_ctx_var)
{
    Promise *it;
    Chain_PULLALL(it, &S(promisechain)) {
        Py_DECREF(it);
    }
}

CAPSULE_API(PROMISE_API, int)
Promise_ProcessChain(_ctx_var)
{
    int ret = 0;
    if (Chain_HEAD(&S(promisechain))) {
        Promise *it;
        S(in_chain_routine) = 1;
        Chain_PULLALL(it, &S(promisechain)) {
            int err = handle_scheduled_promise(_ctx, it);
            Py_DECREF(it);
            if (err) {
                S(in_chain_routine) = 0;
                return -1;
            }
        }
        S(in_chain_routine) = 0;
        ret = 1;
    }
    if (S(promise_count)) {
        ret += 2;
    }
    return ret;
}

CAPSULE_API(PROMISE_API, int)
Promise_GetStats(_ctx_var, Py_ssize_t *active_promises)
{
    *active_promises = S(promise_count);
    return Chain_HEAD(&S(promisechain)) != NULL;
}


/*[clinic input]
promise.process_promise_chain
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_process_promise_chain_impl(PyObject *module)
/*[clinic end generated code: output=0587509f3b9441b4 input=1b727a6bcabff125]*/
{
    _CTX_set_module(module);
    int ret = Promise_ProcessChain(_ctx);
    if (ret < 0) {
        return NULL;
    }
    return PyLong_FromSsize_t(S(promise_count));
}

static void
promise_unlockloop(_ctx_var)
{
    _Py_IDENTIFIER(set);
    PyObject *ret = _PyObject_CallMethodIdNoArgs(S(await_event), &PyId_set);
    if (!ret) {
        PyErr_WriteUnraisable(S(await_event));
    } else {
        Py_DECREF(ret);
    }
}

/*[clinic input]
promise.run_forever

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_run_forever_impl(PyObject *module)
/*[clinic end generated code: output=dbc5007e1e267e20 input=a4668cb0592ac027]*/
{
    _CTX_set_module(module);
    PyObject *event = PyObject_CallNoArgs(S(EventType));
    if (!event)
        return NULL;
    if (Promise_StartLoop(_ctx, (unlockloop) promise_unlockloop, _ctx)) {
        Py_DECREF(event);
        return NULL;
    }
    S(await_event) = event;
    _Py_IDENTIFIER(wait);
    _Py_IDENTIFIER(clear);
    while (1) {
        Py_ssize_t process_result = Promise_ProcessChain(_ctx);
        if (process_result <= 0)
            break;
        PyObject *ret = _PyObject_CallMethodIdNoArgs(event, &PyId_wait);
        if (!ret)
            break;
        Py_DECREF(ret);
        ret = _PyObject_CallMethodIdNoArgs(event, &PyId_clear);
        if (!ret)
            break;
        Py_DECREF(ret);
    }
    Py_CLEAR(S(await_event));
    Promise_ClearChain(_ctx);   // TODO: maybe I should not clear the chain?
    Promise_StopLoop(_ctx, (unlockloop) promise_unlockloop, _ctx);
    if (PyErr_Occurred())
        return NULL;

    // In the case when promises remain in memory, they can be fulfilled later,
    // which can lead to unsuspected consequences.

    Py_RETURN_NONE;
}

PyDoc_STRVAR(promiseiter_doc, "Iterator for async magic");

static PyObject *
promiseiter_new(Promise *promise)
{
    _CTX_set(promise);
    Promiseiter *it = (Promiseiter *) Freelist_GC_New(PromiseiterType);
    if (!it)
        return NULL;
    PyTrack_MarkAllocated(it);
    PyObject_GC_Track(it);
    _CTX_save(it);
    if (promise->coro || promise->flags & PROMISE_RESOLVED) {
        it->promise = Promise_Then(promise);
        if (it->promise == NULL) {
            Py_DECREF(it);
            return NULL;
        }
    } else {
        Py_INCREF(promise);
        it->promise = promise;
    }
    return (PyObject *) it;
}

static int
promiseiter_traverse(Promiseiter *self, visitproc visit, void *arg)
{
    Py_VISIT(Py_TYPE(self));
    Py_VISIT(self->promise);
    return 0;
}

static int
promiseiter_clear(Promiseiter *self)
{
    Py_CLEAR(self->promise);
    return 0;
}

static void
promiseiter_dealloc(Promiseiter *self)
{
    _CTX_set(self);
    PyTrack_MarkDeleted(self);
    PyObject_GC_UnTrack(self);
    promiseiter_clear(self);
    Freelist_GC_Delete(PromiseiterType, self);
}

static PyObject *
promiseiter_repr(PyObject *self)
{
    return PyUnicode_FromFormat("<promiseiter object at %p>", self);
}

static PyObject *
promiseiter_iternext(Promiseiter *self)
{
    PyObject *res = (PyObject *) (self->promise);
    self->promise = NULL;
    return res;
}

static PyObject *
promiseiter_send(Promiseiter *self, PyObject *value)
{
    _PyGen_SetStopIterationValue(value);
    return NULL;
}

static PyMethodDef promiseiter_methods[] = {
    {"send", (PyCFunction) promiseiter_send, METH_O, NULL},
    {NULL, NULL},
};


static PyType_Slot promiseiter_slots[] = {
    {Py_tp_doc, (char *) promiseiter_doc},
    {Py_tp_methods, promiseiter_methods},
    {Py_tp_dealloc, promiseiter_dealloc},
    {Py_tp_traverse, promiseiter_traverse},
    {Py_tp_clear, promiseiter_clear},
    {Py_tp_repr, promiseiter_repr},
    {Py_tp_iter, PyObject_SelfIter},
    {Py_tp_iternext, promiseiter_iternext},
    {0, 0},
};

static PyType_Spec promiseiter_spec = {
    "promisedio.promise.Promiseiter",
    sizeof(Promiseiter),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_IMMUTABLETYPE,
    promiseiter_slots,
};

PyDoc_STRVAR(promise_doc, "The Promise object represents the eventual completion (or failure) of "
                          "an asynchronous operation and its resulting value.\n");

static PyMethodDef promise_methods[] = {
    PROMISE_PROMISE_THEN_METHODDEF
    PROMISE_PROMISE_CATCH_METHODDEF
    {NULL} /* Sentinel */
};

/*[clinic input]
class promise.Promise "Promise *" "&PromiseType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=1cf6b3558b6d3efd]*/

/*[clinic input]
promise.Promise.then
    fulfilled: object = None
    rejected: object = None
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Promise_then_impl(Promise *self, PyObject *fulfilled,
                          PyObject *rejected)
/*[clinic end generated code: output=9573450632b756bf input=d1643be0516cbaa7]*/
{
    fulfilled = (fulfilled == Py_None ? NULL : fulfilled);
    if (fulfilled && !PyCallable_Check(fulfilled)) {
        PyErr_SetString(PyExc_TypeError, "`fulfilled` argument must be a callable");
        return NULL;
    }
    rejected = (rejected == Py_None ? NULL : rejected);
    if (rejected && !PyCallable_Check(rejected)) {
        PyErr_SetString(PyExc_TypeError, "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = Promise_Then(self);
    if (promise) {
        promise->flags |= PROMISE_PY_CALLBACK;
        PyTrack_XINCREF(fulfilled);
        PyTrack_XINCREF(rejected);
        promise->fulfilled = fulfilled;
        promise->rejected = rejected;
    }
    return (PyObject *) promise;
}

/*[clinic input]
promise.Promise.catch
    rejected: object
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Promise_catch_impl(Promise *self, PyObject *rejected)
/*[clinic end generated code: output=10c9938cc577180f input=29eda98237fd3448]*/
{
    if (!PyCallable_Check(rejected)) {
        PyErr_SetString(PyExc_TypeError, "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = Promise_Then(self);
    if (promise) {
        promise->flags |= PROMISE_PY_CALLBACK;
        PyTrack_INCREF(rejected);
        promise->rejected = rejected;
    }
    return (PyObject *) promise;
}

static int
promise_traverse(Promise *self, visitproc visit, void *arg)
{
    Py_VISIT(Py_TYPE(self));
    Py_VISIT(Chain_HEAD(self));
    Py_VISIT(Chain_NEXT(self));
    Py_VISIT(self->value);
    Py_VISIT(self->ctx);
    // coro, fulfilled and rejected normally should be NULL
    Py_VISIT(self->coro);
    if (self->flags & PROMISE_PY_CALLBACK) {
        Py_VISIT(self->fulfilled);
        Py_VISIT(self->rejected);
    }
    return 0;
}

static int
promise_clear(Promise *self)
{
    Py_CLEAR(Chain_HEAD(self));
    Py_CLEAR(Chain_NEXT(self));
    PyTrack_CLEAR(self->value);
    PyTrack_CLEAR(self->ctx);
    // coro, context, fulfilled and rejected normally should be NULL
    PyTrack_CLEAR(self->coro);
    if (self->flags & PROMISE_PY_CALLBACK) {
        PyTrack_CLEAR(self->fulfilled);
        PyTrack_CLEAR(self->rejected);
    }
    return 0;
}

static void
promise_finalize(Promise *self)
{
    LOG("%p", self);
    if (!self->coro) {
        return;
    }

    // We're only here for coroutine
    PyObject *error_type, *error_value, *error_traceback;
    PyErr_Fetch(&error_type, &error_value, &error_traceback);

    _CTX_set(self);
    PySys_FormatStderr("Exception ignored in: %S\n", self->coro);
    PySys_WriteStderr("Traceback (most recent call last):\n");
    PyObject *result = _PyObject_CallOneArg(S(print_stack), (PyObject *) ((PyCoroObject *) self->coro)->cr_frame);
    if (!result) {
        PyErr_WriteUnraisable(self->coro);
    }
    PySys_WriteStderr("RuntimeError: a coroutine awaits a promise that will never be fulfilled\n");
    Py_XDECREF(result);

    PyErr_Restore(error_type, error_value, error_traceback);
}

static void
promise_dealloc(Promise *self)
{
    _CTX_set(self);
    if ((self->flags & PROMISE_REJECTED) && (!(self->flags & PROMISE_VALUABLE))) {
        print_unhandled_exception_from_dealloc(self->value);
    }

    if (self->coro) {
        LOG("coro %p, me %p", self->coro, self);
        if (PyObject_CallFinalizerFromDealloc((PyObject *) self) < 0) {
            // resurrected.
            return;
        }
    }

    if (!(self->flags & PROMISE_SCHEDULED)) {
        S(promise_count)--;
        LOG("promise_count=%zd", S(promise_count));
    }

    PyTrack_MarkDeleted(self);
    PyObject_GC_UnTrack(self);
    promise_clear(self);
    Freelist_GC_Delete(PromiseType, self);
}

static PyObject *
promise_repr(PyObject *self)
{
    unsigned int flags = ((Promise *) self)->flags;
    if (flags & PROMISE_RESOLVED) {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat("<Promise object at %p | FULFILLED | RESOLVED (%R)>", self,
                                        ((Promise *) self)->value);
        } else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat("<Promise object at %p | REJECTED | RESOLVED (%R)>", self,
                                        ((Promise *) self)->value);
        }
    } else if (flags & PROMISE_RESOLVING) {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat("<Promise object at %p | FULFILLED | RESOLVING (%R)>", self,
                                        ((Promise *) self)->value);
        } else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat("<Promise object at %p | REJECTED | RESOLVING (%R)>", self,
                                        ((Promise *) self)->value);
        }
    } else {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat("<Promise object at %p | FULFILLED | SCHEDULED (%R)>", self,
                                        ((Promise *) self)->value);
        } else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat("<Promise object at %p | REJECTED | SCHEDULED (%R)>", self,
                                        ((Promise *) self)->value);
        }
    }
    return PyUnicode_FromFormat("<Promise object at %p | PENDING>", self);
}

static PyType_Slot promise_slots[] = {
    {Py_tp_doc, (char *) promise_doc},
    {Py_tp_methods, promise_methods},
    {Py_tp_dealloc, promise_dealloc},
    {Py_tp_finalize, promise_finalize},
    {Py_tp_traverse, promise_traverse},
    {Py_tp_clear, promise_clear},
    {Py_tp_repr, promise_repr},
    {Py_am_await, promiseiter_new},
    {0, 0},
};

static PyType_Spec promise_spec = {
    "promisedio.promise.Promise",
    sizeof(Promise),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_IMMUTABLETYPE,
    promise_slots,
};

PyDoc_STRVAR(deferred_doc, "A Deferred object is used to provide a new promise "
                           "along with methods to change its state.\n");

static PyMethodDef deferred_methods[] = {
    PROMISE_DEFERRED_RESOLVE_METHODDEF
    PROMISE_DEFERRED_REJECT_METHODDEF
    PROMISE_DEFERRED_PROMISE_METHODDEF
    {NULL} /* Sentinel */
};

/*[clinic input]
promise.deferred
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_deferred_impl(PyObject *module)
/*[clinic end generated code: output=4760b84ba7d768a3 input=4303141f412f77d4]*/
{
    _CTX_set_module(module);
    Promise *promise = Promise_New(_ctx);
    if (!promise)
        return NULL;
    Deferred *self = (Deferred *) Freelist_GC_New(DeferredType);
    if (!self) {
        Py_DECREF(promise);
        return NULL;
    }
    _CTX_save(self);
    self->promise = promise;
    PyTrack_MarkAllocated(self);
    PyObject_GC_Track(self);
    return (PyObject *) self;
}

/*[clinic input]
class promise.Deferred "Deferred *" "&DeferredType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=0a0fbb23a0253343]*/

/*[clinic input]
promise.Deferred.resolve
    value: object
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_resolve_impl(Deferred *self, PyObject *value)
/*[clinic end generated code: output=f055b923264841e2 input=91fdbc261cc8f9a6]*/
{
    Promise_ResolveEx(self->promise, value, 1);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.reject
    value: object
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_reject_impl(Deferred *self, PyObject *value)
/*[clinic end generated code: output=65ffe03c0340f128 input=db4da272b9a4e4e8]*/
{
    if (!PyExceptionClass_Check(value) && !PyExceptionInstance_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "exceptions must be classes deriving BaseException or "
                                         "instances of such a class");
        return NULL;
    }
    Promise_RejectEx(self->promise, value, 1);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.promise
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_promise_impl(Deferred *self)
/*[clinic end generated code: output=77601f45478590bb input=9210627b1b2b5b68]*/
{
    Py_INCREF(self->promise);
    return (PyObject *) self->promise;
}

static int
deferred_traverse(Deferred *self, visitproc visit, void *arg)
{
    Py_VISIT(Py_TYPE(self));
    Py_VISIT(self->promise);
    return 0;
}

static int
deferred_clear(Deferred *self)
{
    Py_CLEAR(self->promise);
    return 0;
}

static void
deferred_dealloc(Deferred *self)
{
    _CTX_set(self);
    PyTrack_MarkDeleted(self);
    PyObject_GC_UnTrack(self);
    deferred_clear(self);
    Freelist_GC_Delete(DeferredType, self);
}

static PyObject *
deferred_repr(PyObject *self)
{
    return PyUnicode_FromFormat("<Deferred object at %p>", self);
}

static PyType_Slot deferred_slots[] = {
    {Py_tp_doc, (char *) deferred_doc},
    {Py_tp_methods, deferred_methods},
    {Py_tp_dealloc, deferred_dealloc},
    {Py_tp_traverse, deferred_traverse},
    {Py_tp_clear, deferred_clear},
    {Py_tp_repr, deferred_repr},
    {0, 0},
};

static PyType_Spec deferred_spec = {
    "promisedio.promise.Deferred",
    sizeof(Deferred),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_IMMUTABLETYPE,
    deferred_slots,
};


/*[clinic input]
class promise.Lock "Lock *" "_CTX_get_type(type)->LockType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=dc5e70ca7a8c9155]*/

CAPSULE_API(PROMISE_API, Lock *)
Lock_New(_ctx_var)
{
    return (Lock *) promise_Lock_impl(S(LockType));
}

CAPSULE_API(PROMISE_API, Promise *)
Lock_Acquire(Lock *self)
{
    _CTX_set(self);
    Promise *promise = Promise_New(_ctx);
    if (promise) {
        if (self->locked) {
            Py_INCREF(self);
            Py_INCREF(promise);
            Chain_APPEND(self, promise);
        } else {
            schedule_promise(promise, Py_None, PROMISE_FULFILLED, 1);
        }
        self->locked++;
    }
    return promise;
}

CAPSULE_API(PROMISE_API, void)
Lock_Release(Lock *self)
{
    if (self->locked) {
        _CTX_set(self);
        Promise *it;
        Chain_PULLALL(it, self) {
            self->locked--;
            schedule_promise(it, Py_None, PROMISE_FULFILLED, 1);
            Py_DECREF(it);
            Py_DECREF(self);
            break;
        }
    }
}

/*[clinic input]
@classmethod
promise.Lock.__new__

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Lock_impl(PyTypeObject *type)
/*[clinic end generated code: output=d9149ff17417bda6 input=6c9fb50af8d29a9f]*/
{
    Lock *self = (Lock *) Py_New(type);
    if (!self)
        return NULL;

    _CTX_set_type(type);
    self->locked = 0;
    _CTX_save(self);
    Chain_INIT(self);
    PyTrack_MarkAllocated(self);
    return (PyObject *) self;
}

/*[clinic input]
promise.Lock.acquire

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Lock_acquire_impl(Lock *self)
/*[clinic end generated code: output=20d044ffec9c1f84 input=1207c4f90d9bdb82]*/
{
    return (PyObject *) Lock_Acquire(self);
}

/*[clinic input]
promise.Lock.release

[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Lock_release_impl(Lock *self)
/*[clinic end generated code: output=b1c748e07809746e input=6896117a2c29291e]*/
{
    Lock_Release(self);
    Py_RETURN_NONE;
}

static void
lock_dealloc(Lock *self)
{
    Py_Delete(self);
}

static PyMethodDef lock_methods[] = {
    PROMISE_LOCK_ACQUIRE_METHODDEF
    PROMISE_LOCK_RELEASE_METHODDEF
    {NULL} /* Sentinel */
};

static PyType_Slot lock_slots[] = {
    {Py_tp_new, promise_Lock},
    {Py_tp_methods, lock_methods},
    {Py_tp_dealloc, lock_dealloc},
    {0, 0},
};

static PyType_Spec lock_spec = {
    "promisedio.promise.Lock",
    sizeof(Lock),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    lock_slots,
};

static PyType_Slot nullvalue_slots[] = {
    {0, 0},
};

static PyType_Spec nullvalue_spec = {
    "promisedio.promise.NoArgs",
    sizeof(NoArgs),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    nullvalue_slots
};

static PyMethodDef promisemodule_methods[] = {
    PROMISE_CLEARFREELISTS_METHODDEF
    PROMISE_SETFREELISTLIMITS_METHODDEF
    PROMISE_DEFERRED_METHODDEF
    PROMISE_EXECASYNC_METHODDEF
    PROMISE_PROCESS_PROMISE_CHAIN_METHODDEF
    PROMISE_RUN_FOREVER_METHODDEF
    {NULL, NULL},
};

static int
promisemodule_init(PyObject *module)
{
    LOG("(%p)", module);
    _CTX_set_module(module);
    Freelist_GC_INIT(PromiseType, 1024);
    Freelist_GC_INIT(DeferredType, 1024);
    Freelist_GC_INIT(PromiseiterType, 1024);
    Chain_INIT(&(S(promisechain)));
    S(in_chain_routine) = 0;
    S(unlockloop_func) = NULL;
    S(unlockloop_ctx) = NULL;
    S(await_event) = NULL;
    S(promise_count) = 0;
    S(PromiseType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &promise_spec, NULL);
    if (S(PromiseType) == NULL)
        return -1;
    S(DeferredType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &deferred_spec, NULL);
    if (S(DeferredType) == NULL)
        return -1;
    S(PromiseiterType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &promiseiter_spec, NULL);
    if (S(PromiseiterType) == NULL)
        return -1;
    S(LockType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &lock_spec, NULL);
    if (S(LockType) == NULL)
        return -1;
    S(NoArgs) = PyType_FromModuleAndSpec(module, &nullvalue_spec, NULL);
    if (S(NoArgs) == NULL)
        return -1;
    int err = -1;
    PyObject *threading = PyImport_ImportModule("threading");
    if (!threading)
        goto finally;
    S(EventType) = PyObject_GetAttrString(threading, "Event");
    if (S(EventType) == NULL)
        goto finally;
    PyObject *d = PyModule_GetDict(module);
    if (PyDict_SetItemString(d, "Promise", (PyObject *) S(PromiseType)) < 0)
        goto finally;
    if (PyDict_SetItemString(d, "Lock", (PyObject *) S(LockType)) < 0)
        goto finally;
    PyObject *traceback = PyImport_ImportModule("traceback");
    if (!traceback)
        goto finally;
    S(print_stack) = PyObject_GetAttrString(traceback, "print_stack");
    if (S(print_stack) == NULL)
        goto finally;
    err = 0;
finally:
    Py_XDECREF(threading);
    return err;
}

#include "capsule.h"

static int
promisemodule_create_api(PyObject *module)
{
    LOG("(%p)", module);
    Capsule_CREATE(module, PROMISE_API);
    return 0;
}

static int
promisemodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _CTX_set_module(module);
    Py_VISIT(S(PromiseType));
    Py_VISIT(S(DeferredType));
    Py_VISIT(S(PromiseiterType));
    Py_VISIT(S(LockType));
    Py_VISIT(S(EventType));
    Py_VISIT(S(NoArgs));
    Py_VISIT(S(print_stack));
    Py_VISIT(Chain_HEAD(&S(promisechain)));
    return 0;
}

static int
promisemodule_clear(PyObject *module)
{
    _CTX_set_module(module);
    Py_CLEAR(S(PromiseType));
    Py_CLEAR(S(DeferredType));
    Py_CLEAR(S(PromiseiterType));
    Py_CLEAR(S(LockType));
    Py_CLEAR(S(EventType));
    Py_CLEAR(S(NoArgs));
    Py_CLEAR(S(print_stack));
    Promise_ClearChain(_ctx);
    return 0;
}

static void
promisemodule_free(void *module)
{
    LOG("(%p)", module);
    _CTX_set_module(module);
    Freelist_GC_Clear(PromiseType);
    Freelist_GC_Clear(PromiseiterType);
    Freelist_GC_Clear(DeferredType);
    promisemodule_clear((PyObject *) module);
}

static PyModuleDef_Slot promisemodule_slots[] = {
    {Py_mod_exec, promisemodule_init},
    {Py_mod_exec, promisemodule_create_api},
    {0, NULL},
};

static struct PyModuleDef promisemodule_def = {
    PyModuleDef_HEAD_INIT,
    "promisedio.promise",
    NULL,
    sizeof(_modulestate),
    promisemodule_methods,
    promisemodule_slots,
    promisemodule_traverse,
    promisemodule_clear,
    promisemodule_free,
};

PyMODINIT_FUNC
PyInit_promise(void)
{
    return PyModuleDef_Init(&promisemodule_def);
}
