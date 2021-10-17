/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(fs_FileIO_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Close this handle.\n"
"\n"
"This method has no effect if the file is already closed. Once the file is closed,\n"
"any operation on the file (e.g. reading or writing) will raise a ValueError.\n"
"As a convenience, it is allowed to call this method more than once; only the first\n"
"call, however, will have an effect.\n"
"\n"
"");

#define FS_FILEIO_CLOSE_METHODDEF    \
    {"close", (PyCFunction)fs_FileIO_close, METH_NOARGS, fs_FileIO_close__doc__},

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_close_impl(FileIO *self);

static PyObject *
fs_FileIO_close(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return fs_FileIO_close_impl(self);
}

PyDoc_STRVAR(fs_FileIO_fileno__doc__,
"fileno($self, /)\n"
"--\n"
"\n"
"Return the underlying file descriptor (an integer).\n"
"\n"
"");

#define FS_FILEIO_FILENO_METHODDEF    \
    {"fileno", (PyCFunction)fs_FileIO_fileno, METH_NOARGS, fs_FileIO_fileno__doc__},

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_fileno_impl(FileIO *self);

static PyObject *
fs_FileIO_fileno(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return fs_FileIO_fileno_impl(self);
}

PyDoc_STRVAR(fs_FileIO_read__doc__,
"read($self, /, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.\n"
"\n"
"");

#define FS_FILEIO_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))fs_FileIO_read, METH_FASTCALL|METH_KEYWORDS, fs_FileIO_read__doc__},

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_off_t offset);

static PyObject *
fs_FileIO_read(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    Py_ssize_t size = -1;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        if (!ssize_t_converter(args[0], &size)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (!off_t_converter(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fs_FileIO_read_impl(self, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fs_FileIO_write__doc__,
"write($self, /, data, offset=-1)\n"
"--\n"
"\n"
"Write the given bytes to the IO handle.\n"
"\n"
"Returns the number of bytes written, which is always the length of data in bytes.\n"
"\n"
"");

#define FS_FILEIO_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))fs_FileIO_write, METH_FASTCALL|METH_KEYWORDS, fs_FileIO_write__doc__},

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_write_impl(FileIO *self, PyObject *data, Py_off_t offset);

static PyObject *
fs_FileIO_write(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *data;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    data = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!off_t_converter(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fs_FileIO_write_impl(self, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fs_FileIO_seek__doc__,
"seek($self, /, offset, whence=0)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.\n"
"\n"
"");

#define FS_FILEIO_SEEK_METHODDEF    \
    {"seek", (PyCFunction)(void(*)(void))fs_FileIO_seek, METH_FASTCALL|METH_KEYWORDS, fs_FileIO_seek__doc__},

Py_LOCAL_INLINE(PyObject *)
fs_FileIO_seek_impl(FileIO *self, Py_off_t offset, int whence);

static PyObject *
fs_FileIO_seek(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"offset", "whence", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "seek", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    Py_off_t offset;
    int whence = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!off_t_converter(args[0], &offset)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    whence = _PyLong_AsInt(args[1]);
    if (whence == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fs_FileIO_seek_impl(self, offset, whence);

exit:
    return return_value;
}
/*[clinic end generated code: output=9a7c9ef9f370bd67 input=a9049054013a1b77]*/
