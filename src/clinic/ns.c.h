/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(ns_Stream_getsockname__doc__,
"getsockname($self, /)\n"
"--\n"
"\n"
"\n"
"Return the socket\'s own address (For TCP only).");

#define NS_STREAM_GETSOCKNAME_METHODDEF    \
    {"getsockname", (PyCFunction)ns_Stream_getsockname, METH_NOARGS, ns_Stream_getsockname__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_getsockname_impl(Stream *self);

static PyObject *
ns_Stream_getsockname(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_Stream_getsockname_impl(self);
}

PyDoc_STRVAR(ns_Stream_getpeername__doc__,
"getpeername($self, /)\n"
"--\n"
"\n"
"\n"
"Get the current address to which the handle is bound (For TCP only)");

#define NS_STREAM_GETPEERNAME_METHODDEF    \
    {"getpeername", (PyCFunction)ns_Stream_getpeername, METH_NOARGS, ns_Stream_getpeername__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_getpeername_impl(Stream *self);

static PyObject *
ns_Stream_getpeername(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_Stream_getpeername_impl(self);
}

PyDoc_STRVAR(ns_Stream_write__doc__,
"write($self, /, data)\n"
"--\n"
"\n"
"\n"
"Write data to the stream.");

#define NS_STREAM_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))ns_Stream_write, METH_FASTCALL|METH_KEYWORDS, ns_Stream_write__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_write_impl(Stream *self, PyObject *data);

static PyObject *
ns_Stream_write(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"data", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[1];
    PyObject *data;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    data = args[0];
    return_value = ns_Stream_write_impl(self, data);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_Stream_read__doc__,
"read($self, /, n=-1)\n"
"--\n"
"\n"
"Read up to n bytes. \n"
"\n"
"If n is not provided, or set to -1, reads as much bytes as possible.\n"
"\n"
"If EOF was received and the internal buffer is empty, return an empty bytes object.");

#define NS_STREAM_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))ns_Stream_read, METH_FASTCALL|METH_KEYWORDS, ns_Stream_read__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_read_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_Stream_read(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"n", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[1];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    Py_ssize_t n = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!ssize_t_converter(args[0], &n)) {
        goto exit;
    }
skip_optional_pos:
    return_value = ns_Stream_read_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_Stream_readexactly__doc__,
"readexactly($self, /, n)\n"
"--\n"
"\n"
"Read exactly n bytes.\n"
"\n"
"Raise an IncompleteReadError if EOF is reached before n can be read. \n"
"Use the IncompleteReadError.partial attribute to get the partially read data.\n"
"\n"
"Returned value is not limited by the configured stream limit, if hard limit is not set.");

#define NS_STREAM_READEXACTLY_METHODDEF    \
    {"readexactly", (PyCFunction)(void(*)(void))ns_Stream_readexactly, METH_FASTCALL|METH_KEYWORDS, ns_Stream_readexactly__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_readexactly_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_Stream_readexactly(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"n", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "readexactly", 0};
    PyObject *argsbuf[1];
    Py_ssize_t n;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!ssize_t_converter(args[0], &n)) {
        goto exit;
    }
    return_value = ns_Stream_readexactly_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_Stream_readuntil__doc__,
"readuntil($self, /, c)\n"
"--\n"
"\n"
"Read data from the stream until c is found.\n"
"\n"
"Raise an IncompleteReadError if EOF is reached before n can be found.\n"
"Use the IncompleteReadError.partial attribute to get the partially read data.\n"
"\n"
"If the amount of data read exceeds the configured stream hard limit, a LimitOverrunError exception is raised, \n"
"and the data is left in the internal buffer and can be read again.\n"
"\n"
"Returned value is not limited by the configured stream limit, if hard limit is not set.");

#define NS_STREAM_READUNTIL_METHODDEF    \
    {"readuntil", (PyCFunction)(void(*)(void))ns_Stream_readuntil, METH_FASTCALL|METH_KEYWORDS, ns_Stream_readuntil__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_readuntil_impl(Stream *self, char c);

static PyObject *
ns_Stream_readuntil(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"c", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "readuntil", 0};
    PyObject *argsbuf[1];
    char c;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyBytes_Check(args[0]) && PyBytes_GET_SIZE(args[0]) == 1) {
        c = PyBytes_AS_STRING(args[0])[0];
    }
    else if (PyByteArray_Check(args[0]) && PyByteArray_GET_SIZE(args[0]) == 1) {
        c = PyByteArray_AS_STRING(args[0])[0];
    }
    else {
        _PyArg_BadArgument("readuntil", "argument 'c'", "a byte string of length 1", args[0]);
        goto exit;
    }
    return_value = ns_Stream_readuntil_impl(self, c);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_Stream_get_error__doc__,
"get_error($self, /)\n"
"--\n"
"\n"
"Get the current error code (EOF is not error, so normally should be zero).");

#define NS_STREAM_GET_ERROR_METHODDEF    \
    {"get_error", (PyCFunction)ns_Stream_get_error, METH_NOARGS, ns_Stream_get_error__doc__},

Py_LOCAL_INLINE(int)
ns_Stream_get_error_impl(Stream *self);

static PyObject *
ns_Stream_get_error(Stream *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = ns_Stream_get_error_impl(self);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyLong_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_Stream_at_eof__doc__,
"at_eof($self, /)\n"
"--\n"
"\n"
"\n"
"True if EOF is reached.");

#define NS_STREAM_AT_EOF_METHODDEF    \
    {"at_eof", (PyCFunction)ns_Stream_at_eof, METH_NOARGS, ns_Stream_at_eof__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_Stream_at_eof_impl(Stream *self);

static PyObject *
ns_Stream_at_eof(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_Stream_at_eof_impl(self);
}
/*[clinic end generated code: output=888f266530fcddeb input=a9049054013a1b77]*/
