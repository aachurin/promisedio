// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <Python.h>
#include "promise.h"
#include "memory.h"
#include "clinic/promise.c.h"

#define Promise_CHECK_EXACT(obj) (Py_TYPE(obj) == &PromiseType)

#define Chain_ADD(chain, chain_head, chain_tail) \
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

Mem_GC_FREELIST(Promise);
Mem_GC_FREELIST(Deferred);
Mem_GC_FREELIST(PromiseIter);

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

void
promise_print_unhandled_exception()
{
    PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    if (exc == NULL) {
        PySys_WriteStderr("lost exception value\n");
        return;
    }
    PyErr_NormalizeException(&exc, &val, &tb);
    if (tb == NULL) {
        tb = OWNED(Py_None);
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
            tb = OWNED(Py_None);
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
promise_new()
{
    Promise *promise = Mem_GC_NEW(Promise, &PromiseType);
    if (promise == NULL)
        return NULL;
    promise->head = NULL;
    promise->next = NULL;
    promise->value = NULL;
    promise->coro = NULL;
    promise->flags = PROMISE_INITIAL;
    promise->fulfilled = NULL;
    promise->rejected = NULL;
    promise->context = NULL;
    PyObject_GC_Track(promise);
    return promise;
}

void
promise_callback(Promise *p, promisecallback fulfilled, promisecallback rejected, PyObject *context) {
    assert (!(p->flags & (PROMISE_HAS_CALLBACK | PROMISE_FREEZED)));
    p->flags |= PROMISE_C_CALLBACK;
    p->fulfilled = (PyObject *) fulfilled;
    p->rejected = (PyObject *) rejected;
    p->context = context;
}

Promise *
promise_then(Promise *p)
{
    Promise *promise = promise_new();
    if (promise == NULL)
        return NULL;
    p->flags |= PROMISE_VALUABLE;
    if (p->flags & PROMISE_RESOLVED) {
        promise->value = OWNED(p->value);
        promise->flags |= p->flags & PROMISE_SCHEDULED;
        Chain_ADD(&promise_chain, promise, promise);
    } else {
        Chain_ADD(p, promise, promise);
    }
    promise->flags |= PROMISE_INTERIM;
    return OWNED(promise);
}

static inline void
schedule_promise(Promise *self, PyObject *value, unsigned int flag)
{
    self->value = OWNED(value);
    self->flags |= flag;
    Py_INCREF(self);
    Chain_ADD(&promise_chain, self, self);
}

Promise *
promise_new_resolved(PyObject *value)
{
    Promise *promise = promise_new();
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

void
promise_resolve(Promise *self, PyObject *value)
{
    assert (!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED))
        schedule_promise(self, value, PROMISE_FULFILLED);
}

void
promise_reject(Promise *self, PyObject *value)
{
    assert (!(self->flags & PROMISE_INTERIM));
    if (!(self->flags & PROMISE_SCHEDULED))
        schedule_promise(self, value, PROMISE_REJECTED);
}

int
promise_reject_args(Promise *p, PyObject *exc, PyObject *args)
{
    assert (!(p->flags & PROMISE_INTERIM));
    if (!(p->flags & PROMISE_SCHEDULED)) {
        PyObject *value = PyObject_Call(exc, args, NULL);
        if (value == NULL)
            return -1;
        schedule_promise(p, value, PROMISE_REJECTED);
        Py_DECREF(value);
    }
    return 0;
}

int
promise_reject_string(Promise *p, PyObject *exc, const char *msg)
{
    assert (!(p->flags & PROMISE_INTERIM));
    if (!(p->flags & PROMISE_SCHEDULED)) {
        PyObject *value = new_exception(exc, msg);
        if (value == NULL)
            return -1;
        schedule_promise(p, value, PROMISE_REJECTED);
        Py_DECREF(value);
    }
    return 0;
}

void
promise_reject_py_exc(Promise *p)
{
    PyObject *exc = fetch_current_exception();
    promise_reject(p, exc);
    Py_XDECREF(exc);
}

static int
promise_traverse(Promise *self, visitproc visit, void *arg)
{
    Py_VISIT(self->next);
    Py_VISIT(self->head);
    Py_VISIT(self->value);
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
    Mem_GC_DEL(Promise, self);
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
    rejected: object = None

@doc[Promise.then]
[clinic start generated code]*/

static inline PyObject *
promise_Promise_then_impl(Promise *self, PyObject *fulfilled,
                          PyObject *rejected)
/*[clinic end generated code: output=f5334e56a034be28 input=fc0a44add0ce74f7]*/
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
    Promise *promise = promise_then(self);
    if (promise) {
        promise->flags |= PROMISE_PY_CALLBACK;
        promise->fulfilled = XOWNED(fulfilled);
        promise->rejected = XOWNED(rejected);
    }
    return (PyObject *) promise;
}

/*[clinic input]
promise.Promise.catch
    self: self(type="Promise *")
    rejected: object

@doc[Promise.catch]
[clinic start generated code]*/

static inline PyObject *
promise_Promise_catch_impl(Promise *self, PyObject *rejected)
/*[clinic end generated code: output=6ba8d48eaec56d88 input=66119f711bbabbc4]*/
{
    if (!PyCallable_Check(rejected)) {
        PyErr_SetString(PyExc_TypeError,
                        "`rejected` argument must be a callable");
        return NULL;
    }
    Promise *promise = promise_then(self);
    if (promise) {
        promise->flags |= PROMISE_PY_CALLBACK;
        promise->rejected = OWNED(rejected);
    }
    return (PyObject *) promise;
}

static PyMethodDef PromiseType_methods[] = {
    PROMISE_PROMISE_THEN_METHODDEF
    PROMISE_PROMISE_CATCH_METHODDEF
    {NULL}  /* Sentinel */
};

static PyObject *
promiseiter_new(PyObject *self)
{
    PromiseIter *it = Mem_GC_NEW(PromiseIter, &PromiseiterType);
    if (!it)
        return NULL;
    PyObject_GC_Track(it);

    Promise *promise = (Promise *) self;
    if (promise->coro || promise->flags & PROMISE_RESOLVED) {
        promise = promise_then(promise);
        if (promise == NULL) {
            Py_DECREF(it);
            return NULL;
        }
        it->promise = promise;
    } else {
        it->promise = OWNED(promise);
    }
    return (PyObject *) it;
}

static PyAsyncMethods PromiseType_as_async = {
    (unaryfunc) promiseiter_new,     /* am_await */
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
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_dealloc = (destructor) promise_dealloc,
    .tp_traverse = (traverseproc) promise_traverse,
    .tp_clear = (inquiry) promise_clear,
    .tp_repr = promise_repr,
    .tp_methods = PromiseType_methods,
    .tp_as_async = &PromiseType_as_async
};

static int
promiseiter_traverse(PromiseIter *self, visitproc visit, void *arg)
{
    Py_VISIT(self->promise);
    return 0;
}

static int
promiseiter_clear(PromiseIter *self)
{
    Py_CLEAR(self->promise);
    return 0;
}

static void
promiseiter_dealloc(PromiseIter *self)
{
    PyObject_GC_UnTrack(self);
    promiseiter_clear(self);
    Mem_GC_DEL(PromiseIter, self);
}

static PyObject*
promiseiter_repr(PyObject* self)
{
    return PyUnicode_FromFormat("<promiseiter object at %p>", self);
}

static PyObject *
promiseiter_iternext(PromiseIter *self)
{
    PyObject *res = (PyObject *) (self->promise);
    self->promise = NULL;
    return res;
}

static PyObject *
promiseiter_send(PromiseIter *self, PyObject *value)
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
    .tp_basicsize = sizeof(PromiseIter),
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
deferred_new()
{
    Promise *promise = promise_new();
    if (promise == NULL) {
        return NULL;
    }

    Deferred *self = Mem_GC_NEW(Deferred, &DeferredType);
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
    Mem_GC_DEL(Deferred, self);
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

@doc[Deferred.resolve]
[clinic start generated code]*/

static inline PyObject *
promise_Deferred_resolve_impl(Deferred *self, PyObject *value)
/*[clinic end generated code: output=b54734e0189ffbe7 input=da7acc2b24e502f3]*/
{
    promise_resolve(self->promise, value);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.reject
    self: self(type="Deferred *")
    value: object

@doc[Deferred.reject]
[clinic start generated code]*/

static inline PyObject *
promise_Deferred_reject_impl(Deferred *self, PyObject *value)
/*[clinic end generated code: output=5d9badc8d68eb6af input=6c9c062cb8050ed6]*/
{
    if (value == NULL || (!PyExceptionClass_Check(value) && !PyExceptionInstance_Check(value))) {
        PyErr_SetString(PyExc_TypeError,
                        "exceptions must be classes deriving BaseException or "
                        "instances of such a class");
        return NULL;
    }
    promise_reject(self->promise, value);
    Py_RETURN_NONE;
}

/*[clinic input]
promise.Deferred.promise
    self: self(type="Deferred *")

@doc[Deferred.promise]
[clinic start generated code]*/

static inline PyObject *
promise_Deferred_promise_impl(Deferred *self)
/*[clinic end generated code: output=705cbf07ed7d6867 input=21148e1ebba88ab7]*/
{
    return (PyObject *) OWNED(self->promise);
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
promise_exec_async(PyObject* coro)
{
    Promise *promise = promise_new();
    if (promise == NULL) {
        return -1;
    }
    schedule_promise(promise, Py_None, PROMISE_FULFILLED | PROMISE_VALUABLE);
    promise->coro = OWNED(coro);
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
        if (state & PROMISE_REJECTED) {
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
                promise_print_unhandled_exception();
            }
            return 0;
        }

        if (!Promise_CHECK_EXACT(result)) {
            Py_DECREF(result);
            value = new_exception(PyExc_RuntimeError, "`await` argument expected to be a promise.");
            if (value == NULL) {
                value = fetch_current_exception();
            }
            state = PROMISE_REJECTED;
            continue;
        }

        ((Promise *) result)->flags |= PROMISE_VALUABLE;
        ((Promise *) result)->coro = OWNED(coro);
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

    unsigned int state = promise->flags & PROMISE_SCHEDULED;
    promise->flags |= PROMISE_RESOLVING;

    if (promise->flags & PROMISE_HAS_CALLBACK) {
        PyObject *value = NULL;
        PyObject *handler = (promise->flags & PROMISE_FULFILLED) ? promise->fulfilled : promise->rejected;
        if (handler) {
            if (promise->flags & PROMISE_C_CALLBACK) {
                value = ((promisecallback) handler)(promise->value, promise->context);
                if (promise->context && value == promise->context) {
                    promise->context = NULL;
                }
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
        }
        Py_CLEAR(promise->context);
        if (promise->flags & PROMISE_PY_CALLBACK) {
            Py_CLEAR(promise->fulfilled);
            Py_CLEAR(promise->rejected);
        }
        promise->flags ^= promise->flags & PROMISE_HAS_CALLBACK;

        if (value != NULL) {
            if (Promise_CHECK_EXACT(value)) {
                Promise *new_promise = (Promise *) value;
                if (new_promise == promise) {
                    // The same promise. It's bad but not fatal.
                    // Nothing to do.
                } else if (new_promise->flags & PROMISE_RESOLVED) {
                    // Easy peasy. Just copy state and value.
                    state = new_promise->flags & PROMISE_SCHEDULED;
                    value = new_promise->value;
                    Py_XSETREF(promise->value, OWNED(value));
                } else {
                    if (new_promise->coro || Py_REFCNT(new_promise) > 2) {
                        Py_XSETREF(new_promise, promise_then(new_promise));
                    }
                    if (promise->coro || Py_REFCNT(promise) > 1) {
                        // We must re-schedule promise
                        Py_INCREF(promise);
                        Chain_ADD(new_promise, promise, promise);
                    } else {
                        // We can replace current promise with the new one without consequences
                        new_promise->head = promise->head;
                        new_promise->tail = promise->tail;
                        promise->head = NULL;
                        promise->tail = NULL;
                    }
                    Py_DECREF(new_promise);
                    return 0;
                }
                Py_DECREF(new_promise);
            } else {
                Py_XSETREF(promise->value, value);
            }
        }
    } else {
        Py_CLEAR(promise->context);
    }

    promise->flags ^= promise->flags & PROMISE_SCHEDULED;
    promise->flags |= state | PROMISE_RESOLVED;

    if (promise->head) {
        Promise *it = promise->head;
        PyObject *value = promise->value;
        while (it) {
            it->value = OWNED(value);
            it->flags |= state;
            it = it->next;
        }
        Chain_ADD(&promise_chain, promise->head, promise->tail);
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
promise_clear_chain()
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
promise_process_chain()
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
promise_module_init()
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
