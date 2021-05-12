// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <Python.h>
#include "promise.h"
#include "memory.h"
#include "clinic/promise.c.h"

#define Promise_CheckExact(obj) (Py_TYPE(obj) == &PromiseType)

#define Chain_Add(chain, chain_head, chain_tail) \
    if ((chain)->head == NULL) {                 \
        (chain)->head = chain_head;              \
    } else {                                     \
        (chain)->tail->next = chain_head;        \
    }                                            \
    (chain)->tail = chain_tail

static PyTypeObject PromiseType;
static PyTypeObject DeferredType;
static PyTypeObject PromiseiterType;

enum {
    PROMISE_INITIAL     = 0x0000,
    PROMISE_FULFILLED   = 0x0001,
    PROMISE_REJECTED    = 0x0002,
    PROMISE_RESOLVING   = 0x0004,
    PROMISE_RESOLVED    = 0x0008,
    PROMISE_INTERIM     = 0x0010,
    PROMISE_C_CALLBACK  = 0x0020,
    PROMISE_PY_CALLBACK = 0x0040,
    PROMISE_VALUABLE    = 0x0100
};

enum {
    PROMISE_HAS_CALLBACK = (PROMISE_C_CALLBACK | PROMISE_PY_CALLBACK),
    PROMISE_SCHEDULED = (PROMISE_FULFILLED | PROMISE_REJECTED),
    PROMISE_FREEZED = (PROMISE_RESOLVING | PROMISE_RESOLVED)
};

Mem_GC_FreeList(Promise);
Mem_GC_FreeList(Deferred);
Mem_GC_FreeList(Promiseiter);

static struct {
    Promise *head;
    Promise *tail;
} promise_chain = {
    .head = NULL,
    .tail = NULL
};

static PyObject *
new_exception(PyObject *exception, const char *msg)
{
    PyObject *value = PyUnicode_FromString(msg);
    if (value == NULL) {
        return NULL;
    }
    PyObject *exc = PyObject_CallOneArg(exception, value);
    Py_DECREF(value);
    return exc;
}

static PyObject *
fetch_current_exception()
{
    PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    if (exc == NULL)
        return NULL;
    PyErr_NormalizeException(&exc, &val, &tb);
    if (tb != NULL) {
        PyException_SetTraceback(val, tb);
        Py_DECREF(tb);
    }
    Py_DECREF(exc);
    return val;
}

static void
print_unhandled_exception()
{
    PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    if (exc == NULL) {
        PySys_WriteStderr("lost exception value\n");
        return;
    }
    PyErr_NormalizeException(&exc, &val, &tb);
    if (tb == NULL) {
        tb = Py_None;
        Py_INCREF(tb);
    }
    PyException_SetTraceback(val, tb);
    PyErr_Display(exc, val, tb);
    PyException_SetTraceback(val, Py_None);
    Py_DECREF(exc);
    Py_DECREF(val);
    Py_DECREF(tb);
    PySys_WriteStderr("\n");
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
            tb = Py_None;
            Py_INCREF(tb);
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

Promise *
Promise_New()
{
    Promise *promise = Mem_GC_New(Promise, &PromiseType);
    if (promise == NULL)
        return NULL;
    promise->head = NULL;
    promise->next = NULL;
    promise->value = NULL;
    promise->coro = NULL;
    promise->flags = PROMISE_INITIAL;
    promise->fulfilled = NULL;
    promise->rejected = NULL;
    promise->finally = NULL;
    promise->context = NULL;
    PyObject_GC_Track(promise);
    return promise;
}

void
Promise_Callback(Promise *self, promisecallback fulfilled, promisecallback rejected, PyObject *context)
{
    assert (!(self->flags & (PROMISE_HAS_CALLBACK | PROMISE_FREEZED)));
    assert (!(self->finally));
    assert (fulfilled || rejected);
    self->flags |= PROMISE_C_CALLBACK;
    self->fulfilled = (PyObject *) fulfilled;
    self->rejected = (PyObject *) rejected;
    self->context = context;
}

Promise *
Promise_Then(Promise *self)
{
    Promise *promise = Promise_New();
    if (promise == NULL)
        return NULL;
    self->flags |= PROMISE_VALUABLE;
    if (self->flags & PROMISE_RESOLVED) {
        Py_INCREF(self->value);
        promise->value = self->value;
        promise->flags |= self->flags & PROMISE_SCHEDULED;
        Chain_Add(&promise_chain, promise, promise);
    } else {
        Chain_Add(self, promise, promise);
    }
    promise->flags |= PROMISE_INTERIM;
    Py_INCREF(promise);
    return promise;
}

static inline void
schedule_promise(Promise *self, PyObject *value, unsigned int flag)
{
    Py_INCREF(value);
    Py_INCREF(self);
    self->value = value;
    self->flags |= flag;
    Chain_Add(&promise_chain, self, self);
}

Promise *
Promise_NewResolved(PyObject *value)
{
    Promise *promise = Promise_New();
    if (promise != NULL) {
        if (value == NULL) {
            schedule_promise(promise, Py_None, PROMISE_FULFILLED);
        } else {
            schedule_promise(promise, value, PROMISE_FULFILLED);
        }
    }
    Py_XDECREF(value);
    return promise;
}

int
Promise_Resolve(Promise *self, PyObject *value)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "value must be an object");
        return -1;
    }
    if (self->flags & PROMISE_INTERIM) {
        PyErr_SetString(PyExc_RuntimeError,
                        "the fulfillment of an interim promise is not allowed");
        return -1;
    }
    if (self->flags & PROMISE_SCHEDULED)
        return 0;  // ignore

    schedule_promise(self, value, PROMISE_FULFILLED);
    return 0;
}

int
Promise_Reject(Promise *self, PyObject *value)
{
    if (value == NULL || (!PyExceptionClass_Check(value) && !PyExceptionInstance_Check(value))) {
        PyErr_SetString(PyExc_TypeError,
                        "exceptions must be classes deriving BaseException or "
                        "instances of such a class");
        return -1;
    }
    if (self->flags & PROMISE_INTERIM) {
        PyErr_SetString(PyExc_RuntimeError,
                        "the fulfillment of an interim promise is not allowed");
        return -1;
    }
    if (self->flags & PROMISE_SCHEDULED)
        return 0;  // ignore

    schedule_promise(self, value, PROMISE_REJECTED);
    return 0;
}

int
Promise_RejectWithPyErr(Promise *self)
{
    PyObject *exc = fetch_current_exception();
    int res = Promise_Reject(self, exc);
    Py_XDECREF(exc);
    return res;
}

static int
promise_traverse(Promise *self, visitproc visit, void *arg)
{
    Py_VISIT(self->next);
    Py_VISIT(self->head);
    Py_VISIT(self->value);
    Py_VISIT(self->finally);
    // coro, context, fulfilled and rejected normally should be NULL
    Py_VISIT(self->coro);
    Py_VISIT(self->context);
    if (self->flags & PROMISE_PY_CALLBACK) {
        Py_VISIT(self->fulfilled);
        Py_VISIT(self->rejected);
    }
    return 0;
}

static int
promise_clear(Promise *self)
{
    Py_CLEAR(self->next);
    Py_CLEAR(self->head);
    Py_CLEAR(self->value);
    Py_CLEAR(self->finally);
    // coro, context, fulfilled and rejected normally should be NULL
    Py_CLEAR(self->coro);
    Py_CLEAR(self->context);
    if (self->flags & PROMISE_PY_CALLBACK) {
        Py_CLEAR(self->fulfilled);
        Py_CLEAR(self->rejected);
    }
    return 0;
}

static void
promise_dealloc(Promise *self)
{
    if ((self->flags & PROMISE_REJECTED) && (!(self->flags & PROMISE_VALUABLE))) {
        print_unhandled_exception_from_dealloc(self->value);
    }
    PyObject_GC_UnTrack(self);
    promise_clear(self);
    Mem_GC_Del(Promise, self);
}

static PyObject*
promise_repr(PyObject* self)
{
    unsigned int flags = ((Promise *) self)->flags;
    if (flags & PROMISE_RESOLVED) {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | FULFILLED | RESOLVED (%R)>",
                self,
                ((Promise *) self)->value
            );
        } else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | REJECTED | RESOLVED (%R)>",
                self,
                ((Promise *) self)->value
            );
        }
    } else if (flags & PROMISE_RESOLVING) {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | FULFILLED | RESOLVING (%R)>",
                self,
                ((Promise *) self)->value
            );
        } else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | REJECTED | RESOLVING (%R)>",
                self,
                ((Promise *) self)->value
            );
        }
    } else {
        if (flags & PROMISE_FULFILLED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | FULFILLED | SCHEDULED (%R)>",
                self,
                ((Promise *) self)->value
            );
        }
        else if (flags & PROMISE_REJECTED) {
            return PyUnicode_FromFormat(
                "<Promise object at %p | REJECTED | SCHEDULED (%R)>",
                self,
                ((Promise *) self)->value
            );
        }
    }
    return PyUnicode_FromFormat("<Promise object at %p | PENDING>", self);
}

/*[clinic input]
module promise
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=c425861e022a97bb]*/

/*[clinic input]
class promise.Promise "promise *" "&PromiseType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=ce758fec03358926]*/

/*[clinic input]
promise.Promise.then

    self: self(type="Promise *")
    fulfilled: object = None
        success callback
    rejected: object = None
        failure callback
    /

The then() method returns a new Promise.

It takes up to two arguments:
callback functions for the success and failure cases of the Promise.
[clinic start generated code]*/

static PyObject *
promise_Promise_then_impl(Promise *self, PyObject *fulfilled,
                          PyObject *rejected)
/*[clinic end generated code: output=05ba8f14e75a37d1 input=cde2596bf9a47acc]*/
{
    fulfilled = (fulfilled == Py_None ? NULL: fulfilled);
    if (fulfilled && !PyCallable_Check(fulfilled)) {
        PyErr_SetString(PyExc_TypeError,
                        "`fulfilled` argument must be a callable");
        return NULL;
    }
    rejected = (rejected == Py_None ? NULL: rejected);
    if (rejected && !PyCallable_Check(rejected)) {
        PyErr_SetString(PyExc_TypeError,
                        "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = Promise_Then(self);
    if (promise) {
        Py_XINCREF(fulfilled);
        Py_XINCREF(rejected);
        promise->flags |= PROMISE_PY_CALLBACK;
        promise->fulfilled = fulfilled;
        promise->rejected = rejected;
    }
    return (PyObject *) promise;
}

/*[clinic input]
promise.Promise.catch

    self: self(type="Promise *")
    rejected: object
        failure callback
    /

The catch() method returns a new Promise and deals with rejected cases only.
[clinic start generated code]*/

static PyObject *
promise_Promise_catch(Promise *self, PyObject *rejected)
/*[clinic end generated code: output=03c895dec1349232 input=105b1740f16dd015]*/
{
    if (!PyCallable_Check(rejected)) {
        PyErr_SetString(PyExc_TypeError,
                        "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = Promise_Then(self);
    if (promise) {
        Py_INCREF(rejected);
        promise->flags |= PROMISE_PY_CALLBACK;
        promise->rejected = rejected;
    }
    return (PyObject *) promise;
}

/*[clinic input]
promise.Promise.finally_

    self: self(type="Promise *")
    finally_: object
        finally callback
    /

The finally_() method returns a new Promise.

When the promise is settled, i.e either fulfilled or rejected,
the specified callback function is executed. This provides a way for code to be run whether the promise
was fulfilled successfully or rejected once the Promise has been dealt with.
[clinic start generated code]*/

static PyObject *
promise_Promise_finally_(Promise *self, PyObject *finally_)
/*[clinic end generated code: output=0cb3d910e3facf7d input=181db7d6d3b9ffb8]*/
{
    if (!PyCallable_Check(finally_)) {
        PyErr_SetString(PyExc_TypeError,
                        "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = Promise_Then(self);
    if (promise) {
        Py_INCREF(finally_);
        self->finally = finally_;
    }
    return  (PyObject *) promise;
}

static PyMethodDef PromiseType_methods[] = {
    PROMISE_PROMISE_THEN_METHODDEF
    PROMISE_PROMISE_CATCH_METHODDEF
    PROMISE_PROMISE_FINALLY__METHODDEF
    {NULL}  /* Sentinel */
};

static PyObject *
Promiseiter_New(PyObject *self)
{
    Promiseiter *it = Mem_GC_New(Promiseiter, &PromiseiterType);
    if (it == NULL)
        return NULL;

    PyObject_GC_Track(it);
    Promise *promise = (Promise *) self;
    if (promise->coro || promise->flags & PROMISE_RESOLVED) {
        promise = Promise_Then(promise);
    } else {
        Py_INCREF(promise);
    }
    if (promise == NULL) {
        Py_DECREF(it);
        return NULL;
    }
    it->promise = promise;
    return (PyObject *) it;
}

static PyAsyncMethods PromiseType_as_async = {
    (unaryfunc) Promiseiter_New,     /* am_await */
    0,                     /* am_aiter */
    0                      /* am_anext */
};

PyDoc_STRVAR(PromiseType__doc__,
"The Promise object represents the eventual completion (or failure) of an "
"asynchronous operation and its resulting value.\n");

static PyTypeObject PromiseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Promise",
    .tp_doc = PromiseType__doc__,
    .tp_basicsize = sizeof(Promise),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_HAVE_FINALIZE,
    .tp_dealloc = (destructor) promise_dealloc,
    .tp_traverse = (traverseproc) promise_traverse,
    .tp_clear = (inquiry) promise_clear,
    .tp_repr = promise_repr,
    .tp_methods = PromiseType_methods,
    .tp_as_async = &PromiseType_as_async
};

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
    PyObject_GC_UnTrack(self);
    promiseiter_clear(self);
    Mem_GC_Del(Promiseiter, self);
}

static PyObject*
promiseiter_repr(PyObject* self)
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

static PyMethodDef PromiseIterType_methods[] = {
    {"send",  (PyCFunction)promiseiter_send, METH_O, NULL},
    {NULL, NULL}        /* Sentinel */
};

static PyTypeObject PromiseiterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "PromiseIter",
    .tp_basicsize = sizeof(Promiseiter),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_dealloc = (destructor) promiseiter_dealloc,
    .tp_repr = promiseiter_repr,
    .tp_iter = PyObject_SelfIter,
    .tp_traverse = (traverseproc) promiseiter_traverse,
    .tp_iternext = (iternextfunc) promiseiter_iternext,
    .tp_clear = (inquiry) promiseiter_clear,
    .tp_methods = PromiseIterType_methods
};

Deferred *
Deferred_New()
{
    Promise *promise = Promise_New();
    if (promise == NULL) {
        return NULL;
    }

    Deferred *self = Mem_GC_New(Deferred, &DeferredType);
    if (self == NULL) {
        Py_DECREF(promise);
        return NULL;
    }

    self->promise = promise;
    PyObject_GC_Track(self);
    return self;
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
    PyObject_GC_UnTrack(self);
    deferred_clear(self);
    Mem_GC_Del(Deferred, self);
}

static PyObject*
deferred_repr(PyObject* self)
{
    return PyUnicode_FromFormat("<Deferred object at %p>", self);
}

/*[clinic input]
class promise.Deferred "deferred *" "&DeferredType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=db457374169687bc]*/

/*[clinic input]
promise.Deferred.resolve

    self: self(type="Deferred *")
    value: object
    /

Resolve an associated promise with the given value.
[clinic start generated code]*/

static PyObject *
promise_Deferred_resolve(Deferred *self, PyObject *value)
/*[clinic end generated code: output=889732b43efb738b input=91c6bcfd68907eb9]*/
{
    if (Promise_Resolve(self->promise, value)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.reject

    self: self(type="Deferred *")
    value: object
    /

Reject an associated promise with the given exception.
[clinic start generated code]*/

static PyObject *
promise_Deferred_reject(Deferred *self, PyObject *value)
/*[clinic end generated code: output=ead6c9f40d549dde input=ae34a69722c339b2]*/
{
    if (Promise_Reject(self->promise, value)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.promise

    self: self(type="Deferred *")

Get an associated Promise object.
[clinic start generated code]*/

static PyObject *
promise_Deferred_promise_impl(Deferred *self)
/*[clinic end generated code: output=b560c2d8e4a6a0eb input=23308a5160e5a6c8]*/
{
    Promise *ret = self->promise;
    Py_INCREF(ret);
    return (PyObject *) ret;
}

static PyMethodDef DeferredType_methods[] = {
    PROMISE_DEFERRED_RESOLVE_METHODDEF
    PROMISE_DEFERRED_REJECT_METHODDEF
    PROMISE_DEFERRED_PROMISE_METHODDEF
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(DeferredType__doc__,
"Deferred()\n\n"
"A Deferred object is used to provide a new promise along with methods to change its state.\n");

static PyTypeObject DeferredType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Deferred",
    .tp_doc = DeferredType__doc__,
    .tp_basicsize = sizeof(Deferred),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_dealloc = (destructor) deferred_dealloc,
    .tp_traverse = (traverseproc) deferred_traverse,
    .tp_clear = (inquiry) deferred_clear,
    .tp_repr = deferred_repr,
    .tp_methods = DeferredType_methods,
};

// ---------------------- Module ----------------------

int
Promise_ExecAsync(PyObject* coro)
{
    Promise *promise = Promise_New();
    if (promise == NULL) {
        return -1;
    }
    schedule_promise(promise, Py_None, PROMISE_FULFILLED | PROMISE_VALUABLE);
    Py_INCREF(coro);
    promise->coro = coro;
    Py_DECREF(promise);
    return 0;
}

static int
resume_coroutine(PyObject *coro, PyObject *value, unsigned int state)
{
    _Py_IDENTIFIER(throw);

    PyObject *result;
    Py_INCREF(value);

    while (1) {
        if (state == PROMISE_REJECTED) {
            result = _PyObject_CallMethodIdOneArg(coro, &PyId_throw, value);
        } else {
            result = _PyGen_Send((PyGenObject *) coro, value);
        }

        Py_DECREF(value);

        if (result == NULL) {
            if (_PyGen_FetchStopIterationValue(&value) == 0) {
                Py_DECREF(value);
            } else {
                if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt) ||
                    PyErr_ExceptionMatches(PyExc_SystemExit))
                {
                    return -1;
                }
                print_unhandled_exception();
            }
            return 0;
        }

        if (!Promise_CheckExact(result)) {
            Py_DECREF(result);
            value = new_exception(PyExc_RuntimeError, "`await` argument expected to be a promise.");
            if (value == NULL) {
                value = fetch_current_exception();
            }
            state = PROMISE_REJECTED;
            continue;
        }

        ((Promise *) result)->flags |= PROMISE_VALUABLE;
        Py_INCREF(coro);
        ((Promise *) result)->coro = coro;
        Py_DECREF(result);

        break;
    }

    return 0;
}

#define CLEAR_PROMISE_CALLBACK(ob)                         \
    if ((ob)->flags & PROMISE_HAS_CALLBACK) {              \
        if ((ob)->flags & PROMISE_PY_CALLBACK) {           \
            Py_CLEAR((ob)->fulfilled);                     \
            Py_CLEAR((ob)->rejected);                      \
        }                                                  \
        (ob)->flags ^= (ob)->flags & PROMISE_HAS_CALLBACK; \
    }

static int
handle_scheduled_promise(Promise *promise)
{
    // it's a heart of engine

    assert(!(promise->flags & PROMISE_RESOLVED) && (promise->flags & PROMISE_SCHEDULED));

    unsigned int state = promise->flags & PROMISE_SCHEDULED;
    promise->flags |= PROMISE_RESOLVING;

    if (promise->flags & PROMISE_HAS_CALLBACK) {
        PyObject *value = NULL;
        PyObject *handler = (promise->flags & PROMISE_FULFILLED) ? promise->fulfilled : promise->rejected;
        if (handler) {
            if (promise->flags & PROMISE_C_CALLBACK) {
                // callback should handle the context itself
                value = ((promisecallback) handler)(promise->context, promise->value);
                promise->context = NULL;
            } else {
                // context MUST be NULL in this case
                value = PyObject_CallOneArg(handler, promise->value);
            }
            if (value == NULL) {
                if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt) ||
                    PyErr_ExceptionMatches(PyExc_SystemExit))
                {
                    return -1;
                }
                state = PROMISE_REJECTED;
                value = fetch_current_exception();
            } else {
                state = PROMISE_FULFILLED;
            }
        } else if (promise->flags & PROMISE_C_CALLBACK) {
            Py_CLEAR(promise->context);
        }
        if (value != NULL) {
            if (Promise_CheckExact(value)) {
                Promise *new_promise = (Promise *) value;
                if (new_promise == promise) {
                    // The same promise. It's bad but not fatal.
                    Py_DECREF(new_promise);
                } else {
                    if (new_promise->flags & PROMISE_RESOLVED) {
                        value = new_promise->value;
                        state = new_promise->flags & PROMISE_SCHEDULED;
                        Py_INCREF(value);
                        Py_DECREF(new_promise);
                    } else {
                        // promise will be removed from the promise_chain
                        // we must re-schedule it
                        Py_INCREF(promise);
                        Chain_Add(new_promise, promise, promise);
                        CLEAR_PROMISE_CALLBACK(promise);
                        Py_DECREF(new_promise);
                        return 0;
                    }
                }
            } else {
                Py_XSETREF(promise->value, value);
            }
        }
        if (promise->finally) {
            PyObject *tmp = PyObject_CallNoArgs(promise->finally);
            if (tmp == NULL) {
                if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt) ||
                    PyErr_ExceptionMatches(PyExc_SystemExit))
                {
                    return -1;
                }
                print_unhandled_exception();
            } else {
                Py_DECREF(tmp);
            }
            Py_CLEAR(promise->finally);
        }
        CLEAR_PROMISE_CALLBACK(promise);
    }

    promise->flags ^= promise->flags & PROMISE_SCHEDULED;
    promise->flags |= state | PROMISE_RESOLVED;

    if (promise->head) {
        Promise *it = promise->head;
        PyObject *value = promise->value;
        while (it) {
            Py_INCREF(value);
            it->value = value;
            it->flags |= state;
            it = it->next;
        }
        Chain_Add(&promise_chain, promise->head, promise->tail);
        promise->head = NULL;
    }

    if (promise->coro) {
        int ret = resume_coroutine(promise->coro, promise->value, state);
        // release coro ASAP
        Py_CLEAR(promise->coro);
        return ret;
    }

    return 0;
}

void
PromiseChain_Clear()
{
    while (promise_chain.head) {
        Promise *it = promise_chain.head;
        promise_chain.head = it->next;
        it->next = NULL;
        Py_DECREF(it);
    }
    promise_chain.tail = NULL;
}

int
PromiseChain_Process()
{
    if (promise_chain.head != NULL) {
        while (promise_chain.head) {
            Promise *it = promise_chain.head;
            promise_chain.head = it->next;
            it->next = NULL;
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

int
Promise_module_init()
{
    if (PyType_Ready(&PromiseType) < 0) {
        return -1;
    }
    if (PyType_Ready(&DeferredType) < 0) {
        return -1;
    }
    if (PyType_Ready(&PromiseiterType) < 0) {
        return -1;
    }
    promise_chain.head = NULL;
    promise_chain.tail = NULL;
    return 0;
}
