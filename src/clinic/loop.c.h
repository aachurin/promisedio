/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(loop_use_python_default_sigint__doc__,
"use_python_default_sigint($module, /, flag)\n"
"--\n"
"\n"
"Use python default SIGINT handler.");

#define LOOP_USE_PYTHON_DEFAULT_SIGINT_METHODDEF    \
    {"use_python_default_sigint", (PyCFunction)(void(*)(void))loop_use_python_default_sigint, METH_FASTCALL|METH_KEYWORDS, loop_use_python_default_sigint__doc__},

static PyObject *
loop_use_python_default_sigint_impl(PyObject *module, int flag);

static PyObject *
loop_use_python_default_sigint(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = loop_use_python_default_sigint_impl(module, flag);

exit:
    return return_value;
}

PyDoc_STRVAR(loop_run_loop__doc__,
"run_loop($module, /)\n"
"--\n"
"\n"
"Run event loop.");

#define LOOP_RUN_LOOP_METHODDEF    \
    {"run_loop", (PyCFunction)loop_run_loop, METH_NOARGS, loop_run_loop__doc__},

static PyObject *
loop_run_loop_impl(PyObject *module);

static PyObject *
loop_run_loop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_run_loop_impl(module);
}

PyDoc_STRVAR(loop_close_loop__doc__,
"close_loop($module, /)\n"
"--\n"
"\n"
"Close event loop.");

#define LOOP_CLOSE_LOOP_METHODDEF    \
    {"close_loop", (PyCFunction)loop_close_loop, METH_NOARGS, loop_close_loop__doc__},

static PyObject *
loop_close_loop_impl(PyObject *module);

static PyObject *
loop_close_loop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_close_loop_impl(module);
}

PyDoc_STRVAR(loop_check_can_fork__doc__,
"check_can_fork($module, /)\n"
"--\n"
"\n");

#define LOOP_CHECK_CAN_FORK_METHODDEF    \
    {"check_can_fork", (PyCFunction)loop_check_can_fork, METH_NOARGS, loop_check_can_fork__doc__},

static PyObject *
loop_check_can_fork_impl(PyObject *module);

static PyObject *
loop_check_can_fork(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_check_can_fork_impl(module);
}

PyDoc_STRVAR(loop__inspect_loop__doc__,
"_inspect_loop($module, /)\n"
"--\n"
"\n"
"Prints all handles associated with the given loop.\n"
"\n"
"Warning This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.");

#define LOOP__INSPECT_LOOP_METHODDEF    \
    {"_inspect_loop", (PyCFunction)loop__inspect_loop, METH_NOARGS, loop__inspect_loop__doc__},

static PyObject *
loop__inspect_loop_impl(PyObject *module);

static PyObject *
loop__inspect_loop(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop__inspect_loop_impl(module);
}
/*[clinic end generated code: output=98daf6eac977e00e input=a9049054013a1b77]*/
