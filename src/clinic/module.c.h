/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(promisedio__getallocatedobjectscount__doc__,
"_getallocatedobjectscount($module, /)\n"
"--\n"
"\n"
"Get number of allocated objects,");

#define PROMISEDIO__GETALLOCATEDOBJECTSCOUNT_METHODDEF    \
    {"_getallocatedobjectscount", (PyCFunction)promisedio__getallocatedobjectscount, METH_NOARGS, promisedio__getallocatedobjectscount__doc__},

static PyObject *
promisedio__getallocatedobjectscount_impl(PyObject *module);

static PyObject *
promisedio__getallocatedobjectscount(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__getallocatedobjectscount_impl(module);
}

PyDoc_STRVAR(promisedio__memdebug__doc__,
"_memdebug($module, /)\n"
"--\n"
"\n"
"Memory debug info");

#define PROMISEDIO__MEMDEBUG_METHODDEF    \
    {"_memdebug", (PyCFunction)promisedio__memdebug, METH_NOARGS, promisedio__memdebug__doc__},

static PyObject *
promisedio__memdebug_impl(PyObject *module);

static PyObject *
promisedio__memdebug(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return promisedio__memdebug_impl(module);
}

PyDoc_STRVAR(promisedio__clearfreelists__doc__,
"_clearfreelists($module, /)\n"
"--\n"
"\n"
"Clear freelists");

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
"Schedule coroutine execution.");

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
"Process promise async chain once.");

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
"Create a new Deferred object.");

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
"Delay execution for a given number of seconds.");

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
"Use python default SIGINT handler.");

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
"Run event loop.");

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
"Close event loop.");

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
"\n");

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
"Warning This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.");

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
"astat($module, /, name, *, follow_symlinks=True)\n"
"--\n"
"\n"
"Async equivalent of os.stat().");

#define PROMISEDIO_ASTAT_METHODDEF    \
    {"astat", (PyCFunction)(void(*)(void))promisedio_astat, METH_FASTCALL|METH_KEYWORDS, promisedio_astat__doc__},

static PyObject *
promisedio_astat_impl(PyObject *module, PyObject *name, int follow_symlinks);

static PyObject *
promisedio_astat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "astat", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *name = NULL;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &name)) {
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
    return_value = promisedio_astat_impl(module, name, follow_symlinks);

exit:
    /* Cleanup for name */
    Py_XDECREF(name);

    return return_value;
}

PyDoc_STRVAR(promisedio_afstat__doc__,
"afstat($module, /, fd)\n"
"--\n"
"\n"
"Async equivalent of os.fstat().");

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
"Async equivalent of os.lseek().");

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

PyDoc_STRVAR(promisedio_aopenfd__doc__,
"aopenfd($module, /, name, flags=\'r\', mode=438)\n"
"--\n"
"\n"
"Async equivalent of os.open().");

#define PROMISEDIO_AOPENFD_METHODDEF    \
    {"aopenfd", (PyCFunction)(void(*)(void))promisedio_aopenfd, METH_FASTCALL|METH_KEYWORDS, promisedio_aopenfd__doc__},

static PyObject *
promisedio_aopenfd_impl(PyObject *module, PyObject *name, const char *flags,
                        int mode);

static PyObject *
promisedio_aopenfd(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", "flags", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aopenfd", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *name = NULL;
    const char *flags = "r";
    int mode = 438;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &name)) {
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
    return_value = promisedio_aopenfd_impl(module, name, flags, mode);

exit:
    /* Cleanup for name */
    Py_XDECREF(name);

    return return_value;
}

PyDoc_STRVAR(promisedio_aclose__doc__,
"aclose($module, /, fd)\n"
"--\n"
"\n"
"Async equivalent of os.close().");

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
"Async equivalent of os.read().");

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
"Async equivalent of os.write().");

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
"aunlink($module, /, name)\n"
"--\n"
"\n"
"Async equivalent of unlink().");

#define PROMISEDIO_AUNLINK_METHODDEF    \
    {"aunlink", (PyCFunction)(void(*)(void))promisedio_aunlink, METH_FASTCALL|METH_KEYWORDS, promisedio_aunlink__doc__},

static PyObject *
promisedio_aunlink_impl(PyObject *module, PyObject *name);

static PyObject *
promisedio_aunlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aunlink", 0};
    PyObject *argsbuf[1];
    PyObject *name = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &name)) {
        goto exit;
    }
    return_value = promisedio_aunlink_impl(module, name);

exit:
    /* Cleanup for name */
    Py_XDECREF(name);

    return return_value;
}

PyDoc_STRVAR(promisedio_amkdir__doc__,
"amkdir($module, /, name, mode=511)\n"
"--\n"
"\n"
"Async equivalent of mkdir().");

#define PROMISEDIO_AMKDIR_METHODDEF    \
    {"amkdir", (PyCFunction)(void(*)(void))promisedio_amkdir, METH_FASTCALL|METH_KEYWORDS, promisedio_amkdir__doc__},

static PyObject *
promisedio_amkdir_impl(PyObject *module, PyObject *name, int mode);

static PyObject *
promisedio_amkdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "amkdir", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *name = NULL;
    int mode = 511;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &name)) {
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
    return_value = promisedio_amkdir_impl(module, name, mode);

exit:
    /* Cleanup for name */
    Py_XDECREF(name);

    return return_value;
}

PyDoc_STRVAR(promisedio_armdir__doc__,
"armdir($module, /, name)\n"
"--\n"
"\n"
"Async equivalent of rmdir().");

#define PROMISEDIO_ARMDIR_METHODDEF    \
    {"armdir", (PyCFunction)(void(*)(void))promisedio_armdir, METH_FASTCALL|METH_KEYWORDS, promisedio_armdir__doc__},

static PyObject *
promisedio_armdir_impl(PyObject *module, PyObject *name);

static PyObject *
promisedio_armdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "armdir", 0};
    PyObject *argsbuf[1];
    PyObject *name = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!path_converter(args[0], &name)) {
        goto exit;
    }
    return_value = promisedio_armdir_impl(module, name);

exit:
    /* Cleanup for name */
    Py_XDECREF(name);

    return return_value;
}

PyDoc_STRVAR(promisedio_amkdtemp__doc__,
"amkdtemp($module, /, tpl)\n"
"--\n"
"\n"
"Async equivalent of mkdtemp.");

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

PyDoc_STRVAR(promisedio_aopen__doc__,
"aopen($module, /, name, flags=\'r\', mode=438)\n"
"--\n"
"\n"
"Async equivalent of open().");

#define PROMISEDIO_AOPEN_METHODDEF    \
    {"aopen", (PyCFunction)(void(*)(void))promisedio_aopen, METH_FASTCALL|METH_KEYWORDS, promisedio_aopen__doc__},

static PyObject *
promisedio_aopen_impl(PyObject *module, PyObject *name, const char *flags,
                      int mode);

static PyObject *
promisedio_aopen(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"name", "flags", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "aopen", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *name;
    const char *flags = "r";
    int mode = 438;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    name = args[0];
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
    mode = _PyLong_AsInt(args[2]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = promisedio_aopen_impl(module, name, flags, mode);

exit:
    return return_value;
}
/*[clinic end generated code: output=c68ce860f8dd23c8 input=a9049054013a1b77]*/
