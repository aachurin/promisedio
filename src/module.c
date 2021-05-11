// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "common.h"
#include "general_types.h"
#include "promise.h"
#include "timer.h"
#include "fs.h"
#include "loop.h"
#include "fileio.h"

/*[python input]
class path_converter(CConverter):
    type = "PyObject *"
    converter = "path_converter"
    c_default = "NULL"
    def cleanup(self):
        return f"Py_XDECREF({self.name});"
[python start generated code]*/
/*[python end generated code: output=da39a3ee5e6b4b0d input=8b145077d6a99926]*/

static int
path_converter(PyObject *obj, PyObject **path)
{
    *path = Fs_Path(obj);
    if (*path == NULL)
        return 0;
    return 1;
}

#include "clinic/module.c.h"

static PyMethodDef module_methods[] = {         \
    PROMISEDIO__GETALLOCATEDOBJECTSCOUNT_METHODDEF      \
    PROMISEDIO__MEMDEBUG_METHODDEF                      \
    PROMISEDIO__CLEARFREELISTS_METHODDEF                \
    PROMISEDIO_PROCESS_PROMISE_CHAIN_METHODDEF          \
    PROMISEDIO_DEFERRED_METHODDEF                       \
    PROMISEDIO_EXEC_ASYNC_METHODDEF                     \
    PROMISEDIO_SLEEP_METHODDEF                          \
    PROMISEDIO_RUN_LOOP_METHODDEF                       \
    PROMISEDIO_CLOSE_LOOP_METHODDEF                     \
    PROMISEDIO_USE_PYTHON_DEFAULT_SIGINT_METHODDEF      \
    PROMISEDIO_CHECK_CAN_FORK_METHODDEF                 \
    PROMISEDIO__INSPECTLOOP_METHODDEF                   \
    PROMISEDIO_ACLOSE_METHODDEF                         \
    PROMISEDIO_AFSTAT_METHODDEF                         \
    PROMISEDIO_AOPENFD_METHODDEF                        \
    PROMISEDIO_AOPEN_METHODDEF                          \
    PROMISEDIO_AREAD_METHODDEF                          \
    PROMISEDIO_AWRITE_METHODDEF                         \
    PROMISEDIO_ASTAT_METHODDEF                          \
    PROMISEDIO_ASEEK_METHODDEF                          \
    {NULL, NULL}
};

/*[clinic input]
module promisedio
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=9c500866a0fbbef8]*/

/*[clinic input]
promisedio._getallocatedobjectscount

Get number of allocated objects,
[clinic start generated code]*/

static PyObject *
promisedio__getallocatedobjectscount_impl(PyObject *module)
/*[clinic end generated code: output=64fa0ceb8c702eeb input=3653fe3582a0c7df]*/
{
    return PyLong_FromSize_t(Mem_AllocCount());
}

/*[clinic input]
promisedio._memdebug

Memory debug info
[clinic start generated code]*/

static PyObject *
promisedio__memdebug_impl(PyObject *module)
/*[clinic end generated code: output=3e73444951015a07 input=5be249ff53c72946]*/
{
    Mem_DebugInfo();
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio._clearfreelists

Clear freelists
[clinic start generated code]*/

static PyObject *
promisedio__clearfreelists_impl(PyObject *module)
/*[clinic end generated code: output=c8dea74416142a83 input=5134b1016c9494a9]*/
{
    Mem_ClearFreelists();
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.exec_async
    coro: object(subclass_of='&PyCoro_Type')
    /

Schedule coroutine execution.
[clinic start generated code]*/

static PyObject *
promisedio_exec_async_impl(PyObject *module, PyObject *coro)
/*[clinic end generated code: output=278e1ced79dfa3ac input=b4b82522f580b96c]*/
{
    if (Promise_ExecAsync(coro)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.process_promise_chain -> bool

Process promise async chain once.
[clinic start generated code]*/

static int
promisedio_process_promise_chain_impl(PyObject *module)
/*[clinic end generated code: output=8f2b7be1a7c1c33b input=7b52b2c804a788a8]*/
{
    return PromiseChain_Process();
}

/*[clinic input]
promisedio.deferred

Create a new Deferred object.
[clinic start generated code]*/

static PyObject *
promisedio_deferred_impl(PyObject *module)
/*[clinic end generated code: output=cf587616d6ede614 input=9d3b687d152ceb95]*/
{
    return (PyObject *) Deferred_New();
}

/*[clinic input]
promisedio.sleep
    seconds: double

Delay execution for a given number of seconds.
[clinic start generated code]*/

static PyObject *
promisedio_sleep_impl(PyObject *module, double seconds)
/*[clinic end generated code: output=db7bf8f2c1e0f2da input=eee189eaf37b5581]*/
{
    if (seconds < 0) {
        PyErr_SetString(PyExc_ValueError, "sleep length must be non-negative");
        return NULL;
    }
    return (PyObject *) Timer_Timeout(seconds);
}

/*[clinic input]
promisedio.use_python_default_sigint
    flag: bool

Use python default SIGINT handler.
[clinic start generated code]*/

static PyObject *
promisedio_use_python_default_sigint_impl(PyObject *module, int flag)
/*[clinic end generated code: output=8988d961a83023c5 input=78af53302a06dc97]*/
{
    LoopConfig *config = Loop_GetConfig();
    if (config == NULL) {
        return NULL;
    }
    config->use_python_default_sigint = flag;
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.run_loop

Run event loop.
[clinic start generated code]*/

static PyObject *
promisedio_run_loop_impl(PyObject *module)
/*[clinic end generated code: output=85e004b460bfcf13 input=db37076872c5af31]*/
{
    if (Loop_Run()) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.close_loop

Close event loop.
[clinic start generated code]*/

static PyObject *
promisedio_close_loop_impl(PyObject *module)
/*[clinic end generated code: output=85b576004e22a3f6 input=2719b6868cff99dd]*/
{
    if (Loop_Close()) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.check_can_fork

[clinic start generated code]*/

static PyObject *
promisedio_check_can_fork_impl(PyObject *module)
/*[clinic end generated code: output=fc0d77cc39bd1fe9 input=b5e47eb7aedda36c]*/
{
    if (Loop_IsCreated()) {
        fprintf(stderr, "RuntimeWarning: It's unsafe to use fork. Run Loop_Close before fork.\n");
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio._inspectloop

Prints all handles associated with the given loop.

Warning This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.
[clinic start generated code]*/

static PyObject *
promisedio__inspectloop_impl(PyObject *module)
/*[clinic end generated code: output=3fb3fc307ee4edc6 input=bf98e7d27d404965]*/
{
    uv_print_all_handles(Loop_Get(), stderr);
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.astat
    name: path
    *
    follow_symlinks: bool = True

Async equivalent of os.stat().
[clinic start generated code]*/

static PyObject *
promisedio_astat_impl(PyObject *module, PyObject *name, int follow_symlinks)
/*[clinic end generated code: output=2c0e909e379a9ecf input=57322c086a203a26]*/
{
    return (PyObject *) Fs_stat(PyBytes_AS_STRING(name), follow_symlinks);
}

/*[clinic input]
promisedio.afstat
    fd: int

Async equivalent of os.fstat().
[clinic start generated code]*/

static PyObject *
promisedio_afstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=0f5b65d5830d63e3 input=efe5a7502bdd6381]*/
{
    return (PyObject *) Fs_fstat(fd);
}

/*[clinic input]
promisedio.aseek
    fd: int
    pos: Py_ssize_t
    how: int

Async equivalent of os.lseek().
[clinic start generated code]*/

static PyObject *
promisedio_aseek_impl(PyObject *module, int fd, Py_ssize_t pos, int how)
/*[clinic end generated code: output=7ba6c05a3a2ac821 input=740a31387b83fc1d]*/
{
    return (PyObject *) Fs_seek(fd, pos, how);
}

/*[clinic input]
promisedio.aopenfd
    name: path
    flags: str = "r"
    mode: int = 0o666

Async equivalent of os.open().
[clinic start generated code]*/

static PyObject *
promisedio_aopenfd_impl(PyObject *module, PyObject *name, const char *flags,
                        int mode)
/*[clinic end generated code: output=b540faccc179cab2 input=042710841eb76b91]*/
{
    return (PyObject *) Fs_open(PyBytes_AS_STRING(name), flags, mode);
}

/*[clinic input]
promisedio.aclose
    fd: int

Async equivalent of os.close().
[clinic start generated code]*/

static PyObject *
promisedio_aclose_impl(PyObject *module, int fd)
/*[clinic end generated code: output=8bee8b49aeab6a3c input=fbbf1c56cc362359]*/
{
    return (PyObject *) Fs_close(fd);
}

/*[clinic input]
promisedio.aread
    fd: int
    size: Py_ssize_t = -1
    offset: Py_ssize_t = -1

Async equivalent of os.read().
[clinic start generated code]*/

static PyObject *
promisedio_aread_impl(PyObject *module, int fd, Py_ssize_t size,
                      Py_ssize_t offset)
/*[clinic end generated code: output=bf7079c627b8be8e input=51bfdd0ce090c2d3]*/
{
    if (size < 0) {
        return (PyObject *) Fs_readall(fd, offset);
    } else {
        return (PyObject *) Fs_read(fd, size, offset);
    }
}

/*[clinic input]
promisedio.awrite
    fd: int
    data: object
    offset: Py_ssize_t = -1

Async equivalent of os.write().
[clinic start generated code]*/

static PyObject *
promisedio_awrite_impl(PyObject *module, int fd, PyObject *data,
                       Py_ssize_t offset)
/*[clinic end generated code: output=7cf412bfa8880d3c input=0ef935ba3f15fbfd]*/
{
    return (PyObject *) Fs_write(fd, data, offset);
}

/*[clinic input]
promisedio.aopen
    name: object
    flags: str = "r"
    mode: int = 0o666

Async equivalent of open().
[clinic start generated code]*/

static PyObject *
promisedio_aopen_impl(PyObject *module, PyObject *name, const char *flags,
                      int mode)
/*[clinic end generated code: output=54ea524453ba1fc6 input=40e3b267203c0381]*/
{
    return FileIO_Open(name, flags, mode);
}

static void
module_free(PyObject *module)
{
}

static struct PyModuleDef iomodule = {
    PyModuleDef_HEAD_INIT,
    "_io",
    NULL,
    -1,
    module_methods,
    NULL,
    NULL,
    NULL,
    (freefunc) module_free
};

PyMODINIT_FUNC
PyInit__cext(void)
{
    PyObject *m = PyModule_Create(&iomodule);
    if (m == NULL) {
        return NULL;
    }
    if (GeneralTypes_module_init()) {
        return NULL;
    }
    if (Promise_module_init()) {
        return NULL;
    }
    if (Fs_module_init()) {
        return NULL;
    }
    if (FileIO_module_init()) {
        return NULL;
    }
    return m;
}
