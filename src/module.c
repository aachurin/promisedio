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
class Path_converter(CConverter):
    type = "PyObject *"
    converter = "Path_converter"
    c_default = "NULL"
    def cleanup(self):
        return f"Py_XDECREF({self.name});"

class File_converter(CConverter):
    type = "int"
    converter = "File_converter"

class Py_off_t_converter(CConverter):
    type = "Py_off_t"
    converter = "Py_off_t_converter"

[python start generated code]*/
/*[python end generated code: output=da39a3ee5e6b4b0d input=95148387e689d4f2]*/

static int
Path_converter(PyObject *arg, PyObject **addr)
{
    *addr = Fs_Path(arg);
    if (*addr == NULL)
        return 0;
    return 1;
}

static int
Py_off_t_converter(PyObject *arg, Py_off_t *addr)
{
    *addr = PyLong_AsOff_t(arg);
    if (PyErr_Occurred())
        return 0;
    return 1;
}

static int
File_converter(PyObject *arg, int *addr)
{
    int fd = _PyLong_AsInt(arg);
    if (fd < 0) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "negative file descriptor");
        }
        return 0;
    }
    *addr = fd;
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
    PROMISEDIO_RUN_LOOP_METHODDEF
    PROMISEDIO_CLOSE_LOOP_METHODDEF
    PROMISEDIO_USE_PYTHON_DEFAULT_SIGINT_METHODDEF
    PROMISEDIO_CHECK_CAN_FORK_METHODDEF
    PROMISEDIO__INSPECTLOOP_METHODDEF
    PROMISEDIO_CLOSE_METHODDEF
    PROMISEDIO_FSTAT_METHODDEF
    PROMISEDIO_OPENFD_METHODDEF
    PROMISEDIO_OPEN_METHODDEF
    PROMISEDIO_READ_METHODDEF
    PROMISEDIO_WRITE_METHODDEF
    PROMISEDIO_STAT_METHODDEF
    PROMISEDIO_SEEK_METHODDEF
    PROMISEDIO_UNLINK_METHODDEF
    PROMISEDIO_MKDIR_METHODDEF
    PROMISEDIO_RMDIR_METHODDEF
    PROMISEDIO_MKDTEMP_METHODDEF
    PROMISEDIO_MKSTEMP_METHODDEF
    PROMISEDIO_SCANDIR_METHODDEF
    PROMISEDIO_RENAME_METHODDEF
    PROMISEDIO_FSYNC_METHODDEF
    PROMISEDIO_FTRUNCATE_METHODDEF
    PROMISEDIO_FDATASYNC_METHODDEF
    PROMISEDIO_COPYFILE_METHODDEF
    PROMISEDIO_SENDFILE_METHODDEF
    PROMISEDIO_ACCESS_METHODDEF
    PROMISEDIO_CHMOD_METHODDEF
    PROMISEDIO_FCHMOD_METHODDEF
    PROMISEDIO_UTIME_METHODDEF
    PROMISEDIO_FUTIME_METHODDEF
    PROMISEDIO_LINK_METHODDEF
    PROMISEDIO_SYMLINK_METHODDEF
    PROMISEDIO_READLINK_METHODDEF
    PROMISEDIO_SLEEP_METHODDEF
    PROMISEDIO_SET_TIMEOUT_METHODDEF
    PROMISEDIO_SET_INTERVAL_METHODDEF
    PROMISEDIO_CLEAR_TIMEOUT_METHODDEF
    PROMISEDIO_CLEAR_INTERVAL_METHODDEF
    PROMISEDIO_TIME_METHODDEF
    PROMISEDIO_HRTIME_METHODDEF
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
[clinic start generated code]*/

static PyObject *
promisedio_deferred_impl(PyObject *module)
/*[clinic end generated code: output=cf587616d6ede614 input=58fd5ec8c6556282]*/
{
    return (PyObject *) Deferred_New();
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
promisedio.sleep
    seconds: double
[clinic start generated code]*/

static PyObject *
promisedio_sleep_impl(PyObject *module, double seconds)
/*[clinic end generated code: output=db7bf8f2c1e0f2da input=dd7b38342420d4a2]*/
{
    if (seconds < 0) {
        PyErr_SetString(PyExc_ValueError, "sleep length must be non-negative");
        return NULL;
    }
    return (PyObject *) Timer_Timeout((uint64_t) (seconds * 1000));
}

/*[clinic input]
promisedio.stat
    path: Path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static PyObject *
promisedio_stat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=652e62289cfd1010 input=98f7a21ff01529d7]*/
{
    if (follow_symlinks) {
        return (PyObject *) Fs_stat(PyBytes_AS_STRING(path));
    } else {
        return (PyObject *) Fs_lstat(PyBytes_AS_STRING(path));
    }
}

/*[clinic input]
promisedio.fstat
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_fstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=cb1c99d842780871 input=c35238d28ba2f7bb]*/
{
    return (PyObject *) Fs_fstat(fd);
}

/*[clinic input]
promisedio.seek
    fd: File
    pos: Py_off_t
    how: int
[clinic start generated code]*/

static PyObject *
promisedio_seek_impl(PyObject *module, int fd, Py_off_t pos, int how)
/*[clinic end generated code: output=abc2e2c73224c917 input=4b9c20aa787a3c37]*/
{
    return (PyObject *) Fs_seek(fd, pos, how);
}

/*[clinic input]
promisedio.open
    path: object
    flags: str = "r"
    closefd: bool(accept={int}) = True
[clinic start generated code]*/

static PyObject *
promisedio_open_impl(PyObject *module, PyObject *path, const char *flags,
                     int closefd)
/*[clinic end generated code: output=3992fbaabfcba0a1 input=5e582c492f82641b]*/
{
    return FileIO_Open(path, flags, closefd);
}


/*[clinic input]
promisedio.openfd
    path: Path
    flags: str = "r"
    mode: int = 0o666
[clinic start generated code]*/

static PyObject *
promisedio_openfd_impl(PyObject *module, PyObject *path, const char *flags,
                       int mode)
/*[clinic end generated code: output=4eb112caef422753 input=76377e3f41f79e0a]*/
{
    return (PyObject *) Fs_open(PyBytes_AS_STRING(path), flags, mode);
}

/*[clinic input]
promisedio.close
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_close_impl(PyObject *module, int fd)
/*[clinic end generated code: output=b78edd95980d5449 input=42c8429e8949095c]*/
{
    return (PyObject *) Fs_close(fd);
}

/*[clinic input]
promisedio.read
    fd: File
    size: Py_ssize_t = -1
    offset: Py_off_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_read_impl(PyObject *module, int fd, Py_ssize_t size,
                     Py_off_t offset)
/*[clinic end generated code: output=bf79255a1889159c input=5595f1d0d1c82909]*/
{
    if (size > _PY_READ_MAX) {
        size = _PY_READ_MAX;
    }
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
promisedio.write
    fd: File
    data: object
    offset: Py_off_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_write_impl(PyObject *module, int fd, PyObject *data,
                      Py_off_t offset)
/*[clinic end generated code: output=f9bd04b3053da614 input=f762c6588819b788]*/
{
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_write(fd, data, offset);
}

/*[clinic input]
promisedio.unlink
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_unlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=38e87ecd56c68fb5 input=57de5ab5fcfccd18]*/
{
    return (PyObject *) Fs_unlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.mkdir
    path: Path
    mode: int = 0o777
[clinic start generated code]*/

static PyObject *
promisedio_mkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=ce82906bb5a674d0 input=7ee16b48509dd5ab]*/
{
    return (PyObject *) Fs_mkdir(PyBytes_AS_STRING(path), mode);
}
/*[clinic input]
promisedio.rmdir
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_rmdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=dc2b65d4e534c0bd input=ac525908741e1a2b]*/
{
    return (PyObject *) Fs_rmdir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.mkdtemp
    tpl: Path
[clinic start generated code]*/

static PyObject *
promisedio_mkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=2eb44d496af8a8e1 input=3bd77b4e696b7dc4]*/
{
    return (PyObject *) Fs_mkdtemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.mkstemp
    tpl: Path
[clinic start generated code]*/

static PyObject *
promisedio_mkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=b15c3ed97b57b1df input=2a3626ee35287ceb]*/
{
    return (PyObject *) Fs_mkstemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.scandir
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_scandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=baf725f1c4b89c4e input=3f1532296bde072c]*/
{
    return (PyObject *) Fs_scandir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.rename
    path: Path
    new_path: Path
[clinic start generated code]*/

static PyObject *
promisedio_rename_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=e1f10cffaf4e78ac input=a8acdd2f39a2e794]*/
{
    return (PyObject *) Fs_rename(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.fsync
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_fsync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=53ec1853d450fac8 input=4cc81acd59ff1d0d]*/
{
    return (PyObject *) Fs_fsync(fd);
}

/*[clinic input]
promisedio.ftruncate
    fd: File
    length: Py_ssize_t
[clinic start generated code]*/

static PyObject *
promisedio_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length)
/*[clinic end generated code: output=5cf1eb0c1a81d227 input=62973f429c4c2d3e]*/
{
    return (PyObject *) Fs_ftruncate(fd, length);
}

/*[clinic input]
promisedio.fdatasync
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_fdatasync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=af4e82430b83379e input=7083f115af028b83]*/
{
    return (PyObject *) Fs_fdatasync(fd);
}

/*[clinic input]
promisedio.copyfile
    path: Path
    new_path: Path
    flags: int = 0
[clinic start generated code]*/

static PyObject *
promisedio_copyfile_impl(PyObject *module, PyObject *path,
                         PyObject *new_path, int flags)
/*[clinic end generated code: output=2f0c7b81477c4d1d input=6042223cbe47d6f3]*/
{
    return (PyObject *) Fs_copyfile(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.sendfile
    out_fd: File
    in_fd: File
    offset: Py_off_t
    count: Py_ssize_t
[clinic start generated code]*/

static PyObject *
promisedio_sendfile_impl(PyObject *module, int out_fd, int in_fd,
                         Py_off_t offset, Py_ssize_t count)
/*[clinic end generated code: output=b923801061026b96 input=985be2863e29b81d]*/
{
    return (PyObject *) Fs_sendfile(out_fd, in_fd, offset, count);
}

/*[clinic input]
promisedio.access
    path: Path
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_access_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=72c3aad178635ebc input=f073207770803da8]*/
{
    return (PyObject *) Fs_access(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.chmod
    path: Path
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_chmod_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=8ca2aac3be1d8e63 input=abc0615429085635]*/
{
    return (PyObject *) Fs_chmod(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.fchmod
    fd: File
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_fchmod_impl(PyObject *module, int fd, int mode)
/*[clinic end generated code: output=4e93ae801a3a1490 input=1586dd08e22bfef0]*/
{
    return (PyObject *) Fs_fchmod(fd, mode);
}

/*[clinic input]
promisedio.utime
    path: Path
    atime: double
    mtime: double,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static PyObject *
promisedio_utime_impl(PyObject *module, PyObject *path, double atime,
                      double mtime, int follow_symlinks)
/*[clinic end generated code: output=c3745e3c7cd89cb8 input=fe6f289d1be121b7]*/
{
    if (follow_symlinks) {
        return (PyObject *) Fs_utime(PyBytes_AS_STRING(path), atime, mtime);
    } else {
        return (PyObject *) Fs_lutime(PyBytes_AS_STRING(path), atime, mtime);
    }
}

/*[clinic input]
promisedio.futime
    fd: File
    atime: double
    mtime: double
[clinic start generated code]*/

static PyObject *
promisedio_futime_impl(PyObject *module, int fd, double atime, double mtime)
/*[clinic end generated code: output=52d2f312ca87055b input=6799823d206904f6]*/
{
    return (PyObject *) Fs_futime(fd, atime, mtime);
}

/*[clinic input]
promisedio.link
    path: Path
    new_path: Path
[clinic start generated code]*/

static PyObject *
promisedio_link_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=1afbef79f6c14811 input=063d87676e3fe9fc]*/
{
    return (PyObject *) Fs_link(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.symlink
    path: Path
    new_path: Path
    *
    flags: int = 0
[clinic start generated code]*/

static PyObject *
promisedio_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                        int flags)
/*[clinic end generated code: output=eb5fb90691d7c69a input=842c5a24a3ab8b5a]*/
{
    return (PyObject *) Fs_symlink(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.readlink
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_readlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=d2325c5a5efbf14b input=dc317bb4642016c4]*/
{
    return (PyObject *) Fs_readlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.set_timeout
    func: object
    timeout: double
    *
    unref: bool = False
[clinic start generated code]*/

static PyObject *
promisedio_set_timeout_impl(PyObject *module, PyObject *func, double timeout,
                            int unref)
/*[clinic end generated code: output=7830188782121ba5 input=45e7907e13826664]*/
{
    if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError,
                        "func must be a callable");
        return NULL;
    }
    if (timeout < 0) {
        PyErr_SetString(PyExc_ValueError, "timeout must be non-negative");
        return NULL;
    }
    return Timer_Start(func, (uint64_t)(timeout * 1000), 0, unref);
}

/*[clinic input]
promisedio.set_interval
    func: object
    interval: double
    *
    unref: bool = False
[clinic start generated code]*/

static PyObject *
promisedio_set_interval_impl(PyObject *module, PyObject *func,
                             double interval, int unref)
/*[clinic end generated code: output=61a00a347fe50e29 input=2774738377b73d06]*/
{
    if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError,
                        "func must be a callable");
        return NULL;
    }
    if (interval < 0) {
        PyErr_SetString(PyExc_ValueError, "interval must be non-negative");
        return NULL;
    }
    uint64_t timeout = (uint64_t)(interval * 1000);
    return Timer_Start(func, timeout, timeout, unref);
}

/*[clinic input]
promisedio.clear_timeout
    timer: object
[clinic start generated code]*/

static PyObject *
promisedio_clear_timeout_impl(PyObject *module, PyObject *timer)
/*[clinic end generated code: output=0c237737c2271ecb input=8ed924841921741b]*/
{
    if (Timer_Stop(timer)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.clear_interval
    timer: object
[clinic start generated code]*/

static PyObject *
promisedio_clear_interval_impl(PyObject *module, PyObject *timer)
/*[clinic end generated code: output=61b5703294ec341e input=65a93076b09802c7]*/
{
    if (Timer_Stop(timer)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.time
[clinic start generated code]*/

static PyObject *
promisedio_time_impl(PyObject *module)
/*[clinic end generated code: output=2e8841c557eac88e input=ef4702cbf9b07e0e]*/
{
    return PyLong_FromUint64_t(uv_now(Loop_Get()));
}

/*[clinic input]
promisedio.hrtime
[clinic start generated code]*/

static PyObject *
promisedio_hrtime_impl(PyObject *module)
/*[clinic end generated code: output=fb5badbdb221edf9 input=50ec38aaab616da8]*/
{
    return PyLong_FromUint64_t(uv_hrtime());
}

static void
module_free(PyObject *module)
{
}

static struct PyModuleDef promisediomodule = {
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
    PyObject *m = PyModule_Create(&promisediomodule);
    if (m == NULL) {
        goto except;
    }
    if (Promise_module_init()) {
        goto except;
    }
    if (Fs_module_init(m)) {
        goto except;
    }
    if (FileIO_module_init()) {
        goto except;
    }
    if (Timer_module_init()) {
        goto except;
    }
    goto finally;
    except:
    Py_CLEAR(m);
    finally:
    return m;
}
