/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(signal_signal__doc__,
"signal($module, /, signalnum, handler)\n"
"--\n"
"\n"
"");

#define SIGNAL_SIGNAL_METHODDEF    \
    {"signal", (PyCFunction)(void(*)(void))signal_signal, METH_FASTCALL|METH_KEYWORDS, signal_signal__doc__},

Py_LOCAL_INLINE(PyObject *)
signal_signal_impl(PyObject *module, PyObject *signalnum, PyObject *handler);

static PyObject *
signal_signal(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"signalnum", "handler", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "signal", 0};
    PyObject *argsbuf[2];
    PyObject *signalnum;
    PyObject *handler;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    signalnum = args[0];
    handler = args[1];
    return_value = signal_signal_impl(module, signalnum, handler);

exit:
    return return_value;
}
/*[clinic end generated code: output=e9fa6a3d7fdf2e5b input=a9049054013a1b77]*/
