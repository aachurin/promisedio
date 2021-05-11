/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(FileIO_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Close this stream.\n"
"\n"
"This method has no effect if the file is already closed. Once the file is closed,\n"
"any operation on the file (e.g. reading or writing) will raise a ValueError.\n"
"As a convenience, it is allowed to call this method more than once; only the first\n"
"call, however, will have an effect.");

#define FILEIO_CLOSE_METHODDEF    \
    {"close", (PyCFunction)FileIO_close, METH_NOARGS, FileIO_close__doc__},

static PyObject *
FileIO_close_impl(FileIO *self);

static PyObject *
FileIO_close(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return FileIO_close_impl(self);
}

PyDoc_STRVAR(FileIO_fileno__doc__,
"fileno($self, /)\n"
"--\n"
"\n"
"Return the underlying file descriptor (an integer).");

#define FILEIO_FILENO_METHODDEF    \
    {"fileno", (PyCFunction)FileIO_fileno, METH_NOARGS, FileIO_fileno__doc__},

static PyObject *
FileIO_fileno_impl(FileIO *self);

static PyObject *
FileIO_fileno(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return FileIO_fileno_impl(self);
}

PyDoc_STRVAR(FileIO_read__doc__,
"read($self, /, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.");

#define FILEIO_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))FileIO_read, METH_FASTCALL|METH_KEYWORDS, FileIO_read__doc__},

static PyObject *
FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_ssize_t offset);

static PyObject *
FileIO_read(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    Py_ssize_t size = -1;
    Py_ssize_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        if (!_Py_convert_optional_to_ssize_t(args[0], &size)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (!_Py_convert_optional_to_ssize_t(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = FileIO_read_impl(self, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(FileIO_write__doc__,
"write($self, /, data, offset=-1)\n"
"--\n"
"\n"
"Write the given bytes to the IO stream.\n"
"\n"
"Returns the number of bytes written, which is always the length of data in bytes.");

#define FILEIO_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))FileIO_write, METH_FASTCALL|METH_KEYWORDS, FileIO_write__doc__},

static PyObject *
FileIO_write_impl(FileIO *self, PyObject *data, Py_ssize_t offset);

static PyObject *
FileIO_write(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *data;
    Py_ssize_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    data = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!_Py_convert_optional_to_ssize_t(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = FileIO_write_impl(self, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(FileIO_seek__doc__,
"seek($self, /, offset, whence=0)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.");

#define FILEIO_SEEK_METHODDEF    \
    {"seek", (PyCFunction)(void(*)(void))FileIO_seek, METH_FASTCALL|METH_KEYWORDS, FileIO_seek__doc__},

static PyObject *
FileIO_seek_impl(FileIO *self, Py_ssize_t offset, int whence);

static PyObject *
FileIO_seek(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"offset", "whence", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "seek", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    Py_ssize_t offset;
    int whence = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = PyNumber_Index(args[0]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        offset = ival;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    whence = _PyLong_AsInt(args[1]);
    if (whence == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = FileIO_seek_impl(self, offset, whence);

exit:
    return return_value;
}
/*[clinic end generated code: output=84115d7a3bd00202 input=a9049054013a1b77]*/
