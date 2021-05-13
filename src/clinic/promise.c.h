/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(promise_Promise_then__doc__,
"then($self, fulfilled=None, rejected=None, /)\n"
"--\n"
"\n"
"The then() method returns a new Promise.\n"
"\n"
"  fulfilled\n"
"    success callback\n"
"  rejected\n"
"    failure callback\n"
"\n"
"It takes up to two arguments:\n"
"callback functions for the success and failure cases of the Promise.\n"
"\n"
"");

#define PROMISE_PROMISE_THEN_METHODDEF    \
    {"then", (PyCFunction)(void(*)(void))promise_Promise_then, METH_FASTCALL, promise_Promise_then__doc__},

static PyObject *
promise_Promise_then_impl(Promise *self, PyObject *fulfilled,
                          PyObject *rejected);

static PyObject *
promise_Promise_then(Promise *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *fulfilled = Py_None;
    PyObject *rejected = Py_None;

    if (!_PyArg_CheckPositional("then", nargs, 0, 2)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    fulfilled = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    rejected = args[1];
skip_optional:
    return_value = promise_Promise_then_impl(self, fulfilled, rejected);

exit:
    return return_value;
}

PyDoc_STRVAR(promise_Promise_catch__doc__,
"catch($self, rejected, /)\n"
"--\n"
"\n"
"The catch() method returns a new Promise and deals with rejected cases only.\n"
"\n"
"  rejected\n"
"    failure callback\n"
"\n"
"");

#define PROMISE_PROMISE_CATCH_METHODDEF    \
    {"catch", (PyCFunction)promise_Promise_catch, METH_O, promise_Promise_catch__doc__},

PyDoc_STRVAR(promise_Promise_finally___doc__,
"finally_($self, finally_, /)\n"
"--\n"
"\n"
"The finally_() method returns a new Promise.\n"
"\n"
"  finally_\n"
"    finally callback\n"
"\n"
"When the promise is settled, i.e either fulfilled or rejected,\n"
"the specified callback function is executed. This provides a way for code to be run whether the promise\n"
"was fulfilled successfully or rejected once the Promise has been dealt with.\n"
"\n"
"");

#define PROMISE_PROMISE_FINALLY__METHODDEF    \
    {"finally_", (PyCFunction)promise_Promise_finally_, METH_O, promise_Promise_finally___doc__},

PyDoc_STRVAR(promise_Deferred_resolve__doc__,
"resolve($self, value, /)\n"
"--\n"
"\n"
"Resolve an associated promise with the given value.\n"
"\n"
"");

#define PROMISE_DEFERRED_RESOLVE_METHODDEF    \
    {"resolve", (PyCFunction)promise_Deferred_resolve, METH_O, promise_Deferred_resolve__doc__},

PyDoc_STRVAR(promise_Deferred_reject__doc__,
"reject($self, value, /)\n"
"--\n"
"\n"
"Reject an associated promise with the given exception.\n"
"\n"
"");

#define PROMISE_DEFERRED_REJECT_METHODDEF    \
    {"reject", (PyCFunction)promise_Deferred_reject, METH_O, promise_Deferred_reject__doc__},

PyDoc_STRVAR(promise_Deferred_promise__doc__,
"promise($self, /)\n"
"--\n"
"\n"
"Get an associated Promise object.\n"
"\n"
"");

#define PROMISE_DEFERRED_PROMISE_METHODDEF    \
    {"promise", (PyCFunction)promise_Deferred_promise, METH_NOARGS, promise_Deferred_promise__doc__},

static PyObject *
promise_Deferred_promise_impl(Deferred *self);

static PyObject *
promise_Deferred_promise(Deferred *self, PyObject *Py_UNUSED(ignored))
{
    return promise_Deferred_promise_impl(self);
}
/*[clinic end generated code: output=d6aa75a0f653ec52 input=a9049054013a1b77]*/
