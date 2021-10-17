/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(nsmodule_getaddrinfo__doc__,
"getaddrinfo($module, /, node, service, family=0, type=0, proto=0,\n"
"            flags=0)\n"
"--\n"
"\n"
"Translate the host/port argument into a sequence of 5-tuples that contain all the necessary arguments for \n"
"creating a socket connected to that service. host is a domain name, a string representation of an IPv4/v6 address or None.\n"
"port is a string service name such as \'http\', a numeric port number or None. \n"
"\n"
"For more information, see [getaddrinfo](https://docs.python.org/3/library/socket.html#socket.getaddrinfo)");

#define NSMODULE_GETADDRINFO_METHODDEF    \
    {"getaddrinfo", (PyCFunction)(void(*)(void))nsmodule_getaddrinfo, METH_FASTCALL|METH_KEYWORDS, nsmodule_getaddrinfo__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_getaddrinfo_impl(PyObject *module, const char *node,
                          PyObject *service, int family, int type, int proto,
                          int flags);

static PyObject *
nsmodule_getaddrinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"node", "service", "family", "type", "proto", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "getaddrinfo", 0};
    PyObject *argsbuf[6];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    const char *node = NULL;
    PyObject *service;
    int family = AF_UNSPEC;
    int type = 0;
    int proto = 0;
    int flags = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 6, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!cstring_optional_converter(args[0], &node)) {
        goto exit;
    }
    service = args[1];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[2]) {
        family = _PyLong_AsInt(args[2]);
        if (family == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[3]) {
        type = _PyLong_AsInt(args[3]);
        if (type == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[4]) {
        proto = _PyLong_AsInt(args[4]);
        if (proto == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    flags = _PyLong_AsInt(args[5]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = nsmodule_getaddrinfo_impl(module, node, service, family, type, proto, flags);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_getnameinfo__doc__,
"getnameinfo($module, /, sockaddr, flags)\n"
"--\n"
"\n"
"Translate a socket address sockaddr into a 2-tuple (host, port). \n"
"\n"
"Depending on the settings of flags, the result can contain a fully-qualified domain name or \n"
"numeric address representation in host.\n"
"Similarly, port can contain a string port name or a numeric port number.\n"
"\n"
"For more information about flags you can consult [getnameinfo(3)](https://man7.org/linux/man-pages/man3/getnameinfo.3.html)");

#define NSMODULE_GETNAMEINFO_METHODDEF    \
    {"getnameinfo", (PyCFunction)(void(*)(void))nsmodule_getnameinfo, METH_FASTCALL|METH_KEYWORDS, nsmodule_getnameinfo__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_getnameinfo_impl(PyObject *module, sockaddr_any *sockaddr,
                          int flags);

static PyObject *
nsmodule_getnameinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"sockaddr", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "getnameinfo", 0};
    PyObject *argsbuf[2];
    sockaddr_any sockaddr;
    int flags;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!inet_addr_converter(args[0], &sockaddr)) {
        goto exit;
    }
    flags = _PyLong_AsInt(args[1]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = nsmodule_getnameinfo_impl(module, &sockaddr, flags);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_getsockname__doc__,
"getsockname($self, /)\n"
"--\n"
"\n"
"\n"
"Return the socket\'s own address (For TCP only).");

#define NSMODULE_STREAM_GETSOCKNAME_METHODDEF    \
    {"getsockname", (PyCFunction)nsmodule_Stream_getsockname, METH_NOARGS, nsmodule_Stream_getsockname__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_getsockname_impl(Stream *self);

static PyObject *
nsmodule_Stream_getsockname(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return nsmodule_Stream_getsockname_impl(self);
}

PyDoc_STRVAR(nsmodule_Stream_getpeername__doc__,
"getpeername($self, /)\n"
"--\n"
"\n"
"\n"
"Get the current address to which the handle is bound (For TCP only)");

#define NSMODULE_STREAM_GETPEERNAME_METHODDEF    \
    {"getpeername", (PyCFunction)nsmodule_Stream_getpeername, METH_NOARGS, nsmodule_Stream_getpeername__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_getpeername_impl(Stream *self);

static PyObject *
nsmodule_Stream_getpeername(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return nsmodule_Stream_getpeername_impl(self);
}

PyDoc_STRVAR(nsmodule_Stream_write__doc__,
"write($self, /, data)\n"
"--\n"
"\n"
"\n"
"Write data to the stream.");

#define NSMODULE_STREAM_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))nsmodule_Stream_write, METH_FASTCALL|METH_KEYWORDS, nsmodule_Stream_write__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_write_impl(Stream *self, PyObject *data);

static PyObject *
nsmodule_Stream_write(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = nsmodule_Stream_write_impl(self, data);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_read__doc__,
"read($self, /, n=-1)\n"
"--\n"
"\n"
"Read up to n bytes. \n"
"\n"
"If n is not provided, or set to -1, reads as much bytes as possible.\n"
"\n"
"If EOF was received and the internal buffer is empty, return an empty bytes object.");

#define NSMODULE_STREAM_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))nsmodule_Stream_read, METH_FASTCALL|METH_KEYWORDS, nsmodule_Stream_read__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_read_impl(Stream *self, Py_ssize_t n);

static PyObject *
nsmodule_Stream_read(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = nsmodule_Stream_read_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_readexactly__doc__,
"readexactly($self, /, n)\n"
"--\n"
"\n"
"Read exactly n bytes.\n"
"\n"
"Raise an IncompleteReadError if EOF is reached before n can be read. \n"
"Use the IncompleteReadError.partial attribute to get the partially read data.\n"
"\n"
"Returned value is not limited by the configured stream limit, if hard limit is not set.");

#define NSMODULE_STREAM_READEXACTLY_METHODDEF    \
    {"readexactly", (PyCFunction)(void(*)(void))nsmodule_Stream_readexactly, METH_FASTCALL|METH_KEYWORDS, nsmodule_Stream_readexactly__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_readexactly_impl(Stream *self, Py_ssize_t n);

static PyObject *
nsmodule_Stream_readexactly(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = nsmodule_Stream_readexactly_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_readuntil__doc__,
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

#define NSMODULE_STREAM_READUNTIL_METHODDEF    \
    {"readuntil", (PyCFunction)(void(*)(void))nsmodule_Stream_readuntil, METH_FASTCALL|METH_KEYWORDS, nsmodule_Stream_readuntil__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_readuntil_impl(Stream *self, char c);

static PyObject *
nsmodule_Stream_readuntil(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = nsmodule_Stream_readuntil_impl(self, c);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_get_error__doc__,
"get_error($self, /)\n"
"--\n"
"\n"
"Get the current error code (EOF is not error, so normally should be zero).");

#define NSMODULE_STREAM_GET_ERROR_METHODDEF    \
    {"get_error", (PyCFunction)nsmodule_Stream_get_error, METH_NOARGS, nsmodule_Stream_get_error__doc__},

Py_LOCAL_INLINE(int)
nsmodule_Stream_get_error_impl(Stream *self);

static PyObject *
nsmodule_Stream_get_error(Stream *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = nsmodule_Stream_get_error_impl(self);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyLong_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(nsmodule_Stream_at_eof__doc__,
"at_eof($self, /)\n"
"--\n"
"\n"
"\n"
"True if EOF is reached.");

#define NSMODULE_STREAM_AT_EOF_METHODDEF    \
    {"at_eof", (PyCFunction)nsmodule_Stream_at_eof, METH_NOARGS, nsmodule_Stream_at_eof__doc__},

Py_LOCAL_INLINE(PyObject *)
nsmodule_Stream_at_eof_impl(Stream *self);

static PyObject *
nsmodule_Stream_at_eof(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return nsmodule_Stream_at_eof_impl(self);
}
/*[clinic end generated code: output=bf32bac5e42ba411 input=a9049054013a1b77]*/
