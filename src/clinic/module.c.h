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

static PyObject *
promisedio__getallocatedobjectscount_impl(PyObject *module);

static PyObject *
promisedio__getallocatedobjectscount(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__getallocatedobjectscount_impl(module);
}

PyDoc_STRVAR(promisedio__printmeminfo__doc__,
"_printmeminfo($module, /)\n"
"--\n"
"\n"
"Memory debug info\n"
"\n"
"");

#define PROMISEDIO__PRINTMEMINFO_METHODDEF    \
    {"_printmeminfo", (PyCFunction)promisedio__printmeminfo, METH_NOARGS, promisedio__printmeminfo__doc__},

static PyObject *
promisedio__printmeminfo_impl(PyObject *module);

static PyObject *
promisedio__printmeminfo(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__printmeminfo_impl(module);
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

static PyObject *
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

static PyObject *
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

static int
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
"Create a new Deferred object.\n"
"\n"
"");

#define PROMISEDIO_DEFERRED_METHODDEF    \
    {"deferred", (PyCFunction)promisedio_deferred, METH_NOARGS, promisedio_deferred__doc__},

static PyObject *
promisedio_deferred_impl(PyObject *module);

static PyObject *
promisedio_deferred(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio_deferred_impl(module);
}

PyDoc_STRVAR(promisedio_asleep__doc__,
"asleep($module, /, seconds)\n"
"--\n"
"\n"
"Delay execution for a given number of seconds.\n"
"\n"
"");

#define PROMISEDIO_ASLEEP_METHODDEF    \
    {"asleep", (PyCFunction)(void(*)(void))promisedio_asleep, METH_FASTCALL|METH_KEYWORDS, promisedio_asleep__doc__},

static PyObject *
promisedio_asleep_impl(PyObject *module, double seconds);

static PyObject *
promisedio_asleep(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"seconds", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "asleep", 0};
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
    return_value = promisedio_asleep_impl(module, seconds);

exit:
    return return_value;
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

static PyObject *
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

static PyObject *
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

static PyObject *
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

static PyObject *
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

static PyObject *
promisedio__inspectloop_impl(PyObject *module);

static PyObject *
promisedio__inspectloop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__inspectloop_impl(module);
}

PyDoc_STRVAR(promisedio_astat__doc__,
"astat($module, /, path, *, follow_symlinks=True)\n"
"--\n"
"\n"
"Get the status of a file. Return a StatObj object.\n"
"\n"
"This function normally follows symlinks; to stat a symlink add the argument follow_symlinks=False\n"
"\n"
"Equivalent to [stat(2)](https://man7.org/linux/man-pages/man2/stat.2.html) \n"
"              [lstat(2)](https://man7.org/linux/man-pages/man2/lstat.2.html).");

#define PROMISEDIO_ASTAT_METHODDEF    \
    {"astat", (PyCFunction)(void(*)(void))promisedio_astat, METH_FASTCALL|METH_KEYWORDS, promisedio_astat__doc__},

static PyObject *
promisedio_astat_impl(PyObject *module, PyObject *path, int follow_symlinks);

static PyObject *
promisedio_astat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "astat", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
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
    return_value = promisedio_astat_impl(module, path, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_afstat__doc__,
"afstat($module, /, fd)\n"
"--\n"
"\n"
"Get the status of the file descriptor fd. Return a StatObj object.\n"
"\n"
"Equivalent to [fstat(2)](https://man7.org/linux/man-pages/man2/fstat.2.html).");

#define PROMISEDIO_AFSTAT_METHODDEF    \
    {"afstat", (PyCFunction)(void(*)(void))promisedio_afstat, METH_FASTCALL|METH_KEYWORDS, promisedio_afstat__doc__},

static PyObject *
promisedio_afstat_impl(PyObject *module, int fd);

static PyObject *
promisedio_afstat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "afstat", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_afstat_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aseek__doc__,
"aseek($module, /, fd, pos, how)\n"
"--\n"
"\n"
"Set the current position of file descriptor fd to position pos, modified by how:\n"
"SEEK_SET or 0 to set the position relative to the beginning of the file;\n"
"SEEK_CUR or 1 to set it relative to the current position;\n"
"SEEK_END or 2 to set it relative to the end of the file.\n"
"\n"
"Return the new cursor position in bytes, starting from the beginning.\n"
"\n"
"Equivalent to [lseek(2)](https://man7.org/linux/man-pages/man2/lseek.2.html).");

#define PROMISEDIO_ASEEK_METHODDEF    \
    {"aseek", (PyCFunction)(void(*)(void))promisedio_aseek, METH_FASTCALL|METH_KEYWORDS, promisedio_aseek__doc__},

static PyObject *
promisedio_aseek_impl(PyObject *module, int fd, Py_ssize_t pos, int how);

static PyObject *
promisedio_aseek(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "pos", "how", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aseek", 0};
    PyObject *argsbuf[3];
    int fd;
    Py_ssize_t pos;
    int how;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = PyNumber_Index(args[1]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        pos = ival;
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
    return_value = promisedio_aseek_impl(module, fd, pos, how);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aopen__doc__,
"aopen($module, /, path, flags=\'r\', closefd=True)\n"
"--\n"
"\n"
"Open file and return a corresponding file object. If the file cannot be opened, an OSError is raised. \n"
"\n"
"Equivalent to python [open](https://docs.python.org/3/library/functions.html#open) (binary mode only).");

#define PROMISEDIO_AOPEN_METHODDEF    \
    {"aopen", (PyCFunction)(void(*)(void))promisedio_aopen, METH_FASTCALL|METH_KEYWORDS, promisedio_aopen__doc__},

static PyObject *
promisedio_aopen_impl(PyObject *module, PyObject *path, const char *flags,
                      int closefd);

static PyObject *
promisedio_aopen(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "flags", "closefd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aopen", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path;
    const char *flags = "r";
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
        if (!PyUnicode_Check(args[1])) {
            _PyArg_BadArgument("aopen", "argument 'flags'", "str", args[1]);
            goto exit;
        }
        Py_ssize_t flags_length;
        flags = PyUnicode_AsUTF8AndSize(args[1], &flags_length);
        if (flags == NULL) {
            goto exit;
        }
        if (strlen(flags) != (size_t)flags_length) {
            PyErr_SetString(PyExc_ValueError, "embedded null character");
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
    return_value = promisedio_aopen_impl(module, path, flags, closefd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aopenfd__doc__,
"aopenfd($module, /, path, flags=\'r\', mode=438)\n"
"--\n"
"\n"
"Open the file path and set various flags according to flags and possibly its mode according to mode. \n"
"Return the file descriptor for the newly opened file. The new file descriptor is non-inheritable.\n"
"\n"
"Equivalent to [open(2)](https://man7.org/linux/man-pages/man2/open.2.html).");

#define PROMISEDIO_AOPENFD_METHODDEF    \
    {"aopenfd", (PyCFunction)(void(*)(void))promisedio_aopenfd, METH_FASTCALL|METH_KEYWORDS, promisedio_aopenfd__doc__},

static PyObject *
promisedio_aopenfd_impl(PyObject *module, PyObject *path, const char *flags,
                        int mode);

static PyObject *
promisedio_aopenfd(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "flags", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aopenfd", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    const char *flags = "r";
    int mode = 438;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        if (!PyUnicode_Check(args[1])) {
            _PyArg_BadArgument("aopenfd", "argument 'flags'", "str", args[1]);
            goto exit;
        }
        Py_ssize_t flags_length;
        flags = PyUnicode_AsUTF8AndSize(args[1], &flags_length);
        if (flags == NULL) {
            goto exit;
        }
        if (strlen(flags) != (size_t)flags_length) {
            PyErr_SetString(PyExc_ValueError, "embedded null character");
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
    mode = _PyLong_AsInt(args[2]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_aopenfd_impl(module, path, flags, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_aclose__doc__,
"aclose($module, /, fd)\n"
"--\n"
"\n"
"Close file descriptor fd.\n"
"\n"
"Equivalent to [close(2)](https://man7.org/linux/man-pages/man2/close.2.html).");

#define PROMISEDIO_ACLOSE_METHODDEF    \
    {"aclose", (PyCFunction)(void(*)(void))promisedio_aclose, METH_FASTCALL|METH_KEYWORDS, promisedio_aclose__doc__},

static PyObject *
promisedio_aclose_impl(PyObject *module, int fd);

static PyObject *
promisedio_aclose(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aclose", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_aclose_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aread__doc__,
"aread($module, /, fd, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read from file descriptor fd until we have size characters or we hit EOF.\n"
"If offset is present and is not negative, read at most size bytes from file descriptor fd at a position of offset,\n"
"leaving the file offset unchanged.\n"
"\n"
"Equivalent to [read(2)](https://man7.org/linux/man-pages/man2/read.2.html)\n"
"              [preadv(2)](https://man7.org/linux/man-pages/man2/preadv.2.html)");

#define PROMISEDIO_AREAD_METHODDEF    \
    {"aread", (PyCFunction)(void(*)(void))promisedio_aread, METH_FASTCALL|METH_KEYWORDS, promisedio_aread__doc__},

static PyObject *
promisedio_aread_impl(PyObject *module, int fd, Py_ssize_t size,
                      Py_ssize_t offset);

static PyObject *
promisedio_aread(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aread", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    int fd;
    Py_ssize_t size = -1;
    Py_ssize_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        if (PyFloat_Check(args[1])) {
            PyErr_SetString(PyExc_TypeError,
                            "integer argument expected, got float" );
            goto exit;
        }
        {
            Py_ssize_t ival = -1;
            PyObject *iobj = PyNumber_Index(args[1]);
            if (iobj != NULL) {
                ival = PyLong_AsSsize_t(iobj);
                Py_DECREF(iobj);
            }
            if (ival == -1 && PyErr_Occurred()) {
                goto exit;
            }
            size = ival;
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
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = PyNumber_Index(args[2]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        offset = ival;
    }
skip_optional_pos:
    return_value = promisedio_aread_impl(module, fd, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_awrite__doc__,
"awrite($module, /, fd, data, offset=-1)\n"
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

#define PROMISEDIO_AWRITE_METHODDEF    \
    {"awrite", (PyCFunction)(void(*)(void))promisedio_awrite, METH_FASTCALL|METH_KEYWORDS, promisedio_awrite__doc__},

static PyObject *
promisedio_awrite_impl(PyObject *module, int fd, PyObject *data,
                       Py_ssize_t offset);

static PyObject *
promisedio_awrite(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "awrite", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    int fd;
    PyObject *data;
    Py_ssize_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    data = args[1];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = PyNumber_Index(args[2]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        offset = ival;
    }
skip_optional_pos:
    return_value = promisedio_awrite_impl(module, fd, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aunlink__doc__,
"aunlink($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the file path.\n"
"\n"
"Equivalent to [unlink(2)](https://man7.org/linux/man-pages/man2/unlink.2.html).");

#define PROMISEDIO_AUNLINK_METHODDEF    \
    {"aunlink", (PyCFunction)(void(*)(void))promisedio_aunlink, METH_FASTCALL|METH_KEYWORDS, promisedio_aunlink__doc__},

static PyObject *
promisedio_aunlink_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_aunlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aunlink", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_aunlink_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_amkdir__doc__,
"amkdir($module, /, path, mode=511)\n"
"--\n"
"\n"
"Create a directory named path with numeric mode mode.\n"
"If the directory already exists, FileExistsError is raised.\n"
"\n"
"On some systems, mode is ignored.\n"
"\n"
"Equivalent to [mkdir(2)](https://man7.org/linux/man-pages/man2/mkdir.2.html).");

#define PROMISEDIO_AMKDIR_METHODDEF    \
    {"amkdir", (PyCFunction)(void(*)(void))promisedio_amkdir, METH_FASTCALL|METH_KEYWORDS, promisedio_amkdir__doc__},

static PyObject *
promisedio_amkdir_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
promisedio_amkdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "amkdir", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int mode = 511;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
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
    return_value = promisedio_amkdir_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_armdir__doc__,
"armdir($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the directory path. If the directory does not exist or is not empty,\n"
"an FileNotFoundError or an OSError is raised respectively.\n"
"\n"
"Equivalent to [rmdir(2)](https://man7.org/linux/man-pages/man2/rmdir.2.html).");

#define PROMISEDIO_ARMDIR_METHODDEF    \
    {"armdir", (PyCFunction)(void(*)(void))promisedio_armdir, METH_FASTCALL|METH_KEYWORDS, promisedio_armdir__doc__},

static PyObject *
promisedio_armdir_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_armdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "armdir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_armdir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_amkdtemp__doc__,
"amkdtemp($module, /, tpl)\n"
"--\n"
"\n"
"Generate a uniquely named temporary directory from template tpl.\n"
"The last six characters of template must be XXXXXX and these are replaced with a string that makes\n"
"the directory name unique.\n"
"\n"
"Return the modified template string.\n"
"\n"
"Equivalent to [mkdtemp(3)](https://man7.org/linux/man-pages/man3/mkdtemp.3.html).");

#define PROMISEDIO_AMKDTEMP_METHODDEF    \
    {"amkdtemp", (PyCFunction)(void(*)(void))promisedio_amkdtemp, METH_FASTCALL|METH_KEYWORDS, promisedio_amkdtemp__doc__},

static PyObject *
promisedio_amkdtemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
promisedio_amkdtemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "amkdtemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &tpl)) {
        goto exit;
    }
    return_value = promisedio_amkdtemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(promisedio_amkstemp__doc__,
"amkstemp($module, /, tpl)\n"
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

#define PROMISEDIO_AMKSTEMP_METHODDEF    \
    {"amkstemp", (PyCFunction)(void(*)(void))promisedio_amkstemp, METH_FASTCALL|METH_KEYWORDS, promisedio_amkstemp__doc__},

static PyObject *
promisedio_amkstemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
promisedio_amkstemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "amkstemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &tpl)) {
        goto exit;
    }
    return_value = promisedio_amkstemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(promisedio_ascandir__doc__,
"ascandir($module, /, path)\n"
"--\n"
"\n"
"Return a sequence of the entries in the directory given by path (entry_type, entry_name).  \n"
"Special entries \'.\' and \'..\' are not included.\n"
"\n"
"Equivalent to [scandir(3)](https://man7.org/linux/man-pages/man3/scandir.3.html).");

#define PROMISEDIO_ASCANDIR_METHODDEF    \
    {"ascandir", (PyCFunction)(void(*)(void))promisedio_ascandir, METH_FASTCALL|METH_KEYWORDS, promisedio_ascandir__doc__},

static PyObject *
promisedio_ascandir_impl(PyObject *module, PyObject *path);

static PyObject *
promisedio_ascandir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "ascandir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
        goto exit;
    }
    return_value = promisedio_ascandir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(promisedio_arename__doc__,
"arename($module, /, path, new_path)\n"
"--\n"
"\n"
"Rename the file or directory path to new_path.\n"
"\n"
"Equivalent to [rename(2)](https://man7.org/linux/man-pages/man2/rename.2.html).");

#define PROMISEDIO_ARENAME_METHODDEF    \
    {"arename", (PyCFunction)(void(*)(void))promisedio_arename, METH_FASTCALL|METH_KEYWORDS, promisedio_arename__doc__},

static PyObject *
promisedio_arename_impl(PyObject *module, PyObject *path, PyObject *new_path);

static PyObject *
promisedio_arename(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "arename", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    PyObject *new_path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &path)) {
        goto exit;
    }
    if (!path_converter(args[1], &new_path)) {
        goto exit;
    }
    return_value = promisedio_arename_impl(module, path, new_path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(promisedio_afsync__doc__,
"afsync($module, /, fd)\n"
"--\n"
"\n"
"Force write of file with file descriptor fd to disk.\n"
"\n"
"Equivalent to [fsync(2)](https://man7.org/linux/man-pages/man2/fsync.2.html).");

#define PROMISEDIO_AFSYNC_METHODDEF    \
    {"afsync", (PyCFunction)(void(*)(void))promisedio_afsync, METH_FASTCALL|METH_KEYWORDS, promisedio_afsync__doc__},

static PyObject *
promisedio_afsync_impl(PyObject *module, int fd);

static PyObject *
promisedio_afsync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "afsync", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = promisedio_afsync_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(promisedio_aftruncate__doc__,
"aftruncate($module, /, fd, length)\n"
"--\n"
"\n"
"Truncate the file corresponding to file descriptor fd, so that it is at most length bytes in size.\n"
"\n"
"Equivalent to [ftruncate(2)](https://man7.org/linux/man-pages/man2/ftruncate.2.html).");

#define PROMISEDIO_AFTRUNCATE_METHODDEF    \
    {"aftruncate", (PyCFunction)(void(*)(void))promisedio_aftruncate, METH_FASTCALL|METH_KEYWORDS, promisedio_aftruncate__doc__},

static PyObject *
promisedio_aftruncate_impl(PyObject *module, int fd, Py_ssize_t length);

static PyObject *
promisedio_aftruncate(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "length", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aftruncate", 0};
    PyObject *argsbuf[2];
    int fd;
    Py_ssize_t length;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (PyFloat_Check(args[0])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    fd = _PyLong_AsInt(args[0]);
    if (fd == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (PyFloat_Check(args[1])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    {
        Py_ssize_t ival = -1;
        PyObject *iobj = PyNumber_Index(args[1]);
        if (iobj != NULL) {
            ival = PyLong_AsSsize_t(iobj);
            Py_DECREF(iobj);
        }
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        length = ival;
    }
    return_value = promisedio_aftruncate_impl(module, fd, length);

exit:
    return return_value;
}
/*[clinic end generated code: output=292078479e78ad4f input=a9049054013a1b77]*/
