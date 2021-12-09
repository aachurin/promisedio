/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(loop_run_until_complete__doc__,
"run_until_complete($module, /)\n"
"--\n"
"\n"
"");

#define LOOP_RUN_UNTIL_COMPLETE_METHODDEF    \
    {"run_until_complete", (PyCFunction)loop_run_until_complete, METH_NOARGS, loop_run_until_complete__doc__},

Py_LOCAL_INLINE(PyObject *)
loop_run_until_complete_impl(PyObject *module);

static PyObject *
loop_run_until_complete(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_run_until_complete_impl(module);
}

PyDoc_STRVAR(loop_time__doc__,
"time($module, /)\n"
"--\n"
"\n"
"Get current timestamp in milliseconds.\n"
"\n"
"The timestamp is cached at the start of the event loop tick.");

#define LOOP_TIME_METHODDEF    \
    {"time", (PyCFunction)loop_time, METH_NOARGS, loop_time__doc__},

Py_LOCAL_INLINE(PyObject *)
loop_time_impl(PyObject *module);

static PyObject *
loop_time(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_time_impl(module);
}

PyDoc_STRVAR(loop_hrtime__doc__,
"hrtime($module, /)\n"
"--\n"
"\n"
"Return current high-resolution real time. \n"
"\n"
"This is expressed in nanoseconds.");

#define LOOP_HRTIME_METHODDEF    \
    {"hrtime", (PyCFunction)loop_hrtime, METH_NOARGS, loop_hrtime__doc__},

Py_LOCAL_INLINE(PyObject *)
loop_hrtime_impl(PyObject *module);

static PyObject *
loop_hrtime(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return loop_hrtime_impl(module);
}
/*[clinic end generated code: output=38e90123cd47295c input=a9049054013a1b77]*/
