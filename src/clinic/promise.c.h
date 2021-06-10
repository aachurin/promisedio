/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(promise_Promise_then__doc__,
"then($self, /, fulfilled=None, rejected=None)\n"
"--\n"
"\n"
"\n"
"Returns a new Promise.\n"
"It takes up to two arguments: callback functions for the success and failure cases of the Promise.");

#define PROMISE_PROMISE_THEN_METHODDEF    \
    {"then", (PyCFunction)(void(*)(void))promise_Promise_then, METH_FASTCALL|METH_KEYWORDS, promise_Promise_then__doc__},

static inline PyObject *
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
"\n"
"Returns a new Promise.\n"
"It is the same as `.then(None, rejected)`");

#define PROMISE_PROMISE_CATCH_METHODDEF    \
    {"catch", (PyCFunction)(void(*)(void))promise_Promise_catch, METH_FASTCALL|METH_KEYWORDS, promise_Promise_catch__doc__},

static inline PyObject *
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

PyDoc_STRVAR(promise_Deferred_resolve__doc__,
"resolve($self, /, value)\n"
"--\n"
"\n"
"\n"
"Resolves related Promise object with the given value.");

#define PROMISE_DEFERRED_RESOLVE_METHODDEF    \
    {"resolve", (PyCFunction)(void(*)(void))promise_Deferred_resolve, METH_FASTCALL|METH_KEYWORDS, promise_Deferred_resolve__doc__},

static inline PyObject *
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
"\n"
"Rejects related Promise object with the given exception exc.");

#define PROMISE_DEFERRED_REJECT_METHODDEF    \
    {"reject", (PyCFunction)(void(*)(void))promise_Deferred_reject, METH_FASTCALL|METH_KEYWORDS, promise_Deferred_reject__doc__},

static inline PyObject *
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
"\n"
"Returns related Promise object.");

#define PROMISE_DEFERRED_PROMISE_METHODDEF    \
    {"promise", (PyCFunction)promise_Deferred_promise, METH_NOARGS, promise_Deferred_promise__doc__},

static inline PyObject *
promise_Deferred_promise_impl(Deferred *self);

static PyObject *
promise_Deferred_promise(Deferred *self, PyObject *Py_UNUSED(ignored))
{
    return promise_Deferred_promise_impl(self);
}
/*[clinic end generated code: output=e1d8c715730977ff input=a9049054013a1b77]*/
