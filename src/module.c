// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "common.h"
//#include "general_types.h"
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

static PyMethodDef module_methods[] = {
    PROMISEDIO__GETALLOCATEDOBJECTSCOUNT_METHODDEF
    PROMISEDIO__PRINTMEMINFO_METHODDEF
    PROMISEDIO__CLEARFREELISTS_METHODDEF
    PROMISEDIO_PROCESS_PROMISE_CHAIN_METHODDEF
    PROMISEDIO_DEFERRED_METHODDEF
    PROMISEDIO_EXEC_ASYNC_METHODDEF
    PROMISEDIO_ASLEEP_METHODDEF
    PROMISEDIO_RUN_LOOP_METHODDEF
    PROMISEDIO_CLOSE_LOOP_METHODDEF
    PROMISEDIO_USE_PYTHON_DEFAULT_SIGINT_METHODDEF
    PROMISEDIO_CHECK_CAN_FORK_METHODDEF
    PROMISEDIO__INSPECTLOOP_METHODDEF
    PROMISEDIO_ACLOSE_METHODDEF
    PROMISEDIO_AFSTAT_METHODDEF
    PROMISEDIO_AOPENFD_METHODDEF
    PROMISEDIO_AOPEN_METHODDEF
    PROMISEDIO_AREAD_METHODDEF
    PROMISEDIO_AWRITE_METHODDEF
    PROMISEDIO_ASTAT_METHODDEF
    PROMISEDIO_ASEEK_METHODDEF
    PROMISEDIO_AUNLINK_METHODDEF
    PROMISEDIO_AMKDIR_METHODDEF
    PROMISEDIO_ARMDIR_METHODDEF
    PROMISEDIO_AMKDTEMP_METHODDEF
    PROMISEDIO_AMKSTEMP_METHODDEF
    PROMISEDIO_ASCANDIR_METHODDEF
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
promisedio._printmeminfo

Memory debug info
[clinic start generated code]*/

static PyObject *
promisedio__printmeminfo_impl(PyObject *module)
/*[clinic end generated code: output=44a238f01c620815 input=8411c0de2085c335]*/
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
promisedio.asleep
    seconds: double

Delay execution for a given number of seconds.
[clinic start generated code]*/

static PyObject *
promisedio_asleep_impl(PyObject *module, double seconds)
/*[clinic end generated code: output=fb41f697d719628e input=d7fea55f3ef07db0]*/
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

Warning: This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.
[clinic start generated code]*/

static PyObject *
promisedio__inspectloop_impl(PyObject *module)
/*[clinic end generated code: output=3fb3fc307ee4edc6 input=439b20f545d96ff0]*/
{
    uv_print_all_handles(Loop_Get(), stderr);
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.astat
    path: path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static PyObject *
promisedio_astat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=2aa73717bcf3edb4 input=a5a1f982de67e864]*/
{
    return (PyObject *) Fs_stat(PyBytes_AS_STRING(path), follow_symlinks);
}

/*[clinic input]
promisedio.afstat
    fd: int
[clinic start generated code]*/

static PyObject *
promisedio_afstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=0f5b65d5830d63e3 input=3e684f625c6e0f01]*/
{
    return (PyObject *) Fs_fstat(fd);
}

/*[clinic input]
promisedio.aseek
    fd: int
    pos: Py_ssize_t
    how: int
[clinic start generated code]*/

static PyObject *
promisedio_aseek_impl(PyObject *module, int fd, Py_ssize_t pos, int how)
/*[clinic end generated code: output=7ba6c05a3a2ac821 input=421dca383d97879a]*/
{
    return (PyObject *) Fs_seek(fd, pos, how);
}

/*[clinic input]
promisedio.aopen
    path: object
    flags: str = "r"
    closefd: bool(accept={int}) = True
[clinic start generated code]*/

static PyObject *
promisedio_aopen_impl(PyObject *module, PyObject *path, const char *flags,
                      int closefd)
/*[clinic end generated code: output=6eded1a7f23de4c4 input=6f63ab1e972e741f]*/
{
    return FileIO_Open(path, flags, closefd);
}


/*[clinic input]
promisedio.aopenfd
    path: path
    flags: str = "r"
    mode: int = 0o666
[clinic start generated code]*/

static PyObject *
promisedio_aopenfd_impl(PyObject *module, PyObject *path, const char *flags,
                        int mode)
/*[clinic end generated code: output=265bc1a4b4bffa85 input=7135552cf7140b06]*/
{
    return (PyObject *) Fs_open(PyBytes_AS_STRING(path), flags, mode);
}

/*[clinic input]
promisedio.aclose
    fd: int
[clinic start generated code]*/

static PyObject *
promisedio_aclose_impl(PyObject *module, int fd)
/*[clinic end generated code: output=8bee8b49aeab6a3c input=63b58ef01d650ac9]*/
{
    return (PyObject *) Fs_close(fd);
}

/*[clinic input]
promisedio.aread
    fd: int
    size: Py_ssize_t = -1
    offset: Py_ssize_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_aread_impl(PyObject *module, int fd, Py_ssize_t size,
                      Py_ssize_t offset)
/*[clinic end generated code: output=bf7079c627b8be8e input=2ed1011b13d0b5d3]*/
{
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be specified in conjunction with the size parameter");
            return NULL;
        }
        return (PyObject *) Fs_readall(fd);
    } else {
        return (PyObject *) Fs_read(fd, size, offset);
    }
}

/*[clinic input]
promisedio.awrite
    fd: int
    data: object
    offset: Py_ssize_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_awrite_impl(PyObject *module, int fd, PyObject *data,
                       Py_ssize_t offset)
/*[clinic end generated code: output=7cf412bfa8880d3c input=6fd0212846920a83]*/
{
    return (PyObject *) Fs_write(fd, data, offset);
}

/*[clinic input]
promisedio.aunlink
    path: path
[clinic start generated code]*/

static PyObject *
promisedio_aunlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=9f211a2a10ad0e95 input=f63624eb7e8bc6a1]*/
{
    return (PyObject *) Fs_unlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.amkdir
    path: path
    mode: int = 0o777
[clinic start generated code]*/

static PyObject *
promisedio_amkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=c9a4643e07356ea7 input=98ed47135f76fd34]*/
{
    return (PyObject *) Fs_mkdir(PyBytes_AS_STRING(path), mode);
}
/*[clinic input]
promisedio.armdir
    path: path
[clinic start generated code]*/

static PyObject *
promisedio_armdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=059f06a8acdd7383 input=6e19f73a872f8b56]*/
{
    return (PyObject *) Fs_rmdir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.amkdtemp
    tpl: path
[clinic start generated code]*/

static PyObject *
promisedio_amkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=e0c1506fcefab432 input=6d88427f5e223fb4]*/
{
    return (PyObject *) Fs_mkdtemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.amkstemp
    tpl: path
[clinic start generated code]*/

static PyObject *
promisedio_amkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=c08feef6f593c303 input=0efed5914c8d9fef]*/
{
    return (PyObject *) Fs_mkstemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.ascandir
    path: path
[clinic start generated code]*/

static PyObject *
promisedio_ascandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=dee5ea789f520bfe input=2987e9c7f9f92114]*/
{
    return (PyObject *) Fs_scandir(PyBytes_AS_STRING(path));
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
//    if (GeneralTypes_module_init()) {
//        return NULL;
//    }
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
