/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(promise_clearfreelists__doc__,
"clearfreelists($module, /)\n"
"--\n"
"\n"
"Clear freelists\n"
"\n"
"");

#define PROMISE_CLEARFREELISTS_METHODDEF    \
    {"clearfreelists", (PyCFunction)promise_clearfreelists, METH_NOARGS, promise_clearfreelists__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_clearfreelists_impl(PyObject *module);

static PyObject *
promise_clearfreelists(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promise_clearfreelists_impl(module);
}

PyDoc_STRVAR(promise_setfreelistlimits__doc__,
"setfreelistlimits($module, /, promise_limit=-1, promiseiter_limit=-1,\n"
"                  deferred_limit=-1)\n"
"--\n"
"\n"
"Set freelist limits\n"
"\n"
"");

#define PROMISE_SETFREELISTLIMITS_METHODDEF    \
    {"setfreelistlimits", (PyCFunction)(void(*)(void))promise_setfreelistlimits, METH_FASTCALL|METH_KEYWORDS, promise_setfreelistlimits__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_setfreelistlimits_impl(PyObject *module, Py_ssize_t promise_limit,
                               Py_ssize_t promiseiter_limit,
                               Py_ssize_t deferred_limit);

static PyObject *
promise_setfreelistlimits(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"promise_limit", "promiseiter_limit", "deferred_limit", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "setfreelistlimits", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    Py_ssize_t promise_limit = -1;
    Py_ssize_t promiseiter_limit = -1;
    Py_ssize_t deferred_limit = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = _PyNumber_Index(args[0]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            promise_limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[1]) {
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = _PyNumber_Index(args[1]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            promiseiter_limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = _PyNumber_Index(args[2]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        deferred_limit = ival;
    }
skip_optional_pos:
    return_value = promise_setfreelistlimits_impl(module, promise_limit, promiseiter_limit, deferred_limit);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_execasync__doc__,
"exec_async($module, /, coro)\n"
"--\n"
"\n"
"");

#define PROMISE_EXECASYNC_METHODDEF    \
    {"exec_async", (PyCFunction)(void(*)(void))promise_execasync, METH_FASTCALL|METH_KEYWORDS, promise_execasync__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_execasync_impl(PyObject *module, PyObject *coro);

static PyObject *
promise_execasync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"coro", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "exec_async", 0};
    PyObject *argsbuf[1];
    PyObject *coro;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyObject_TypeCheck(args[0], &PyCoro_Type)) {
        _PyArg_BadArgument("exec_async", "argument 'coro'", (&PyCoro_Type)->tp_name, args[0]);
        goto exit;
    }
    coro = args[0];
    return_value = promise_execasync_impl(module, coro);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_process_promise_chain__doc__,
"process_promise_chain($module, /)\n"
"--\n"
"\n"
"");

#define PROMISE_PROCESS_PROMISE_CHAIN_METHODDEF    \
    {"process_promise_chain", (PyCFunction)promise_process_promise_chain, METH_NOARGS, promise_process_promise_chain__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_process_promise_chain_impl(PyObject *module);

static PyObject *
promise_process_promise_chain(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promise_process_promise_chain_impl(module);
}

PyDoc_STRVAR(promise_run_forever__doc__,
"run_forever($module, /)\n"
"--\n"
"\n"
"Run loop.");

#define PROMISE_RUN_FOREVER_METHODDEF    \
    {"run_forever", (PyCFunction)promise_run_forever, METH_NOARGS, promise_run_forever__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_run_forever_impl(PyObject *module);

static PyObject *
promise_run_forever(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promise_run_forever_impl(module);
}

PyDoc_STRVAR(promise_Promise_then__doc__,
"then($self, /, fulfilled=None, rejected=None)\n"
"--\n"
"\n"
"Create new Promise.\n"
"\n"
"It takes up to two arguments: callback functions for the success and failure cases of the Promise.");

#define PROMISE_PROMISE_THEN_METHODDEF    \
    {"then", (PyCFunction)(void(*)(void))promise_Promise_then, METH_FASTCALL|METH_KEYWORDS, promise_Promise_then__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Promise_then_impl(Promise *self, PyObject *fulfilled,
                          PyObject *rejected);

static PyObject *
promise_Promise_then(Promise *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fulfilled", "rejected", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "then", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    PyObject *fulfilled = Py_None;
    PyObject *rejected = Py_None;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        fulfilled = args[0];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    rejected = args[1];
skip_optional_pos:
    return_value = promise_Promise_then_impl(self, fulfilled, rejected);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_Promise_catch__doc__,
"catch($self, /, rejected)\n"
"--\n"
"\n"
"Create new Promise.\n"
"\n"
"It is the same as `.then(None, rejected)`");

#define PROMISE_PROMISE_CATCH_METHODDEF    \
    {"catch", (PyCFunction)(void(*)(void))promise_Promise_catch, METH_FASTCALL|METH_KEYWORDS, promise_Promise_catch__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Promise_catch_impl(Promise *self, PyObject *rejected);

static PyObject *
promise_Promise_catch(Promise *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"rejected", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "catch", 0};
    PyObject *argsbuf[1];
    PyObject *rejected;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    rejected = args[0];
    return_value = promise_Promise_catch_impl(self, rejected);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_deferred__doc__,
"deferred($module, /)\n"
"--\n"
"\n"
"Create new Deferred object.");

#define PROMISE_DEFERRED_METHODDEF    \
    {"deferred", (PyCFunction)promise_deferred, METH_NOARGS, promise_deferred__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_deferred_impl(PyObject *module);

static PyObject *
promise_deferred(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promise_deferred_impl(module);
}

PyDoc_STRVAR(promise_Deferred_resolve__doc__,
"resolve($self, /, value)\n"
"--\n"
"\n"
"Resolve related Promise object with the given value.");

#define PROMISE_DEFERRED_RESOLVE_METHODDEF    \
    {"resolve", (PyCFunction)(void(*)(void))promise_Deferred_resolve, METH_FASTCALL|METH_KEYWORDS, promise_Deferred_resolve__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_resolve_impl(Deferred *self, PyObject *value);

static PyObject *
promise_Deferred_resolve(Deferred *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"value", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "resolve", 0};
    PyObject *argsbuf[1];
    PyObject *value;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    value = args[0];
    return_value = promise_Deferred_resolve_impl(self, value);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_Deferred_reject__doc__,
"reject($self, /, value)\n"
"--\n"
"\n"
"Reject related Promise object with the given exception exc.");

#define PROMISE_DEFERRED_REJECT_METHODDEF    \
    {"reject", (PyCFunction)(void(*)(void))promise_Deferred_reject, METH_FASTCALL|METH_KEYWORDS, promise_Deferred_reject__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_reject_impl(Deferred *self, PyObject *value);

static PyObject *
promise_Deferred_reject(Deferred *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"value", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "reject", 0};
    PyObject *argsbuf[1];
    PyObject *value;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    value = args[0];
    return_value = promise_Deferred_reject_impl(self, value);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_Deferred_promise__doc__,
"promise($self, /)\n"
"--\n"
"\n"
"Get related Promise object.");

#define PROMISE_DEFERRED_PROMISE_METHODDEF    \
    {"promise", (PyCFunction)promise_Deferred_promise, METH_NOARGS, promise_Deferred_promise__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Deferred_promise_impl(Deferred *self);

static PyObject *
promise_Deferred_promise(Deferred *self, PyObject *Py_UNUSED(ignored))
{
    return promise_Deferred_promise_impl(self);
}

Py_LOCAL_INLINE(PyObject *)
promise_Lock_impl(PyTypeObject *type);

static PyObject *
promise_Lock(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *return_value = NULL;

    if ((type == _CTX_get_type(type)->LockType) &&
        !_PyArg_NoPositional("Lock", args)) {
        goto exit;
    }
    if ((type == _CTX_get_type(type)->LockType) &&
        !_PyArg_NoKeywords("Lock", kwargs)) {
        goto exit;
    }
    return_value = promise_Lock_impl(type);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_Lock_acquire__doc__,
"acquire($self, /)\n"
"--\n"
"\n"
"");

#define PROMISE_LOCK_ACQUIRE_METHODDEF    \
    {"acquire", (PyCFunction)promise_Lock_acquire, METH_NOARGS, promise_Lock_acquire__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Lock_acquire_impl(Lock *self);

static PyObject *
promise_Lock_acquire(Lock *self, PyObject *Py_UNUSED(ignored))
{
    return promise_Lock_acquire_impl(self);
}

PyDoc_STRVAR(promise_Lock_release__doc__,
"release($self, /)\n"
"--\n"
"\n"
"");

#define PROMISE_LOCK_RELEASE_METHODDEF    \
    {"release", (PyCFunction)promise_Lock_release, METH_NOARGS, promise_Lock_release__doc__},

Py_LOCAL_INLINE(PyObject *)
promise_Lock_release_impl(Lock *self);

static PyObject *
promise_Lock_release(Lock *self, PyObject *Py_UNUSED(ignored))
{
    return promise_Lock_release_impl(self);
}
/*[clinic end generated code: output=bdbb853088ca1433 input=a9049054013a1b77]*/
