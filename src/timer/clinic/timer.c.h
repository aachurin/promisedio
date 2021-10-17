/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(timer_sleep__doc__,
"sleep($module, /, seconds)\n"
"--\n"
"\n"
"Delay execution for a given number of seconds.");

#define TIMER_SLEEP_METHODDEF    \
    {"sleep", (PyCFunction)(void(*)(void))timer_sleep, METH_FASTCALL|METH_KEYWORDS, timer_sleep__doc__},

Py_LOCAL_INLINE(PyObject *)
timer_sleep_impl(PyObject *module, double seconds);

static PyObject *
timer_sleep(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = timer_sleep_impl(module, seconds);

exit:
    return return_value;
}

PyDoc_STRVAR(timer_set_timeout__doc__,
"set_timeout($module, /, func, timeout, *, unref=False)\n"
"--\n"
"\n"
"Set timer which executes a function once the timer expires.\n"
"\n"
"Return Timer object. This value can be passed to clear_timeout() to cancel the timeout.");

#define TIMER_SET_TIMEOUT_METHODDEF    \
    {"set_timeout", (PyCFunction)(void(*)(void))timer_set_timeout, METH_FASTCALL|METH_KEYWORDS, timer_set_timeout__doc__},

Py_LOCAL_INLINE(PyObject *)
timer_set_timeout_impl(PyObject *module, PyObject *func, double timeout,
                       int unref);

static PyObject *
timer_set_timeout(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = timer_set_timeout_impl(module, func, timeout, unref);

exit:
    return return_value;
}

PyDoc_STRVAR(timer_set_interval__doc__,
"set_interval($module, /, func, interval, *, unref=False)\n"
"--\n"
"\n"
"Set timer which executes repeatedly a function, with a fixed time delay between each call.\n"
"\n"
"Return Timer object. This value can be passed to clear_interval() to cancel the interval.");

#define TIMER_SET_INTERVAL_METHODDEF    \
    {"set_interval", (PyCFunction)(void(*)(void))timer_set_interval, METH_FASTCALL|METH_KEYWORDS, timer_set_interval__doc__},

Py_LOCAL_INLINE(PyObject *)
timer_set_interval_impl(PyObject *module, PyObject *func, double interval,
                        int unref);

static PyObject *
timer_set_interval(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = timer_set_interval_impl(module, func, interval, unref);

exit:
    return return_value;
}

PyDoc_STRVAR(timer_clear_timeout__doc__,
"clear_timeout($module, /, timer)\n"
"--\n"
"\n"
"Cancel timeout previously established by calling set_timeout().");

#define TIMER_CLEAR_TIMEOUT_METHODDEF    \
    {"clear_timeout", (PyCFunction)(void(*)(void))timer_clear_timeout, METH_FASTCALL|METH_KEYWORDS, timer_clear_timeout__doc__},

Py_LOCAL_INLINE(PyObject *)
timer_clear_timeout_impl(PyObject *module, PyObject *timer);

static PyObject *
timer_clear_timeout(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = timer_clear_timeout_impl(module, timer);

exit:
    return return_value;
}

PyDoc_STRVAR(timer_clear_interval__doc__,
"clear_interval($module, /, timer)\n"
"--\n"
"\n"
"Cancel interval previously established by calling set_interval().");

#define TIMER_CLEAR_INTERVAL_METHODDEF    \
    {"clear_interval", (PyCFunction)(void(*)(void))timer_clear_interval, METH_FASTCALL|METH_KEYWORDS, timer_clear_interval__doc__},

Py_LOCAL_INLINE(PyObject *)
timer_clear_interval_impl(PyObject *module, PyObject *timer);

static PyObject *
timer_clear_interval(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
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
    return_value = timer_clear_interval_impl(module, timer);

exit:
    return return_value;
}
/*[clinic end generated code: output=e3e3ca37a001bb77 input=a9049054013a1b77]*/
