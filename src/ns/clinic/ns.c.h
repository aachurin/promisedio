/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(ns_getaddrinfo__doc__,
"getaddrinfo($module, /, node, service, family=0, type=0, proto=0,\n"
"            flags=0)\n"
"--\n"
"\n"
"Translate the host/port argument into a sequence of 5-tuples that contain all the necessary arguments for \n"
"creating a socket connected to that service. _host_ is a domain name, a string representation of an IPv4/v6 address or _None_.\n"
"_port_ is a string service name such as \'http\', a numeric port number or _None_. \n"
"\n"
"For more information, see [getaddrinfo](https://docs.python.org/3/library/socket.html#socket.getaddrinfo)\n"
"\n"
"");

#define NS_GETADDRINFO_METHODDEF    \
    {"getaddrinfo", (PyCFunction)(void(*)(void))ns_getaddrinfo, METH_FASTCALL|METH_KEYWORDS, ns_getaddrinfo__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_getaddrinfo_impl(PyObject *module, const char *node, PyObject *service,
                    int family, int type, int proto, int flags);

static PyObject *
ns_getaddrinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_getaddrinfo_impl(module, node, service, family, type, proto, flags);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_getnameinfo__doc__,
"getnameinfo($module, /, sockaddr, flags)\n"
"--\n"
"\n"
"Translate a socket address sockaddr into a 2-tuple (host, port). \n"
"\n"
"Depending on the settings of flags, the result can contain a fully-qualified domain name or \n"
"numeric address representation in host.\n"
"Similarly, port can contain a string port name or a numeric port number.\n"
"\n"
"For more information about flags you can consult [getnameinfo(3)](https://man7.org/linux/man-pages/man3/getnameinfo.3.html)\n"
"\n"
"");

#define NS_GETNAMEINFO_METHODDEF    \
    {"getnameinfo", (PyCFunction)(void(*)(void))ns_getnameinfo, METH_FASTCALL|METH_KEYWORDS, ns_getnameinfo__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_getnameinfo_impl(PyObject *module, sockaddr_any *sockaddr, int flags);

static PyObject *
ns_getnameinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_getnameinfo_impl(module, &sockaddr, flags);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_open_connection__doc__,
"open_connection($module, /, remote_addr, *,\n"
"                local_addr=<unrepresentable>, limit=-1, chunk_size=-1,\n"
"                nodelay=-1, keepalive=-1, ssl=<unrepresentable>,\n"
"                server_hostname=<unrepresentable>, timeout=0.0)\n"
"--\n"
"\n"
"Establish an IPv4 or IPv6 TCP connection.\n"
"\n"
"_limit_: The internal buffer size limit used by the _Stream_ instance. By default, the limit is set to 64 KiB.\n"
"_chunk_size_: The minimum size of one chunk of the internal buffer. By default, is set to 496 bytes (on x64).\n"
"_nodelay_: Enable/disable TCP_NODELAY. Set 1 to disable Nagle\'s algorithm.\n"
"_keepalive_: Enable/disable TCP keep-alive. The value is the initial delay in seconds. Set 0 to disable TCP keep-alive.\n"
"\n"
"");

#define NS_OPEN_CONNECTION_METHODDEF    \
    {"open_connection", (PyCFunction)(void(*)(void))ns_open_connection, METH_FASTCALL|METH_KEYWORDS, ns_open_connection__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_open_connection_impl(PyObject *module, sockaddr_any *remote_addr,
                        PyObject *local_addr, Py_ssize_t limit,
                        Py_ssize_t chunk_size, int nodelay, int keepalive,
                        PyObject *ssl, PyObject *server_hostname,
                        double timeout);

static PyObject *
ns_open_connection(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"remote_addr", "local_addr", "limit", "chunk_size", "nodelay", "keepalive", "ssl", "server_hostname", "timeout", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "open_connection", 0};
    PyObject *argsbuf[9];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    sockaddr_any remote_addr;
    PyObject *local_addr = NULL;
    Py_ssize_t limit = -1;
    Py_ssize_t chunk_size = -1;
    int nodelay = -1;
    int keepalive = -1;
    PyObject *ssl = NULL;
    PyObject *server_hostname = NULL;
    double timeout = 0.0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!inet_addr_converter(args[0], &remote_addr)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    if (args[1]) {
        local_addr = args[1];
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[2]) {
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
            limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[3]) {
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = _PyNumber_Index(args[3]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            chunk_size = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[4]) {
        nodelay = _PyLong_AsInt(args[4]);
        if (nodelay == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[5]) {
        keepalive = _PyLong_AsInt(args[5]);
        if (keepalive == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[6]) {
        if (!PyObject_TypeCheck(args[6], _CTX_get_module(module)->SSLContextType)) {
            _PyArg_BadArgument("open_connection", "argument 'ssl'", (_CTX_get_module(module)->SSLContextType)->tp_name, args[6]);
            goto exit;
        }
        ssl = args[6];
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[7]) {
        server_hostname = args[7];
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (PyFloat_CheckExact(args[8])) {
        timeout = PyFloat_AS_DOUBLE(args[8]);
    }
    else
    {
        timeout = PyFloat_AsDouble(args[8]);
        if (timeout == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
skip_optional_kwonly:
    return_value = ns_open_connection_impl(module, &remote_addr, local_addr, limit, chunk_size, nodelay, keepalive, ssl, server_hostname, timeout);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_set_tcp_nodelay__doc__,
"set_tcp_nodelay($self, /, enabled)\n"
"--\n"
"\n"
"Enable/disable TCP_NODELAY, which disables Nagle\'s algorithm.\n"
"\n"
"");

#define NS_TCPSTREAM_SET_TCP_NODELAY_METHODDEF    \
    {"set_tcp_nodelay", (PyCFunction)(void(*)(void))ns_TcpStream_set_tcp_nodelay, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_set_tcp_nodelay__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_set_tcp_nodelay_impl(Stream *self, int enabled);

static PyObject *
ns_TcpStream_set_tcp_nodelay(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"enabled", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "set_tcp_nodelay", 0};
    PyObject *argsbuf[1];
    int enabled;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    enabled = _PyLong_AsInt(args[0]);
    if (enabled == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = ns_TcpStream_set_tcp_nodelay_impl(self, enabled);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_set_tcp_keepalive__doc__,
"set_tcp_keepalive($self, /, delay)\n"
"--\n"
"\n"
"Enable/disable TCP keep-alive. Set zero to disable.\n"
"\n"
"");

#define NS_TCPSTREAM_SET_TCP_KEEPALIVE_METHODDEF    \
    {"set_tcp_keepalive", (PyCFunction)(void(*)(void))ns_TcpStream_set_tcp_keepalive, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_set_tcp_keepalive__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_set_tcp_keepalive_impl(Stream *self, int delay);

static PyObject *
ns_TcpStream_set_tcp_keepalive(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"delay", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "set_tcp_keepalive", 0};
    PyObject *argsbuf[1];
    int delay;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    delay = _PyLong_AsInt(args[0]);
    if (delay == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = ns_TcpStream_set_tcp_keepalive_impl(self, delay);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_getpeername__doc__,
"getpeername($self, /)\n"
"--\n"
"\n"
"Get the current address to which the handle is bound (For TCP only)\n"
"\n"
"");

#define NS_TCPSTREAM_GETPEERNAME_METHODDEF    \
    {"getpeername", (PyCFunction)ns_TcpStream_getpeername, METH_NOARGS, ns_TcpStream_getpeername__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_getpeername_impl(Stream *self);

static PyObject *
ns_TcpStream_getpeername(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_TcpStream_getpeername_impl(self);
}

PyDoc_STRVAR(ns_TcpStream_getsockname__doc__,
"getsockname($self, /)\n"
"--\n"
"\n"
"Return the socket\'s own address (For TCP only).\n"
"\n"
"");

#define NS_TCPSTREAM_GETSOCKNAME_METHODDEF    \
    {"getsockname", (PyCFunction)ns_TcpStream_getsockname, METH_NOARGS, ns_TcpStream_getsockname__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_getsockname_impl(Stream *self);

static PyObject *
ns_TcpStream_getsockname(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_TcpStream_getsockname_impl(self);
}

PyDoc_STRVAR(ns_TcpStream_write__doc__,
"write($self, /, data)\n"
"--\n"
"\n"
"Write data to the stream.\n"
"\n"
"");

#define NS_TCPSTREAM_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))ns_TcpStream_write, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_write__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_write_impl(Stream *self, PyObject *data);

static PyObject *
ns_TcpStream_write(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_TcpStream_write_impl(self, data);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_read__doc__,
"read($self, /, n=-1)\n"
"--\n"
"\n"
"Read up to n bytes. \n"
"\n"
"If n is not provided, or set to -1, reads as much bytes as possible.\n"
"If EOF was received and the internal buffer is empty, return an empty bytes object.\n"
"\n"
"");

#define NS_TCPSTREAM_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))ns_TcpStream_read, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_read__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_read_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_TcpStream_read(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_TcpStream_read_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_readexactly__doc__,
"readexactly($self, /, n)\n"
"--\n"
"\n"
"Read exactly n bytes.\n"
"\n"
"Raise an IncompleteReadError if EOF is reached before n can be read. \n"
"Use the IncompleteReadError.partial attribute to get the partially read data.\n"
"\n"
"Returned value is not limited by the configured stream limit.\n"
"\n"
"");

#define NS_TCPSTREAM_READEXACTLY_METHODDEF    \
    {"readexactly", (PyCFunction)(void(*)(void))ns_TcpStream_readexactly, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_readexactly__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_readexactly_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_TcpStream_readexactly(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_TcpStream_readexactly_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_readuntil__doc__,
"readuntil($self, /, c)\n"
"--\n"
"\n"
"Read data from the stream until c is found.\n"
"\n"
"Raise an IncompleteReadError if EOF is reached before n can be found.\n"
"Use the IncompleteReadError.partial attribute to get the partially read data.\n"
"\n"
"If the amount of data read exceeds the configured stream limit, a LimitOverrunError exception is raised, \n"
"and the data is left in the internal buffer and can be read again.\n"
"\n"
"");

#define NS_TCPSTREAM_READUNTIL_METHODDEF    \
    {"readuntil", (PyCFunction)(void(*)(void))ns_TcpStream_readuntil, METH_FASTCALL|METH_KEYWORDS, ns_TcpStream_readuntil__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_readuntil_impl(Stream *self, char c);

static PyObject *
ns_TcpStream_readuntil(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_TcpStream_readuntil_impl(self, c);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpStream_shutdown__doc__,
"shutdown($self, /)\n"
"--\n"
"\n"
"Shut down the stream.\n"
"\n"
"");

#define NS_TCPSTREAM_SHUTDOWN_METHODDEF    \
    {"shutdown", (PyCFunction)ns_TcpStream_shutdown, METH_NOARGS, ns_TcpStream_shutdown__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_shutdown_impl(Stream *self);

static PyObject *
ns_TcpStream_shutdown(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_TcpStream_shutdown_impl(self);
}

PyDoc_STRVAR(ns_TcpStream_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Close the stream handle.\n"
"\n"
"");

#define NS_TCPSTREAM_CLOSE_METHODDEF    \
    {"close", (PyCFunction)ns_TcpStream_close, METH_NOARGS, ns_TcpStream_close__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpStream_close_impl(Stream *self);

static PyObject *
ns_TcpStream_close(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_TcpStream_close_impl(self);
}

PyDoc_STRVAR(ns_open_unix_connection__doc__,
"open_unix_connection($module, /, path, *, limit=-1, chunk_size=-1,\n"
"                     ssl=<unrepresentable>,\n"
"                     server_hostname=<unrepresentable>)\n"
"--\n"
"\n"
"");

#define NS_OPEN_UNIX_CONNECTION_METHODDEF    \
    {"open_unix_connection", (PyCFunction)(void(*)(void))ns_open_unix_connection, METH_FASTCALL|METH_KEYWORDS, ns_open_unix_connection__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_open_unix_connection_impl(PyObject *module, PyObject *path,
                             Py_ssize_t limit, Py_ssize_t chunk_size,
                             PyObject *ssl, PyObject *server_hostname);

static PyObject *
ns_open_unix_connection(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "limit", "chunk_size", "ssl", "server_hostname", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "open_unix_connection", 0};
    PyObject *argsbuf[5];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    Py_ssize_t limit = -1;
    Py_ssize_t chunk_size = -1;
    PyObject *ssl = NULL;
    PyObject *server_hostname = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
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
            limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[2]) {
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
            chunk_size = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[3]) {
        if (!PyObject_TypeCheck(args[3], _CTX_get_module(module)->SSLContextType)) {
            _PyArg_BadArgument("open_unix_connection", "argument 'ssl'", (_CTX_get_module(module)->SSLContextType)->tp_name, args[3]);
            goto exit;
        }
        ssl = args[3];
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    server_hostname = args[4];
skip_optional_kwonly:
    return_value = ns_open_unix_connection_impl(module, path, limit, chunk_size, ssl, server_hostname);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(ns_UnixStream_getpeername__doc__,
"getpeername($self, /)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_GETPEERNAME_METHODDEF    \
    {"getpeername", (PyCFunction)ns_UnixStream_getpeername, METH_NOARGS, ns_UnixStream_getpeername__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_getpeername_impl(Stream *self);

static PyObject *
ns_UnixStream_getpeername(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_UnixStream_getpeername_impl(self);
}

PyDoc_STRVAR(ns_UnixStream_getsockname__doc__,
"getsockname($self, /)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_GETSOCKNAME_METHODDEF    \
    {"getsockname", (PyCFunction)ns_UnixStream_getsockname, METH_NOARGS, ns_UnixStream_getsockname__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_getsockname_impl(Stream *self);

static PyObject *
ns_UnixStream_getsockname(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_UnixStream_getsockname_impl(self);
}

PyDoc_STRVAR(ns_UnixStream_write__doc__,
"write($self, /, data)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))ns_UnixStream_write, METH_FASTCALL|METH_KEYWORDS, ns_UnixStream_write__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_write_impl(Stream *self, PyObject *data);

static PyObject *
ns_UnixStream_write(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_UnixStream_write_impl(self, data);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_UnixStream_read__doc__,
"read($self, /, n=-1)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))ns_UnixStream_read, METH_FASTCALL|METH_KEYWORDS, ns_UnixStream_read__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_read_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_UnixStream_read(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_UnixStream_read_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_UnixStream_readexactly__doc__,
"readexactly($self, /, n)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_READEXACTLY_METHODDEF    \
    {"readexactly", (PyCFunction)(void(*)(void))ns_UnixStream_readexactly, METH_FASTCALL|METH_KEYWORDS, ns_UnixStream_readexactly__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_readexactly_impl(Stream *self, Py_ssize_t n);

static PyObject *
ns_UnixStream_readexactly(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_UnixStream_readexactly_impl(self, n);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_UnixStream_readuntil__doc__,
"readuntil($self, /, c)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_READUNTIL_METHODDEF    \
    {"readuntil", (PyCFunction)(void(*)(void))ns_UnixStream_readuntil, METH_FASTCALL|METH_KEYWORDS, ns_UnixStream_readuntil__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_readuntil_impl(Stream *self, char c);

static PyObject *
ns_UnixStream_readuntil(Stream *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = ns_UnixStream_readuntil_impl(self, c);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_UnixStream_shutdown__doc__,
"shutdown($self, /)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_SHUTDOWN_METHODDEF    \
    {"shutdown", (PyCFunction)ns_UnixStream_shutdown, METH_NOARGS, ns_UnixStream_shutdown__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_shutdown_impl(Stream *self);

static PyObject *
ns_UnixStream_shutdown(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_UnixStream_shutdown_impl(self);
}

PyDoc_STRVAR(ns_UnixStream_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"");

#define NS_UNIXSTREAM_CLOSE_METHODDEF    \
    {"close", (PyCFunction)ns_UnixStream_close, METH_NOARGS, ns_UnixStream_close__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixStream_close_impl(Stream *self);

static PyObject *
ns_UnixStream_close(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_UnixStream_close_impl(self);
}

PyDoc_STRVAR(ns_start_server__doc__,
"start_server($module, /, client_connected_cb, addr, *, limit=-1,\n"
"             chunk_size=-1, backlog=100, nodelay=-1, keepalive=-1,\n"
"             simultaneous_accepts=1, ssl=<unrepresentable>)\n"
"--\n"
"\n"
"");

#define NS_START_SERVER_METHODDEF    \
    {"start_server", (PyCFunction)(void(*)(void))ns_start_server, METH_FASTCALL|METH_KEYWORDS, ns_start_server__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_start_server_impl(PyObject *module, PyObject *client_connected_cb,
                     sockaddr_any *addr, Py_ssize_t limit,
                     Py_ssize_t chunk_size, int backlog, int nodelay,
                     int keepalive, int simultaneous_accepts, PyObject *ssl);

static PyObject *
ns_start_server(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"client_connected_cb", "addr", "limit", "chunk_size", "backlog", "nodelay", "keepalive", "simultaneous_accepts", "ssl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "start_server", 0};
    PyObject *argsbuf[9];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *client_connected_cb;
    sockaddr_any addr;
    Py_ssize_t limit = -1;
    Py_ssize_t chunk_size = -1;
    int backlog = 100;
    int nodelay = -1;
    int keepalive = -1;
    int simultaneous_accepts = 1;
    PyObject *ssl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    client_connected_cb = args[0];
    if (!inet_addr_converter(args[1], &addr)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    if (args[2]) {
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
            limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[3]) {
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = _PyNumber_Index(args[3]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            chunk_size = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[4]) {
        backlog = _PyLong_AsInt(args[4]);
        if (backlog == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[5]) {
        nodelay = _PyLong_AsInt(args[5]);
        if (nodelay == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[6]) {
        keepalive = _PyLong_AsInt(args[6]);
        if (keepalive == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[7]) {
        simultaneous_accepts = _PyLong_AsInt(args[7]);
        if (simultaneous_accepts == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (!PyObject_TypeCheck(args[8], _CTX_get_module(module)->SSLContextType)) {
        _PyArg_BadArgument("start_server", "argument 'ssl'", (_CTX_get_module(module)->SSLContextType)->tp_name, args[8]);
        goto exit;
    }
    ssl = args[8];
skip_optional_kwonly:
    return_value = ns_start_server_impl(module, client_connected_cb, &addr, limit, chunk_size, backlog, nodelay, keepalive, simultaneous_accepts, ssl);

exit:
    return return_value;
}

PyDoc_STRVAR(ns_TcpServer_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"");

#define NS_TCPSERVER_CLOSE_METHODDEF    \
    {"close", (PyCFunction)ns_TcpServer_close, METH_NOARGS, ns_TcpServer_close__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_TcpServer_close_impl(Server *self);

static PyObject *
ns_TcpServer_close(Server *self, PyObject *Py_UNUSED(ignored))
{
    return ns_TcpServer_close_impl(self);
}

PyDoc_STRVAR(ns_start_unix_server__doc__,
"start_unix_server($module, /, client_connected_cb, path, *, limit=-1,\n"
"                  chunk_size=-1, backlog=100, ssl=<unrepresentable>)\n"
"--\n"
"\n"
"");

#define NS_START_UNIX_SERVER_METHODDEF    \
    {"start_unix_server", (PyCFunction)(void(*)(void))ns_start_unix_server, METH_FASTCALL|METH_KEYWORDS, ns_start_unix_server__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_start_unix_server_impl(PyObject *module, PyObject *client_connected_cb,
                          PyObject *path, Py_ssize_t limit,
                          Py_ssize_t chunk_size, int backlog, PyObject *ssl);

static PyObject *
ns_start_unix_server(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"client_connected_cb", "path", "limit", "chunk_size", "backlog", "ssl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "start_unix_server", 0};
    PyObject *argsbuf[6];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *client_connected_cb;
    PyObject *path = NULL;
    Py_ssize_t limit = -1;
    Py_ssize_t chunk_size = -1;
    int backlog = 100;
    PyObject *ssl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    client_connected_cb = args[0];
    if (!PyUnicode_FSConverter(args[1], &path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    if (args[2]) {
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
            limit = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[3]) {
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = _PyNumber_Index(args[3]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            chunk_size = ival;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (args[4]) {
        backlog = _PyLong_AsInt(args[4]);
        if (backlog == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_kwonly;
        }
    }
    if (!PyObject_TypeCheck(args[5], _CTX_get_module(module)->SSLContextType)) {
        _PyArg_BadArgument("start_unix_server", "argument 'ssl'", (_CTX_get_module(module)->SSLContextType)->tp_name, args[5]);
        goto exit;
    }
    ssl = args[5];
skip_optional_kwonly:
    return_value = ns_start_unix_server_impl(module, client_connected_cb, path, limit, chunk_size, backlog, ssl);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(ns_UnixServer_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"");

#define NS_UNIXSERVER_CLOSE_METHODDEF    \
    {"close", (PyCFunction)ns_UnixServer_close, METH_NOARGS, ns_UnixServer_close__doc__},

Py_LOCAL_INLINE(PyObject *)
ns_UnixServer_close_impl(Server *self);

static PyObject *
ns_UnixServer_close(Server *self, PyObject *Py_UNUSED(ignored))
{
    return ns_UnixServer_close_impl(self);
}
/*[clinic end generated code: output=d633bd56a8aaa751 input=a9049054013a1b77]*/
