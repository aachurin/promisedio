// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include "fileio.h"
#include "promise.h"
#include "fs.h"
#include "clinic/fileio.c.h"

/*[clinic input]
class FileIO "FileIO *" "&FileIOType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=983f610ae51ad762]*/

/*[clinic input]
FileIO.close

Close this stream.

This method has no effect if the file is already closed. Once the file is closed,
any operation on the file (e.g. reading or writing) will raise a ValueError.
As a convenience, it is allowed to call this method more than once; only the first
call, however, will have an effect.
[clinic start generated code]*/

static PyObject *
FileIO_close_impl(FileIO *self)
/*[clinic end generated code: output=f3d2268c94d7c395 input=813c5ada7c68198a]*/
{
    if (self->fd < 0) {
        Promise_RETURN_NONE();
    } else if (!self->closefd) {
        self->fd = -1;
        Promise_RETURN_NONE();
    } else {
        int fd = self->fd;
        self->fd = -1;
        return (PyObject *) Fs_close(fd);
    }
}

static inline PyObject *
fileio_err_closed(void)
{
    PyErr_SetString(PyExc_ValueError, "I/O operation on closed file");
    return NULL;
}

/*[clinic input]
FileIO.fileno

Return the underlying file descriptor (an integer).
[clinic start generated code]*/

static PyObject *
FileIO_fileno_impl(FileIO *self)
/*[clinic end generated code: output=76b8d465df7196d5 input=a850a18a1a266f93]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    return PyLong_FromLong((long) self->fd);
}

/*[clinic input]
FileIO.read
    size: Py_ssize_t(accept={int, NoneType}) = -1
    offset: Py_ssize_t(accept={int, NoneType}) = -1

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

static PyObject *
FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_ssize_t offset)
/*[clinic end generated code: output=b01f552b6414314d input=fea665f959c55e39]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    if (size > _PY_READ_MAX) {
        size = _PY_READ_MAX;
    }
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be specified in conjunction with the size parameter");
            return NULL;
        }
        return (PyObject *) Fs_readall(self->fd);
    } else {
        return (PyObject *) Fs_read(self->fd, size, offset);
    }
}

/*[clinic input]
FileIO.write
    data: object
    offset: Py_ssize_t(accept={int, NoneType}) = -1

Write the given bytes to the IO stream.

Returns the number of bytes written, which is always the length of data in bytes.
[clinic start generated code]*/

static PyObject *
FileIO_write_impl(FileIO *self, PyObject *data, Py_ssize_t offset)
/*[clinic end generated code: output=f7e82b9007322940 input=f5f5b9fa18191ed0]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_write(self->fd, data, offset);
}

/*[clinic input]
FileIO.seek
    offset: Py_ssize_t
    whence: int = 0

Read at most size bytes, returned as bytes.
[clinic start generated code]*/

static PyObject *
FileIO_seek_impl(FileIO *self, Py_ssize_t offset, int whence)
/*[clinic end generated code: output=a1b1debad403afa1 input=2ba0c6448e2b699b]*/
{
    if (self->fd < 0)
        return fileio_err_closed();
    return (PyObject *) Fs_seek(self->fd, offset, whence);
}

static void
fileio_dealloc(FileIO *self)
{
    if (self->fd >= 0) {
        // close silently, ignore errors, sync
        int fd = self->fd;
        self->fd = -1;
        uv_loop_t *loop = Loop_Get();
        uv_fs_t req;
        BEGIN_ALLOW_THREADS
        uv_fs_close(loop, &req, fd, NULL);
        END_ALLOW_THREADS
    }
    PyObject_Del(self);
}

static PyObject *
get_closed(FileIO *self, void *closure)
{
    return PyBool_FromLong((long)(self->fd < 0));
}

static PyObject *
get_closefd(FileIO *self, void *closure)
{
    return PyBool_FromLong((long)(self->closefd));
}

static PyGetSetDef FileIOType_getsetlist[] = {
    {"closed", (getter)get_closed, NULL, "True if the file is closed"},
    {"closefd", (getter)get_closefd, NULL, "True if the file descriptor will be closed by close()."},
    {NULL},
};

static PyMethodDef FileIOType_methods[] = {
    FILEIO_FILENO_METHODDEF
    FILEIO_CLOSE_METHODDEF
    FILEIO_READ_METHODDEF
    FILEIO_WRITE_METHODDEF
    FILEIO_SEEK_METHODDEF
    {NULL}  /* Sentinel */
};

static PyTypeObject FileIOType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "FileIO",
    .tp_basicsize = sizeof(FileIO),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor) fileio_dealloc,
    .tp_methods = FileIOType_methods,
    .tp_getset = FileIOType_getsetlist
};

static FileIO *
new_fileobj(int fd, int closefd)
{
    FileIO *fileobj = PyObject_New(FileIO, &FileIOType);
    if (fileobj == NULL)
        return NULL;
    fileobj->fd = fd;
    fileobj->offset = 0;
    fileobj->closefd = closefd;
    return fileobj;
}

static PyObject *
open_fstat_done(PyObject *context, StatObj *value)
{
#if defined(S_ISDIR)
    // only for unix
    if (S_ISDIR(value->st.st_mode)) {
        PyObject *args = Py_BuildValue("(is)", EISDIR, uv_strerror(UV_EISDIR));
        if (args == NULL)
            return NULL;
        PyErr_SetObject(PyExc_OSError, args);
        Py_DECREF(args);
        Py_DECREF(context);
        return NULL;
    }
#endif
    return context;
}

static PyObject *
open_done(FileIO *context, PyObject *value)
{
    context->fd = _PyLong_AsInt(value);
    Promise *promise = Fs_fstat(context->fd);
    Promise_CALLBACK(promise, open_fstat_done, NULL, context);
    return (PyObject *) promise;
}

PyObject *
FileIO_Open(PyObject *file, const char *flags, int closefd)
{
    if (PyNumber_Check(file)) {
        int fd = _PyLong_AsInt(file);
        if (fd < 0) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "negative file descriptor");
            }
            return NULL;
        }
        FileIO *fileobj = new_fileobj(fd, closefd);
        if (fileobj == NULL)
            return NULL;
        Promise_RETURN_RESOLVED((PyObject *) fileobj);
    }
    if (!closefd) {
        PyErr_SetString(PyExc_ValueError, "Cannot use closefd=False with file name");
        return NULL;
    }
    PyObject *path = Fs_Path(file);
    if (path == NULL)
        return NULL;
    FileIO *fileobj = new_fileobj(-1, closefd);
    if (fileobj == NULL) {
        Py_DECREF(path);
        return NULL;
    }
    Promise *promise = Fs_open(PyBytes_AS_STRING(path), flags, 0666);
    Py_DECREF(path);
    if (promise == NULL) {
        Py_DECREF(fileobj);
        return NULL;
    }
    Promise_CALLBACK(promise, open_done, NULL, fileobj);
    return (PyObject *) promise;
}

int
FileIO_module_init()
{
    if (PyType_Ready(&FileIOType) < 0) {
        return -1;
    }
    return 0;
}
