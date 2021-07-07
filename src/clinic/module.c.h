/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(promisedio__getallocatedobjectscount__doc__,
"_getallocatedobjectscount($module, /)\n"
"--\n"
"\n"
"Get number of allocated objects,\n"
"\n"
"");

#define PROMISEDIO__GETALLOCATEDOBJECTSCOUNT_METHODDEF    \
    {"_getallocatedobjectscount", (PyCFunction)promisedio__getallocatedobjectscount, METH_NOARGS, promisedio__getallocatedobjectscount__doc__},

static inline PyObject *
promisedio__getallocatedobjectscount_impl(PyObject *module);

static PyObject *
promisedio__getallocatedobjectscount(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__getallocatedobjectscount_impl(module);
}

PyDoc_STRVAR(promisedio__clearfreelists__doc__,
"_clearfreelists($module, /)\n"
"--\n"
"\n"
"Clear freelists\n"
"\n"
"");

#define PROMISEDIO__CLEARFREELISTS_METHODDEF    \
    {"_clearfreelists", (PyCFunction)promisedio__clearfreelists, METH_NOARGS, promisedio__clearfreelists__doc__},

static inline PyObject *
promisedio__clearfreelists_impl(PyObject *module);

static PyObject *
promisedio__clearfreelists(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__clearfreelists_impl(module);
}

PyDoc_STRVAR(promisedio_exec_async__doc__,
"exec_async($module, coro, /)\n"
"--\n"
"\n"
"Schedule coroutine execution.\n"
"\n"
"");

#define PROMISEDIO_EXEC_ASYNC_METHODDEF    \
    {"exec_async", (PyCFunction)promisedio_exec_async, METH_O, promisedio_exec_async__doc__},

static inline PyObject *
promisedio_exec_async_impl(PyObject *module, PyObject *coro);

static PyObject *
promisedio_exec_async(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    PyObject *coro;

    if (!PyObject_TypeCheck(arg, &PyCoro_Type)) {
        _PyArg_BadArgument("exec_async", "argument", (&PyCoro_Type)->tp_name, arg);
        goto exit;
    }
    coro = arg;
    return_value = promisedio_exec_async_impl(module, coro);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_process_promise_chain__doc__,
"process_promise_chain($module, /)\n"
"--\n"
"\n"
"Process promise async chain once.\n"
"\n"
"");

#define PROMISEDIO_PROCESS_PROMISE_CHAIN_METHODDEF    \
    {"process_promise_chain", (PyCFunction)promisedio_process_promise_chain, METH_NOARGS, promisedio_process_promise_chain__doc__},

static inline int
promisedio_process_promise_chain_impl(PyObject *module);

static PyObject *
promisedio_process_promise_chain(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = promisedio_process_promise_chain_impl(module);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyBool_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_deferred__doc__,
"deferred($module, /)\n"
"--\n"
"\n"
"Returns new Deferred object.");

#define PROMISEDIO_DEFERRED_METHODDEF    \
    {"deferred", (PyCFunction)promisedio_deferred, METH_NOARGS, promisedio_deferred__doc__},

static inline PyObject *
promisedio_deferred_impl(PyObject *module);

static PyObject *
promisedio_deferred(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_deferred_impl(module);
}

PyDoc_STRVAR(promisedio_use_python_default_sigint__doc__,
"use_python_default_sigint($module, /, flag)\n"
"--\n"
"\n"
"Use python default SIGINT handler.\n"
"\n"
"");

#define PROMISEDIO_USE_PYTHON_DEFAULT_SIGINT_METHODDEF    \
    {"use_python_default_sigint", (PyCFunction)(void(*)(void))promisedio_use_python_default_sigint, METH_FASTCALL|METH_KEYWORDS, promisedio_use_python_default_sigint__doc__},

static inline PyObject *
promisedio_use_python_default_sigint_impl(PyObject *module, int flag);

static PyObject *
promisedio_use_python_default_sigint(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"flag", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "use_python_default_sigint", 0};
    PyObject *argsbuf[1];
    int flag;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    flag = PyObject_IsTrue(args[0]);
    if (flag < 0) {
        goto exit;
    }
    return_value = promisedio_use_python_default_sigint_impl(module, flag);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_run_loop__doc__,
"run_loop($module, /)\n"
"--\n"
"\n"
"Run event loop.\n"
"\n"
"");

#define PROMISEDIO_RUN_LOOP_METHODDEF    \
    {"run_loop", (PyCFunction)promisedio_run_loop, METH_NOARGS, promisedio_run_loop__doc__},

static inline PyObject *
promisedio_run_loop_impl(PyObject *module);

static PyObject *
promisedio_run_loop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_run_loop_impl(module);
}

PyDoc_STRVAR(promisedio_close_loop__doc__,
"close_loop($module, /)\n"
"--\n"
"\n"
"Close event loop.\n"
"\n"
"");

#define PROMISEDIO_CLOSE_LOOP_METHODDEF    \
    {"close_loop", (PyCFunction)promisedio_close_loop, METH_NOARGS, promisedio_close_loop__doc__},

static inline PyObject *
promisedio_close_loop_impl(PyObject *module);

static PyObject *
promisedio_close_loop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_close_loop_impl(module);
}

PyDoc_STRVAR(promisedio_check_can_fork__doc__,
"check_can_fork($module, /)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_CHECK_CAN_FORK_METHODDEF    \
    {"check_can_fork", (PyCFunction)promisedio_check_can_fork, METH_NOARGS, promisedio_check_can_fork__doc__},

static inline PyObject *
promisedio_check_can_fork_impl(PyObject *module);

static PyObject *
promisedio_check_can_fork(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_check_can_fork_impl(module);
}

PyDoc_STRVAR(promisedio__inspectloop__doc__,
"_inspectloop($module, /)\n"
"--\n"
"\n"
"Prints all handles associated with the given loop.\n"
"\n"
"Warning: This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.\n"
"\n"
"");

#define PROMISEDIO__INSPECTLOOP_METHODDEF    \
    {"_inspectloop", (PyCFunction)promisedio__inspectloop, METH_NOARGS, promisedio__inspectloop__doc__},

static inline PyObject *
promisedio__inspectloop_impl(PyObject *module);

static PyObject *
promisedio__inspectloop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__inspectloop_impl(module);
}

PyDoc_STRVAR(promisedio_sleep__doc__,
"sleep($module, /, seconds)\n"
"--\n"
"\n"
"Delay execution for a given number of seconds.");

#define PROMISEDIO_SLEEP_METHODDEF    \
    {"sleep", (PyCFunction)(void(*)(void))promisedio_sleep, METH_FASTCALL|METH_KEYWORDS, promisedio_sleep__doc__},

static inline PyObject *
promisedio_sleep_impl(PyObject *module, double seconds);

static PyObject *
promisedio_sleep(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"seconds", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "sleep", 0};
    PyObject *argsbuf[1];
    double seconds;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_CheckExact(args[0])) {
        seconds = PyFloat_AS_DOUBLE(args[0]);
    }
    else
    {
        seconds = PyFloat_AsDouble(args[0]);
        if (seconds == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    return_value = promisedio_sleep_impl(module, seconds);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_stat__doc__,
"stat($module, /, path, *, follow_symlinks=True)\n"
"--\n"
"\n"
"Get the status of a file. Return a StatObj object.\n"
"\n"
"This function normally follows symlinks; to stat a symlink add the argument follow_symlinks=False\n"
"\n"
"Equivalent to [stat(2)](https://man7.org/linux/man-pages/man2/stat.2.html) \n"
"              [lstat(2)](https://man7.org/linux/man-pages/man2/lstat.2.html).");

#define PROMISEDIO_STAT_METHODDEF    \
    {"stat", (PyCFunction)(void(*)(void))promisedio_stat, METH_FASTCALL|METH_KEYWORDS, promisedio_stat__doc__},

static inline PyObject *
promisedio_stat_impl(PyObject *module, PyObject *path, int follow_symlinks);

static PyObject *
promisedio_stat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "stat", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int follow_symlinks = 1;

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
    follow_symlinks = PyObject_IsTrue(args[1]);
    if (follow_symlinks < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = promisedio_stat_impl(module, path, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_fstat__doc__,
"fstat($module, /, fd)\n"
"--\n"
"\n"
"Get the status of the file descriptor fd. Returns a StatObj object.\n"
"\n"
"Equivalent to [fstat(2)](https://man7.org/linux/man-pages/man2/fstat.2.html).");

#define PROMISEDIO_FSTAT_METHODDEF    \
    {"fstat", (PyCFunction)(void(*)(void))promisedio_fstat, METH_FASTCALL|METH_KEYWORDS, promisedio_fstat__doc__},

static inline PyObject *
promisedio_fstat_impl(PyObject *module, int fd);

static PyObject *
promisedio_fstat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fstat", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = promisedio_fstat_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_seek__doc__,
"seek($module, /, fd, pos, how)\n"
"--\n"
"\n"
"Set the current position of file descriptor fd to position pos, modified by how:\n"
"- SEEK_SET or 0 to set the position relative to the beginning of the file;\n"
"- SEEK_CUR or 1 to set it relative to the current position;\n"
"- SEEK_END or 2 to set it relative to the end of the file.\n"
"\n"
"Return the new cursor position in bytes, starting from the beginning.\n"
"\n"
"Equivalent to [lseek(2)](https://man7.org/linux/man-pages/man2/lseek.2.html).");

#define PROMISEDIO_SEEK_METHODDEF    \
    {"seek", (PyCFunction)(void(*)(void))promisedio_seek, METH_FASTCALL|METH_KEYWORDS, promisedio_seek__doc__},

static inline PyObject *
promisedio_seek_impl(PyObject *module, int fd, Py_off_t pos, int how);

static PyObject *
promisedio_seek(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "pos", "how", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "seek", 0};
    PyObject *argsbuf[3];
    int fd;
    Py_off_t pos;
    int how;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!off_t_converter(args[1], &pos)) {
        goto exit;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    how = _PyLong_AsInt(args[2]);
    if (how == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_seek_impl(module, fd, pos, how);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_open__doc__,
"open($module, /, path, mode=\'r\', closefd=True)\n"
"--\n"
"\n"
"Open file and return a corresponding file object. If the file cannot be opened, an OSError is raised. \n"
"\n"
"Equivalent to python [open](https://docs.python.org/3/library/functions.html#open) (binary mode only).");

#define PROMISEDIO_OPEN_METHODDEF    \
    {"open", (PyCFunction)(void(*)(void))promisedio_open, METH_FASTCALL|METH_KEYWORDS, promisedio_open__doc__},

static inline PyObject *
promisedio_open_impl(PyObject *module, PyObject *path, const char *mode,
                     int closefd);

static PyObject *
promisedio_open(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", "closefd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "open", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path;
    const char *mode = "r";
    int closefd = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    path = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        if (!cstring_converter(args[1], &mode)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    closefd = _PyLong_AsInt(args[2]);
    if (closefd == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_open_impl(module, path, mode, closefd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_openfd__doc__,
"openfd($module, /, path, flags, mode=438)\n"
"--\n"
"\n"
"Open the file path and set various flags according to flags and possibly its mode according to mode. \n"
"Return the file descriptor for the newly opened file. The new file descriptor is non-inheritable.\n"
"\n"
"Equivalent to [open(2)](https://man7.org/linux/man-pages/man2/open.2.html).");

#define PROMISEDIO_OPENFD_METHODDEF    \
    {"openfd", (PyCFunction)(void(*)(void))promisedio_openfd, METH_FASTCALL|METH_KEYWORDS, promisedio_openfd__doc__},

static inline PyObject *
promisedio_openfd_impl(PyObject *module, PyObject *path, int flags, int mode);

static PyObject *
promisedio_openfd(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "flags", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "openfd", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    int flags;
    int mode = 438;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    flags = _PyLong_AsInt(args[1]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[2]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_openfd_impl(module, path, flags, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_close__doc__,
"close($module, /, fd)\n"
"--\n"
"\n"
"Close file descriptor fd.\n"
"\n"
"Equivalent to [close(2)](https://man7.org/linux/man-pages/man2/close.2.html).");

#define PROMISEDIO_CLOSE_METHODDEF    \
    {"close", (PyCFunction)(void(*)(void))promisedio_close, METH_FASTCALL|METH_KEYWORDS, promisedio_close__doc__},

static inline PyObject *
promisedio_close_impl(PyObject *module, int fd);

static PyObject *
promisedio_close(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "close", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = promisedio_close_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_read__doc__,
"read($module, /, fd, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read from file descriptor fd until we have size characters or we hit EOF.\n"
"If offset is present and is not negative, read at most size bytes from file descriptor fd at a position of offset,\n"
"leaving the file offset unchanged.\n"
"\n"
"Equivalent to [read(2)](https://man7.org/linux/man-pages/man2/read.2.html)\n"
"              [preadv(2)](https://man7.org/linux/man-pages/man2/preadv.2.html)");

#define PROMISEDIO_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))promisedio_read, METH_FASTCALL|METH_KEYWORDS, promisedio_read__doc__},

static inline PyObject *
promisedio_read_impl(PyObject *module, int fd, Py_ssize_t size,
                     Py_off_t offset);

static PyObject *
promisedio_read(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    int fd;
    Py_ssize_t size = -1;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        if (!ssize_t_converter(args[1], &size)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_read_impl(module, fd, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_write__doc__,
"write($module, /, fd, data, offset=-1)\n"
"--\n"
"\n"
"Write the data to file descriptor fd.\n"
"If offset is present and is not negative, write the data to file descriptor fd at position of offset, \n"
"leaving the file offset unchanged.\n"
"\n"
"Return the number of bytes actually written.\n"
"\n"
"Equivalent to [write(2)](https://man7.org/linux/man-pages/man2/write.2.html)\n"
"              [pwritev(2)](https://man7.org/linux/man-pages/man2/pwritev.2.html)");

#define PROMISEDIO_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))promisedio_write, METH_FASTCALL|METH_KEYWORDS, promisedio_write__doc__},

static inline PyObject *
promisedio_write_impl(PyObject *module, int fd, PyObject *data,
                      Py_off_t offset);

static PyObject *
promisedio_write(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    int fd;
    PyObject *data;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    data = args[1];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_write_impl(module, fd, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_unlink__doc__,
"unlink($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the file path.\n"
"\n"
"Equivalent to [unlink(2)](https://man7.org/linux/man-pages/man2/unlink.2.html).");

#define PROMISEDIO_UNLINK_METHODDEF    \
    {"unlink", (PyCFunction)(void(*)(void))promisedio_unlink, METH_FASTCALL|METH_KEYWORDS, promisedio_unlink__doc__},

static inline PyObject *
promisedio_unlink_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_unlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "unlink", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_unlink_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_mkdir__doc__,
"mkdir($module, /, path, mode=511)\n"
"--\n"
"\n"
"Create a directory named path with numeric mode mode.\n"
"If the directory already exists, FileExistsError is raised.\n"
"\n"
"On some systems, mode is ignored.\n"
"\n"
"Equivalent to [mkdir(2)](https://man7.org/linux/man-pages/man2/mkdir.2.html).");

#define PROMISEDIO_MKDIR_METHODDEF    \
    {"mkdir", (PyCFunction)(void(*)(void))promisedio_mkdir, METH_FASTCALL|METH_KEYWORDS, promisedio_mkdir__doc__},

static inline PyObject *
promisedio_mkdir_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
promisedio_mkdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkdir", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int mode = 511;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_mkdir_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_rmdir__doc__,
"rmdir($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the directory path. If the directory does not exist or is not empty,\n"
"an FileNotFoundError or an OSError is raised respectively.\n"
"\n"
"Equivalent to [rmdir(2)](https://man7.org/linux/man-pages/man2/rmdir.2.html).");

#define PROMISEDIO_RMDIR_METHODDEF    \
    {"rmdir", (PyCFunction)(void(*)(void))promisedio_rmdir, METH_FASTCALL|METH_KEYWORDS, promisedio_rmdir__doc__},

static inline PyObject *
promisedio_rmdir_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_rmdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "rmdir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_rmdir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_mkdtemp__doc__,
"mkdtemp($module, /, tpl)\n"
"--\n"
"\n"
"Generate a uniquely named temporary directory from template tpl.\n"
"The last six characters of template must be XXXXXX and these are replaced with a string that makes\n"
"the directory name unique.\n"
"\n"
"Return the modified template string.\n"
"\n"
"Equivalent to [mkdtemp(3)](https://man7.org/linux/man-pages/man3/mkdtemp.3.html).");

#define PROMISEDIO_MKDTEMP_METHODDEF    \
    {"mkdtemp", (PyCFunction)(void(*)(void))promisedio_mkdtemp, METH_FASTCALL|METH_KEYWORDS, promisedio_mkdtemp__doc__},

static inline PyObject *
promisedio_mkdtemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
promisedio_mkdtemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkdtemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &tpl)) {
        goto exit;
    }
    return_value = promisedio_mkdtemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(promisedio_mkstemp__doc__,
"mkstemp($module, /, tpl)\n"
"--\n"
"\n"
"Generate a unique temporary filename from template.\n"
"This function creates and opens the file, and returns an open file descriptor for the file.\n"
"The last six characters of template must be \"XXXXXX\" and these are replaced with a string that makes \n"
"the filename unique.\n"
"\n"
"Return the file descriptor and modified template string.\n"
"\n"
"Equivalent to [mkstemp(3)](https://man7.org/linux/man-pages/man3/mkstemp.3.html).");

#define PROMISEDIO_MKSTEMP_METHODDEF    \
    {"mkstemp", (PyCFunction)(void(*)(void))promisedio_mkstemp, METH_FASTCALL|METH_KEYWORDS, promisedio_mkstemp__doc__},

static inline PyObject *
promisedio_mkstemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
promisedio_mkstemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkstemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &tpl)) {
        goto exit;
    }
    return_value = promisedio_mkstemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(promisedio_scandir__doc__,
"scandir($module, /, path)\n"
"--\n"
"\n"
"Return a sequence of the entries in the directory given by path (entry_type, entry_name).  \n"
"Special entries \'.\' and \'..\' are not included.\n"
"\n"
"Equivalent to [scandir(3)](https://man7.org/linux/man-pages/man3/scandir.3.html).");

#define PROMISEDIO_SCANDIR_METHODDEF    \
    {"scandir", (PyCFunction)(void(*)(void))promisedio_scandir, METH_FASTCALL|METH_KEYWORDS, promisedio_scandir__doc__},

static inline PyObject *
promisedio_scandir_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_scandir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "scandir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_scandir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_rename__doc__,
"rename($module, /, path, new_path)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_RENAME_METHODDEF    \
    {"rename", (PyCFunction)(void(*)(void))promisedio_rename, METH_FASTCALL|METH_KEYWORDS, promisedio_rename__doc__},

static inline PyObject *
promisedio_rename_impl(PyObject *module, PyObject *path, PyObject *new_path);

static PyObject *
promisedio_rename(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "rename", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    PyObject *new_path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    return_value = promisedio_rename_impl(module, path, new_path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(promisedio_fsync__doc__,
"fsync($module, /, fd)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_FSYNC_METHODDEF    \
    {"fsync", (PyCFunction)(void(*)(void))promisedio_fsync, METH_FASTCALL|METH_KEYWORDS, promisedio_fsync__doc__},

static inline PyObject *
promisedio_fsync_impl(PyObject *module, int fd);

static PyObject *
promisedio_fsync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fsync", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = promisedio_fsync_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_ftruncate__doc__,
"ftruncate($module, /, fd, length)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_FTRUNCATE_METHODDEF    \
    {"ftruncate", (PyCFunction)(void(*)(void))promisedio_ftruncate, METH_FASTCALL|METH_KEYWORDS, promisedio_ftruncate__doc__},

static inline PyObject *
promisedio_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length);

static PyObject *
promisedio_ftruncate(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "length", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "ftruncate", 0};
    PyObject *argsbuf[2];
    int fd;
    Py_ssize_t length;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!ssize_t_converter(args[1], &length)) {
        goto exit;
    }
    return_value = promisedio_ftruncate_impl(module, fd, length);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_fdatasync__doc__,
"fdatasync($module, /, fd)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_FDATASYNC_METHODDEF    \
    {"fdatasync", (PyCFunction)(void(*)(void))promisedio_fdatasync, METH_FASTCALL|METH_KEYWORDS, promisedio_fdatasync__doc__},

static inline PyObject *
promisedio_fdatasync_impl(PyObject *module, int fd);

static PyObject *
promisedio_fdatasync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fdatasync", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = promisedio_fdatasync_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_copyfile__doc__,
"copyfile($module, /, path, new_path, flags=0)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_COPYFILE_METHODDEF    \
    {"copyfile", (PyCFunction)(void(*)(void))promisedio_copyfile, METH_FASTCALL|METH_KEYWORDS, promisedio_copyfile__doc__},

static inline PyObject *
promisedio_copyfile_impl(PyObject *module, PyObject *path,
                         PyObject *new_path, int flags);

static PyObject *
promisedio_copyfile(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "copyfile", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    PyObject *new_path = NULL;
    int flags = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    flags = _PyLong_AsInt(args[2]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_copyfile_impl(module, path, new_path, flags);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(promisedio_sendfile__doc__,
"sendfile($module, /, out_fd, in_fd, offset, count)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_SENDFILE_METHODDEF    \
    {"sendfile", (PyCFunction)(void(*)(void))promisedio_sendfile, METH_FASTCALL|METH_KEYWORDS, promisedio_sendfile__doc__},

static inline PyObject *
promisedio_sendfile_impl(PyObject *module, int out_fd, int in_fd,
                         Py_off_t offset, Py_ssize_t count);

static PyObject *
promisedio_sendfile(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"out_fd", "in_fd", "offset", "count", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "sendfile", 0};
    PyObject *argsbuf[4];
    int out_fd;
    int in_fd;
    Py_off_t offset;
    Py_ssize_t count;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 4, 4, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &out_fd)) {
        goto exit;
    }
    if (!fd_converter(args[1], &in_fd)) {
        goto exit;
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
    if (!ssize_t_converter(args[3], &count)) {
        goto exit;
    }
    return_value = promisedio_sendfile_impl(module, out_fd, in_fd, offset, count);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_access__doc__,
"access($module, /, path, mode)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_ACCESS_METHODDEF    \
    {"access", (PyCFunction)(void(*)(void))promisedio_access, METH_FASTCALL|METH_KEYWORDS, promisedio_access__doc__},

static inline PyObject *
promisedio_access_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
promisedio_access(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "access", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_access_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_chmod__doc__,
"chmod($module, /, path, mode)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_CHMOD_METHODDEF    \
    {"chmod", (PyCFunction)(void(*)(void))promisedio_chmod, METH_FASTCALL|METH_KEYWORDS, promisedio_chmod__doc__},

static inline PyObject *
promisedio_chmod_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
promisedio_chmod(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "chmod", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_chmod_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_fchmod__doc__,
"fchmod($module, /, fd, mode)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_FCHMOD_METHODDEF    \
    {"fchmod", (PyCFunction)(void(*)(void))promisedio_fchmod, METH_FASTCALL|METH_KEYWORDS, promisedio_fchmod__doc__},

static inline PyObject *
promisedio_fchmod_impl(PyObject *module, int fd, int mode);

static PyObject *
promisedio_fchmod(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fchmod", 0};
    PyObject *argsbuf[2];
    int fd;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_fchmod_impl(module, fd, mode);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_utime__doc__,
"utime($module, /, path, atime, mtime, *, follow_symlinks=True)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_UTIME_METHODDEF    \
    {"utime", (PyCFunction)(void(*)(void))promisedio_utime, METH_FASTCALL|METH_KEYWORDS, promisedio_utime__doc__},

static inline PyObject *
promisedio_utime_impl(PyObject *module, PyObject *path, double atime,
                      double mtime, int follow_symlinks);

static PyObject *
promisedio_utime(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "atime", "mtime", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "utime", 0};
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 3;
    PyObject *path = NULL;
    double atime;
    double mtime;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (PyFloat_CheckExact(args[1])) {
        atime = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        atime = PyFloat_AsDouble(args[1]);
        if (atime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (PyFloat_CheckExact(args[2])) {
        mtime = PyFloat_AS_DOUBLE(args[2]);
    }
    else
    {
        mtime = PyFloat_AsDouble(args[2]);
        if (mtime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    follow_symlinks = PyObject_IsTrue(args[3]);
    if (follow_symlinks < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = promisedio_utime_impl(module, path, atime, mtime, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_futime__doc__,
"futime($module, /, fd, atime, mtime)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_FUTIME_METHODDEF    \
    {"futime", (PyCFunction)(void(*)(void))promisedio_futime, METH_FASTCALL|METH_KEYWORDS, promisedio_futime__doc__},

static inline PyObject *
promisedio_futime_impl(PyObject *module, int fd, double atime, double mtime);

static PyObject *
promisedio_futime(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "atime", "mtime", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "futime", 0};
    PyObject *argsbuf[3];
    int fd;
    double atime;
    double mtime;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (PyFloat_CheckExact(args[1])) {
        atime = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        atime = PyFloat_AsDouble(args[1]);
        if (atime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (PyFloat_CheckExact(args[2])) {
        mtime = PyFloat_AS_DOUBLE(args[2]);
    }
    else
    {
        mtime = PyFloat_AsDouble(args[2]);
        if (mtime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    return_value = promisedio_futime_impl(module, fd, atime, mtime);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_link__doc__,
"link($module, /, path, new_path)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_LINK_METHODDEF    \
    {"link", (PyCFunction)(void(*)(void))promisedio_link, METH_FASTCALL|METH_KEYWORDS, promisedio_link__doc__},

static inline PyObject *
promisedio_link_impl(PyObject *module, PyObject *path, PyObject *new_path);

static PyObject *
promisedio_link(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "link", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    PyObject *new_path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    return_value = promisedio_link_impl(module, path, new_path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(promisedio_symlink__doc__,
"symlink($module, /, path, new_path, *, flags=0)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_SYMLINK_METHODDEF    \
    {"symlink", (PyCFunction)(void(*)(void))promisedio_symlink, METH_FASTCALL|METH_KEYWORDS, promisedio_symlink__doc__},

static inline PyObject *
promisedio_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                        int flags);

static PyObject *
promisedio_symlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "symlink", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    PyObject *new_path = NULL;
    int flags = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    flags = _PyLong_AsInt(args[2]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = promisedio_symlink_impl(module, path, new_path, flags);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(promisedio_readlink__doc__,
"readlink($module, /, path)\n"
"--\n"
"\n"
"");

#define PROMISEDIO_READLINK_METHODDEF    \
    {"readlink", (PyCFunction)(void(*)(void))promisedio_readlink, METH_FASTCALL|METH_KEYWORDS, promisedio_readlink__doc__},

static inline PyObject *
promisedio_readlink_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_readlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "readlink", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_readlink_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_set_timeout__doc__,
"set_timeout($module, /, func, timeout, *, unref=False)\n"
"--\n"
"\n"
"Sets a timer which executes a function once the timer expires.\n"
"\n"
"Returns Timer object. This value can be passed to clear_timeout() to cancel the timeout.");

#define PROMISEDIO_SET_TIMEOUT_METHODDEF    \
    {"set_timeout", (PyCFunction)(void(*)(void))promisedio_set_timeout, METH_FASTCALL|METH_KEYWORDS, promisedio_set_timeout__doc__},

static inline PyObject *
promisedio_set_timeout_impl(PyObject *module, PyObject *func, double timeout,
                            int unref);

static PyObject *
promisedio_set_timeout(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"func", "timeout", "unref", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "set_timeout", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *func;
    double timeout;
    int unref = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    func = args[0];
    if (PyFloat_CheckExact(args[1])) {
        timeout = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        timeout = PyFloat_AsDouble(args[1]);
        if (timeout == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    unref = PyObject_IsTrue(args[2]);
    if (unref < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = promisedio_set_timeout_impl(module, func, timeout, unref);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_set_interval__doc__,
"set_interval($module, /, func, interval, *, unref=False)\n"
"--\n"
"\n"
"Sets a timer which executes repeatedly a function, with a fixed time delay between each call.\n"
"\n"
"Returns Timer object. This value can be passed to clear_interval() to cancel the interval.");

#define PROMISEDIO_SET_INTERVAL_METHODDEF    \
    {"set_interval", (PyCFunction)(void(*)(void))promisedio_set_interval, METH_FASTCALL|METH_KEYWORDS, promisedio_set_interval__doc__},

static inline PyObject *
promisedio_set_interval_impl(PyObject *module, PyObject *func,
                             double interval, int unref);

static PyObject *
promisedio_set_interval(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"func", "interval", "unref", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "set_interval", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *func;
    double interval;
    int unref = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    func = args[0];
    if (PyFloat_CheckExact(args[1])) {
        interval = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        interval = PyFloat_AsDouble(args[1]);
        if (interval == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    unref = PyObject_IsTrue(args[2]);
    if (unref < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = promisedio_set_interval_impl(module, func, interval, unref);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_clear_timeout__doc__,
"clear_timeout($module, /, timer)\n"
"--\n"
"\n"
"Cancels a timeout previously established by calling set_timeout().");

#define PROMISEDIO_CLEAR_TIMEOUT_METHODDEF    \
    {"clear_timeout", (PyCFunction)(void(*)(void))promisedio_clear_timeout, METH_FASTCALL|METH_KEYWORDS, promisedio_clear_timeout__doc__},

static inline PyObject *
promisedio_clear_timeout_impl(PyObject *module, PyObject *timer);

static PyObject *
promisedio_clear_timeout(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"timer", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "clear_timeout", 0};
    PyObject *argsbuf[1];
    PyObject *timer;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    timer = args[0];
    return_value = promisedio_clear_timeout_impl(module, timer);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_clear_interval__doc__,
"clear_interval($module, /, timer)\n"
"--\n"
"\n"
"Cancels an interval previously established by calling set_interval().");

#define PROMISEDIO_CLEAR_INTERVAL_METHODDEF    \
    {"clear_interval", (PyCFunction)(void(*)(void))promisedio_clear_interval, METH_FASTCALL|METH_KEYWORDS, promisedio_clear_interval__doc__},

static inline PyObject *
promisedio_clear_interval_impl(PyObject *module, PyObject *timer);

static PyObject *
promisedio_clear_interval(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"timer", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "clear_interval", 0};
    PyObject *argsbuf[1];
    PyObject *timer;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    timer = args[0];
    return_value = promisedio_clear_interval_impl(module, timer);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_time__doc__,
"time($module, /)\n"
"--\n"
"\n"
"Returns the current timestamp in milliseconds. \n"
"The timestamp is cached at the start of the event loop tick.");

#define PROMISEDIO_TIME_METHODDEF    \
    {"time", (PyCFunction)promisedio_time, METH_NOARGS, promisedio_time__doc__},

static inline PyObject *
promisedio_time_impl(PyObject *module);

static PyObject *
promisedio_time(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_time_impl(module);
}

PyDoc_STRVAR(promisedio_hrtime__doc__,
"hrtime($module, /)\n"
"--\n"
"\n"
"Returns the current high-resolution real time. This is expressed in nanoseconds.");

#define PROMISEDIO_HRTIME_METHODDEF    \
    {"hrtime", (PyCFunction)promisedio_hrtime, METH_NOARGS, promisedio_hrtime__doc__},

static inline PyObject *
promisedio_hrtime_impl(PyObject *module);

static PyObject *
promisedio_hrtime(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_hrtime_impl(module);
}

PyDoc_STRVAR(promisedio_getaddrinfo__doc__,
"getaddrinfo($module, /, node, service, family=0, type=0, proto=0,\n"
"            flags=0)\n"
"--\n"
"\n"
"Translate the host/port argument into a sequence of 5-tuples that contain all the necessary arguments for \n"
"creating a socket connected to that service. host is a domain name, a string representation of an IPv4/v6 address or None.\n"
"port is a string service name such as \'http\', a numeric port number or None. \n"
"\n"
"For more information, see [getaddrinfo](https://docs.python.org/3/library/socket.html#socket.getaddrinfo)");

#define PROMISEDIO_GETADDRINFO_METHODDEF    \
    {"getaddrinfo", (PyCFunction)(void(*)(void))promisedio_getaddrinfo, METH_FASTCALL|METH_KEYWORDS, promisedio_getaddrinfo__doc__},

static inline PyObject *
promisedio_getaddrinfo_impl(PyObject *module, const char *node,
                            PyObject *service, int family, int type,
                            int proto, int flags);

static PyObject *
promisedio_getaddrinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
        if (PyFloat_Check(args[2])) {
            PyErr_SetString(PyExc_TypeError,
                            "integer argument expected, got float" );
            goto exit;
        }
        family = _PyLong_AsInt(args[2]);
        if (family == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[3]) {
        if (PyFloat_Check(args[3])) {
            PyErr_SetString(PyExc_TypeError,
                            "integer argument expected, got float" );
            goto exit;
        }
        type = _PyLong_AsInt(args[3]);
        if (type == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[4]) {
        if (PyFloat_Check(args[4])) {
            PyErr_SetString(PyExc_TypeError,
                            "integer argument expected, got float" );
            goto exit;
        }
        proto = _PyLong_AsInt(args[4]);
        if (proto == -1 && PyErr_Occurred()) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (PyFloat_Check(args[5])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    flags = _PyLong_AsInt(args[5]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_getaddrinfo_impl(module, node, service, family, type, proto, flags);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_getnameinfo__doc__,
"getnameinfo($module, /, sockaddr, flags)\n"
"--\n"
"\n"
"Translate a socket address sockaddr into a 2-tuple (host, port). Depending on the settings of flags,\n"
"the result can contain a fully-qualified domain name or numeric address representation in host.\n"
"Similarly, port can contain a string port name or a numeric port number.\n"
"\n"
"For more information about flags you can consult [getnameinfo(3)](https://man7.org/linux/man-pages/man3/getnameinfo.3.html)");

#define PROMISEDIO_GETNAMEINFO_METHODDEF    \
    {"getnameinfo", (PyCFunction)(void(*)(void))promisedio_getnameinfo, METH_FASTCALL|METH_KEYWORDS, promisedio_getnameinfo__doc__},

static inline PyObject *
promisedio_getnameinfo_impl(PyObject *module, sockaddr_any *sockaddr,
                            int flags);

static PyObject *
promisedio_getnameinfo(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    flags = _PyLong_AsInt(args[1]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_getnameinfo_impl(module, &sockaddr, flags);

exit:
    return return_value;
}
/*[clinic end generated code: output=831eb35389a41ecf input=a9049054013a1b77]*/
