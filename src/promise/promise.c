// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "promise.h"
#include "core/base.h"
#include "core/chain.h"
#include "core/memory.h"
#include "core/module.h"

typedef struct promise_s Promise;

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
    /* Freelists */
    Freelist_GC_HEAD(PromiseType)
    Freelist_GC_HEAD(DeferredType)
    Freelist_GC_HEAD(PromiseiterType)
} _modulestate;

struct promise_s {
    PyObject_HEAD
    Chain_NODE(Promise)
    PyObject *fulfilled;
    PyObject *rejected;
    PyObject *coro;
    PyObject *value;
    PyObject *ctx;
    char data[32];
    int flags;
    Chain_ROOT(Promise)
    _STATE_var;
};

typedef struct {
    PyObject_HEAD
    Promise *promise;
    _STATE_var;
} Deferred;

typedef struct {
    PyObject_HEAD
    Promise *promise;
    _STATE_var;
} Promiseiter;

#include "clinic/promise.c.h"

enum {
    PROMISE_INITIAL = 0x0000,
    PROMISE_FULFILLED = 0x0001,
    PROMISE_REJECTED = 0x0002,
    PROMISE_RESOLVING = 0x0004,
    PROMISE_RESOLVED = 0x0008,
    PROMISE_INTERIM = 0x0010,
    PROMISE_C_CALLBACK = 0x0020,
    PROMISE_PY_CALLBACK = 0x0040,
    PROMISE_VALUABLE = 0x0100
};

enum {
    PROMISE_HAS_CALLBACK = (PROMISE_C_CALLBACK | PROMISE_PY_CALLBACK),
    PROMISE_SCHEDULED = (PROMISE_FULFILLED | PROMISE_REJECTED),
    PROMISE_FREEZED = (PROMISE_RESOLVING | PROMISE_RESOLVED)
};

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
    _STATE_setmodule(module)

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
    _STATE_setmodule(module)

    if (promise_limit >= 0) {
        Freelist_GC_SetLimit(PromiseType, promise_limit);
    }
    if (promiseiter_limit >= 0) {
        Freelist_GC_SetLimit(PromiseiterType, promiseiter_limit);
    }
    if (deferred_limit >= 0) {
        Freelist_GC_SetLimit(DeferredType, deferred_limit);
    }
    Py_RETURN_NONE;
}

static void
print_unhandled_exception_from_dealloc(PyObject *value)
{
    PySys_WriteStderr("Unhandled promise rejection:\n");
    if (value) {
        PyObject *error_type, *error_value, *error_traceback;
        PyErr_Fetch(&error_type, &error_value, &error_traceback);
        PyObject *exc = PyExceptionInstance_Class(value);
        PyObject *tb = PyException_GetTraceback(value);
        if (tb == NULL) {
            tb = Py_NewRef(Py_None);
        }
        PyErr_Display(exc, value, tb);
        PySys_WriteStderr("\n");
        PyException_SetTraceback(value, Py_None);
        Py_DECREF(tb);
        PyErr_Restore(error_type, error_value, error_traceback);
    } else {
        PySys_WriteStderr("lost exception value\n");
    }
}

static Promise *
Promise_New(_STATE_var)
{
    Promise *self = (Promise *) Freelist_GC_New(PromiseType);
    if (self == NULL)
        return NULL;
    Chain_INIT(self);
    Chain_NODEINIT(self);
    self->value = NULL;
    self->coro = NULL;
    self->flags = PROMISE_INITIAL;
    self->fulfilled = NULL;
    self->rejected = NULL;
    self->ctx = NULL;
    _STATE_save(self);
    PyObject_GC_Track(self);
    return self;
}

static void
Promise_Callback(Promise *self, promisecb fulfilled, promisecb rejected)
{
    assert(!(self->flags & (PROMISE_HAS_CALLBACK | PROMISE_FREEZED)));
    self->flags |= PROMISE_C_CALLBACK;
    self->fulfilled = (PyObject *) fulfilled;
    self->rejected = (PyObject *) rejected;
}

static Promise *
Promise_Then(Promise *self)
{
    _STATE_set(self)

    Promise *promise = Promise_New(_state);
    if (promise == NULL)
        return NULL;
    self->flags |= PROMISE_VALUABLE;
    if (self->flags & PROMISE_RESOLVED) {
        promise->value = Py_NewRef(self->value);
        promise->flags |= self->flags & PROMISE_SCHEDULED;
        Chain_APPEND(&S(promisechain), promise);
    } else {
        Chain_APPEND(self, promise);
    }
    promise->flags |= PROMISE_INTERIM;
    return (Promise *) Py_NewRef(promise);
}

Py_LOCAL_INLINE(void)
schedule_promise(Promise *self, PyObject *value, int flag)
{
    _STATE_set(self)

    self->value = Py_NewRef(value);
    self->flags |= flag;
    LOG("schedule_promise(%p)", self);
    Py_INCREF(self);
    Chain_APPEND(&S(promisechain), self);
}

static Promise *
Promise_NewResolved(_STATE_var, PyObject *value)
{
    Promise *promise = Promise_New(_state);
    if (promise) {
        if (value == Py_None) {
            schedule_promise(promise, Py_None, PROMISE_FULFILLED);
        } else {
            schedule_promise(promise, value, PROMISE_FULFILLED);
            Py_DECREF(value);
        }
        return promise;
    }
    return NULL;
}

static int
Promise_IsResolved(Promise *self)
{
    return !!(self->flags & PROMISE_SCHEDULED);
}

static void
Promise_Resolve(Promise *self, PyObject *value)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED))
        schedule_promise(self, value, PROMISE_FULFILLED);
}

static void
Promise_Reject(Promise *self, PyObject *value)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED))
        schedule_promise(self, value, PROMISE_REJECTED);
}


static void
Promise_RejectPyExc(Promise *self)
{
    PyObject *exc = fetch_current_exception();
    Promise_Reject(self, exc);
    Py_XDECREF(exc);
}

static void
Promise_RejectArgs(Promise *self, PyObject *exc, PyObject *args)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED)) {
        PyObject *value = PyObject_Call(exc, args, NULL);
        if (value == NULL)
            return Promise_RejectPyExc(self);
        schedule_promise(self, value, PROMISE_REJECTED);
        Py_DECREF(value);
    }
}

static void
Promise_RejectString(Promise *self, PyObject *exc, const char *msg)
{
    assert(!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED)) {
        PyObject *value = new_exception(exc, msg);
        if (value == NULL)
            return Promise_RejectPyExc(self);
        schedule_promise(self, value, PROMISE_REJECTED);
        Py_DECREF(value);
    }
}

static void *
Promise_Data(Promise *self)
{
    return &(self->data);
}

static PyObject *
Promise_GetCtx(Promise *self)
{
    return self->ctx;
}

static PyObject *
Promise_SetCtx(Promise *self, PyObject *ctx)
{
    PyObject *ret = self->ctx;
    self->ctx = ctx;
    return ret;
}

static int
promise_exec_async(_STATE_var, PyObject *coro)
{
    Promise *promise = Promise_New(_state);
    if (promise == NULL) {
        return -1;
    }
    schedule_promise(promise, Py_None, PROMISE_FULFILLED | PROMISE_VALUABLE);
    promise->coro = Py_NewRef(coro);
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
    _STATE_setmodule(module)

    promise_exec_async(_state, coro);
    Py_RETURN_NONE;
}

static int
resume_coroutine(_STATE_var, PyObject *coro, PyObject *value, int reject)
{
    _Py_IDENTIFIER(throw);

    PyObject *result;
    Py_INCREF(value);

    while (1) {
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

        if (result == NULL) {
            if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)
                || PyErr_ExceptionMatches(PyExc_SystemExit)) {
                return -1;
            }
            promise_print_unhandled_exception();
            return 0;
        }

        if (Py_TYPE(result) != S(PromiseType)) {
            Py_DECREF(result);
            value = new_exception(PyExc_RuntimeError, "`await` argument expected to be a promise.");
            if (value == NULL) {
                value = fetch_current_exception();
            }
            reject = 1;
            continue;
        }

        ((Promise *) result)->flags |= PROMISE_VALUABLE;
        ((Promise *) result)->coro = Py_NewRef(coro);
        Py_DECREF(result);

        break;
    }

    return 0;
}

static int
handle_scheduled_promise(Promise *promise)
{
    // It's a heart of an engine
    assert(!(promise->flags & PROMISE_RESOLVED) && (promise->flags & PROMISE_SCHEDULED));

    _STATE_set(promise)

    int exec_status = promise->flags & PROMISE_SCHEDULED;
    promise->flags |= PROMISE_RESOLVING;

    if (promise->flags & PROMISE_HAS_CALLBACK) {
        PyObject *value = NULL;
        PyObject *handler
            = (promise->flags & PROMISE_FULFILLED) ? promise->fulfilled : promise->rejected;
        if (handler) {
            if (promise->flags & PROMISE_C_CALLBACK) {
                value = ((promisecb) handler)(promise->value, (PyObject *) promise);
            } else {
                value = PyObject_CallOneArg(handler, promise->value);
            }
            if (value == NULL) {
                if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)
                    || PyErr_ExceptionMatches(PyExc_SystemExit)) {
                    return -1;
                }
                exec_status = PROMISE_REJECTED;
                value = fetch_current_exception();
            } else {
                exec_status = PROMISE_FULFILLED;
            }
        }
        if (promise->flags & PROMISE_PY_CALLBACK) {
            Py_CLEAR(promise->fulfilled);
            Py_CLEAR(promise->rejected);
        }
        promise->flags ^= promise->flags & PROMISE_HAS_CALLBACK;

        if (value != NULL) {
            if (Py_TYPE(value) == S(PromiseType)) {
                Promise *new_promise = (Promise *) value;
                if (new_promise == promise) {
                    // The same promise. It's bad but not fatal.
                    // Nothing to do.
                } else if (new_promise->flags & PROMISE_RESOLVED) {
                    // Easy-peasy. Just copy exec_status and value.
                    exec_status = new_promise->flags & PROMISE_SCHEDULED;
                    value = new_promise->value;
                    Py_INCREF(value);
                    Py_XSETREF(promise->value, value);
                } else {
                    if (new_promise->coro || Py_REFCNT(new_promise) > 2) {
                        Py_XSETREF(new_promise, Promise_Then(new_promise));
                    }
                    if (promise->coro || Py_REFCNT(promise) > 1) {
                        // We must re-schedule promise
                        Py_INCREF(promise);
                        Chain_APPEND(new_promise, promise);
                    } else {
                        // We can replace current promise with the new one
                        // without consequences
                        Chain_MOVE(new_promise, promise);
                    }
                    Py_DECREF(new_promise);
                    return 0;
                }
                Py_DECREF(new_promise);
            } else {
                Py_XSETREF(promise->value, value);
            }
        }
    }

    Py_CLEAR(promise->ctx);

    promise->flags ^= promise->flags & PROMISE_SCHEDULED;
    promise->flags |= exec_status | PROMISE_RESOLVED;

    if (Chain_HEAD(promise)) {
        Promise *it;
        PyObject *value = promise->value;
        Chain_FOREACH(it, promise) {
            it->value = Py_NewRef(value);
            it->flags |= exec_status;
        }
        Chain_MOVE(&S(promisechain), promise);
    }

    if (promise->coro) {
        int ret = resume_coroutine(_state, promise->coro, promise->value,
                                   exec_status & PROMISE_REJECTED);
        // release coro ASAP
        Py_CLEAR(promise->coro);
        return ret;
    }

    return 0;
}

static void
Promise_ClearChain(_STATE_var)
{
    Promise *it;
    Chain_PULL(it, &S(promisechain)) {
        Py_DECREF(it);
    }
}

static int
Promise_ProcessChain(_STATE_var)
{
    if (Chain_HEAD(&S(promisechain))) {
        Promise *it;
        Chain_PULL(it, &S(promisechain)) {
            int err = handle_scheduled_promise(it);
            Py_DECREF(it);
            if (err) {
                return -1;
            }
        }
        return 1;
    }
    return 0;
}

/*[clinic input]
promise.process_promise_chain
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_process_promise_chain_impl(PyObject *module)
/*[clinic end generated code: output=0587509f3b9441b4 input=1b727a6bcabff125]*/
{
    _STATE_setmodule(module)

    int ret = Promise_ProcessChain(_state);
    if (ret < 0) {
        return NULL;
    }
    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyDoc_STRVAR(promiseiter_doc, "Iterator for async magic");

static PyObject *
promiseiter_send(Promiseiter *self, PyObject *value);

static PyMethodDef promiseiter_methods[] = {
    {"send", (PyCFunction) promiseiter_send, METH_O, NULL},
    {NULL, NULL},
};

static PyObject *
promiseiter_new(Promise *promise)
{
    _STATE_set(promise)

    Promiseiter *it = (Promiseiter *) Freelist_GC_New(PromiseiterType);
    if (!it)
        return NULL;
    PyObject_GC_Track(it);
    if (promise->coro || promise->flags & PROMISE_RESOLVED) {
        it->promise = Promise_Then(promise);
        if (it->promise == NULL) {
            Py_DECREF(it);
            return NULL;
        }
    } else {
        it->promise = (Promise *) Py_NewRef(promise);
    }
    _STATE_save(it);
    return (PyObject *) it;
}

static int
promiseiter_traverse(Promiseiter *self, visitproc visit, void *arg)
{
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
    _STATE_set(self)

    PyObject_GC_UnTrack(self);
    promiseiter_clear(self);
    Freelist_GC_Del(PromiseiterType, self);
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
    PROMISE_PROMISE_THEN_METHODDEF PROMISE_PROMISE_CATCH_METHODDEF {NULL} /* Sentinel */
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
        promise->fulfilled = Py_XNewRef(fulfilled);
        promise->rejected = Py_XNewRef(rejected);
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
        promise->rejected = Py_NewRef(rejected);
    }
    return (PyObject *) promise;
}

static int
promise_traverse(Promise *self, visitproc visit, void *arg)
{
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
    Py_CLEAR(self->value);
    Py_CLEAR(self->ctx);
    // coro, context, fulfilled and rejected normally should be NULL
    Py_CLEAR(self->coro);
    if (self->flags & PROMISE_PY_CALLBACK) {
        Py_CLEAR(self->fulfilled);
        Py_CLEAR(self->rejected);
    }
    return 0;
}

static void
promise_dealloc(Promise *self)
{
    _STATE_set(self)

    if ((self->flags & PROMISE_REJECTED) && (!(self->flags & PROMISE_VALUABLE))) {
        print_unhandled_exception_from_dealloc(self->value);
    }
    PyObject_GC_UnTrack(self);
    promise_clear(self);
    Freelist_GC_Del(PromiseType, self);
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
    {Py_tp_doc, (char *) promise_doc}, {Py_tp_methods, promise_methods},
    {Py_tp_dealloc, promise_dealloc}, {Py_tp_traverse, promise_traverse},
    {Py_tp_clear, promise_clear}, {Py_tp_repr, promise_repr},
    {Py_am_await, promiseiter_new}, {0, 0},
};

static PyType_Spec promise_spec = {
    "promisedio.promise.Promise",
    sizeof(Promise),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_IMMUTABLETYPE,
    promise_slots,
};

PyDoc_STRVAR(deferred_doc, "A Deferred object is used to provide a new promise "
                           "along with methods to change its _state.\n");

static PyMethodDef deferred_methods[] = {
    PROMISE_DEFERRED_RESOLVE_METHODDEF PROMISE_DEFERRED_REJECT_METHODDEF
    PROMISE_DEFERRED_PROMISE_METHODDEF {NULL} /* Sentinel */
};

/*[clinic input]
promise.deferred
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_deferred_impl(PyObject *module)
/*[clinic end generated code: output=4760b84ba7d768a3 input=4303141f412f77d4]*/
{
    _STATE_setmodule(module)

    Promise *promise = Promise_New(_state);
    if (promise == NULL) {
        return NULL;
    }

    Deferred *self = (Deferred *) Freelist_GC_New(DeferredType);
    if (self == NULL) {
        Py_DECREF(promise);
        return NULL;
    }

    self->promise = promise;
    _STATE_save(self);

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
    Promise_Resolve(self->promise, value);
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
    if (value == NULL || (!PyExceptionClass_Check(value) && !PyExceptionInstance_Check(value))) {
        PyErr_SetString(PyExc_TypeError, "exceptions must be classes deriving BaseException or "
                                         "instances of such a class");
        return NULL;
    }
    Promise_Reject(self->promise, value);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.promise
[clinic start generated code]*/

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_promise_impl(Deferred *self)
/*[clinic end generated code: output=77601f45478590bb input=9210627b1b2b5b68]*/
{
    return (PyObject *) Py_NewRef(self->promise);
}

static int
deferred_traverse(Deferred *self, visitproc visit, void *arg)
{
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
    _STATE_set(self)

    PyObject_GC_UnTrack(self);
    deferred_clear(self);
    Freelist_GC_Del(DeferredType, self);
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

static PyMethodDef promisemodule_methods[] = {
    PROMISE_CLEARFREELISTS_METHODDEF
    PROMISE_SETFREELISTLIMITS_METHODDEF
    PROMISE_DEFERRED_METHODDEF
    PROMISE_EXECASYNC_METHODDEF
    PROMISE_PROCESS_PROMISE_CHAIN_METHODDEF
    {NULL, NULL},
};

static int
promisemodule_init(PyObject *module)
{
    LOG("promisemodule_init(%p)", module);

    _STATE_setmodule(module)

    S(PromiseType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &promise_spec, NULL);
    if (S(PromiseType) == NULL)
        return -1;
    S(DeferredType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &deferred_spec, NULL);
    if (S(DeferredType) == NULL)
        return -1;
    S(PromiseiterType) = (PyTypeObject *) PyType_FromModuleAndSpec(module, &promiseiter_spec, NULL);
    if (S(PromiseiterType) == NULL)
        return -1;
    Freelist_GC_INIT(PromiseType, 1024);
    Freelist_GC_INIT(DeferredType, 1024);
    Freelist_GC_INIT(PromiseiterType, 1024);
    Chain_INIT(&(S(promisechain)));
    return 0;
}

static int
promisemodule_create_api(PyObject *module)
{
    LOG("promisemodule_create_api(%p)", module);
    static void *c_api[] = {
        [PROMISE_NEW_API_ID] = Promise_New,
        [PROMISE_THEN_API_ID] = Promise_Then,
        [PROMISE_NEWRESOLVED_API_ID] = Promise_NewResolved,
        [PROMISE_ISRESOLVED_API_ID] = Promise_IsResolved,
        [PROMISE_RESOLVE_API_ID] = Promise_Resolve,
        [PROMISE_REJECT_API_ID] = Promise_Reject,
        [PROMISE_REJECTARGS_API_ID] = Promise_RejectArgs,
        [PROMISE_REJECTSTRING_API_ID] = Promise_RejectString,
        [PROMISE_REJECTPYEXC_API_ID] = Promise_RejectPyExc,
        [PROMISE_CALLBACK_API_ID] = Promise_Callback,
        [PROMISE_CLEARCHAIN_API_ID] = Promise_ClearChain,
        [PROMISE_PROCESSCHAIN_API_ID] = Promise_ProcessChain,
        [PROMISE_DATA_API_ID] = Promise_Data,
        [PROMISE_GETCTX_API_ID] = Promise_GetCtx,
        [PROMISE_SETCTX_API_ID] = Promise_SetCtx,
    };
    Capsule_CREATE(module, PROMISE_API_ID, c_api);
    return 0;
}

static int
promisemodule_traverse(PyObject *module, visitproc visit, void *arg)
{
    _STATE_setmodule(module)

    Py_VISIT(S(PromiseType));
    Py_VISIT(S(DeferredType));
    Py_VISIT(S(PromiseiterType));
    return 0;
}

static int
promisemodule_clear(PyObject *module)
{
    _STATE_setmodule(module)

    Py_CLEAR(S(PromiseType));
    Py_CLEAR(S(DeferredType));
    Py_CLEAR(S(PromiseiterType));
    return 0;
}

static void
promisemodule_free(void *module)
{
    promisemodule_clear((PyObject *) module);
    LOG("promisemodule_free(%p)", module);
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
