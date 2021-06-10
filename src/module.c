// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#include <uv.h>
#include "common.h"
#include "promise.h"
#include "timer.h"
#include "fs.h"
#include "loop.h"
#include "ns.h"
#include "error.h"

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
    PROMISEDIO_GETADDRINFO_METHODDEF
    PROMISEDIO_GETNAMEINFO_METHODDEF
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

static inline PyObject *
promisedio__getallocatedobjectscount_impl(PyObject *module)
/*[clinic end generated code: output=78ee3f549d41f1ec input=3653fe3582a0c7df]*/
{
    return PyLong_FromSize_t(mem_alloc_count());
}

/*[clinic input]
promisedio._printmeminfo

Memory debug info
[clinic start generated code]*/

static inline PyObject *
promisedio__printmeminfo_impl(PyObject *module)
/*[clinic end generated code: output=4672078ac623b846 input=8411c0de2085c335]*/
{
    mem_debug_info();
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio._clearfreelists

Clear freelists
[clinic start generated code]*/

static inline PyObject *
promisedio__clearfreelists_impl(PyObject *module)
/*[clinic end generated code: output=c018206e005ea267 input=5134b1016c9494a9]*/
{
    mem_clear_freelists();
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.exec_async
    coro: object(subclass_of='&PyCoro_Type')
    /

Schedule coroutine execution.
[clinic start generated code]*/

static inline PyObject *
promisedio_exec_async_impl(PyObject *module, PyObject *coro)
/*[clinic end generated code: output=f80415fd23a668f9 input=b4b82522f580b96c]*/
{
    if (promise_exec_async(coro)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.process_promise_chain -> bool

Process promise async chain once.
[clinic start generated code]*/

static inline int
promisedio_process_promise_chain_impl(PyObject *module)
/*[clinic end generated code: output=bda7976b28f227f9 input=7b52b2c804a788a8]*/
{
    return promise_process_chain();
}

/*[clinic input]
promisedio.deferred
[clinic start generated code]*/

static inline PyObject *
promisedio_deferred_impl(PyObject *module)
/*[clinic end generated code: output=eda3b94d3cc8b309 input=58fd5ec8c6556282]*/
{
    return (PyObject *) deferred_new();
}

/*[clinic input]
promisedio.use_python_default_sigint
    flag: bool

Use python default SIGINT handler.
[clinic start generated code]*/

static inline PyObject *
promisedio_use_python_default_sigint_impl(PyObject *module, int flag)
/*[clinic end generated code: output=629df2435c5de933 input=78af53302a06dc97]*/
{
    LoopConfig *config = loop_get_config();
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

static inline PyObject *
promisedio_run_loop_impl(PyObject *module)
/*[clinic end generated code: output=eee980c99f23e09c input=db37076872c5af31]*/
{
    if (loop_run()) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.close_loop

Close event loop.
[clinic start generated code]*/

static inline PyObject *
promisedio_close_loop_impl(PyObject *module)
/*[clinic end generated code: output=32d2480c1e923efe input=2719b6868cff99dd]*/
{
    if (loop_close()) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.check_can_fork

[clinic start generated code]*/

static inline PyObject *
promisedio_check_can_fork_impl(PyObject *module)
/*[clinic end generated code: output=60685c271edecbe4 input=b5e47eb7aedda36c]*/
{
    if (loop_is_created()) {
        fprintf(stderr, "RuntimeWarning: It's unsafe to use fork. Run loop_close before fork.\n");
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio._inspectloop

Prints all handles associated with the given loop.

Warning: This function is meant for ad hoc debugging, there is no API/ABI stability guarantees.
[clinic start generated code]*/

static inline PyObject *
promisedio__inspectloop_impl(PyObject *module)
/*[clinic end generated code: output=8f8d48d85d2f632a input=439b20f545d96ff0]*/
{
    uv_print_all_handles(loop_get(), stderr);
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.sleep
    seconds: double
[clinic start generated code]*/

static inline PyObject *
promisedio_sleep_impl(PyObject *module, double seconds)
/*[clinic end generated code: output=8a88aab6f9fb1be9 input=dd7b38342420d4a2]*/
{
    if (seconds < 0) {
        PyErr_SetString(PyExc_ValueError, "sleep length must be non-negative");
        return NULL;
    }
    return (PyObject *) timer_timeout((uint64_t) (seconds * 1000));
}

/*[clinic input]
promisedio.stat
    path: Path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static inline PyObject *
promisedio_stat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=ed0b1c09085669df input=98f7a21ff01529d7]*/
{
    if (follow_symlinks) {
        return (PyObject *) fs_stat(PyBytes_AS_STRING(path));
    } else {
        return (PyObject *) fs_lstat(PyBytes_AS_STRING(path));
    }
}

/*[clinic input]
promisedio.fstat
    fd: FD
[clinic start generated code]*/

static inline PyObject *
promisedio_fstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=f3b6877ded2592af input=86f7d64645c008e3]*/
{
    return (PyObject *) fs_fstat(fd);
}

/*[clinic input]
promisedio.seek
    fd: FD
    pos: off_t
    how: int
[clinic start generated code]*/

static inline PyObject *
promisedio_seek_impl(PyObject *module, int fd, Py_off_t pos, int how)
/*[clinic end generated code: output=6dd01686171cb80a input=0d8750bdad86367f]*/
{
    return (PyObject *) fs_seek(fd, pos, how);
}

/*[clinic input]
promisedio.open
    path: object
    flags: cstring = "r"
    closefd: bool(accept={int}) = True
[clinic start generated code]*/

static inline PyObject *
promisedio_open_impl(PyObject *module, PyObject *path, const char *flags,
                     int closefd)
/*[clinic end generated code: output=be30497ea6c56495 input=991d28e6eccffe1c]*/
{
    if (PyNumber_Check(path)) {
        int fd = _PyLong_AsInt(path);
        if (fd < 0) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "negative file descriptor");
            }
            return NULL;
        }
        FileIO *fileobj = fileio_new(fd, closefd);
        if (!fileobj)
            return NULL;
        return (PyObject *) promise_new_resolved((PyObject *) fileobj);
    }
    if (!closefd) {
        PyErr_SetString(PyExc_ValueError, "Cannot use closefd=False with file name");
        return NULL;
    }
    PyObject *fspath;
    if (!PyUnicode_FSConverter(path, &fspath)) {
        return NULL;
    }
    PyObject *result = (PyObject *) fs_fileio_open(PyBytes_AS_STRING(fspath), flags);
    Py_DECREF(fspath);

    return result;
}

/*[clinic input]
promisedio.openfd
    path: Path
    flags: cstring = "r"
    mode: int = 0o666
[clinic start generated code]*/

static inline PyObject *
promisedio_openfd_impl(PyObject *module, PyObject *path, const char *flags,
                       int mode)
/*[clinic end generated code: output=062197276215e6d4 input=25bc023de82be48e]*/
{
    return (PyObject *) fs_open(PyBytes_AS_STRING(path), flags, mode);
}

/*[clinic input]
promisedio.close
    fd: FD
[clinic start generated code]*/

static inline PyObject *
promisedio_close_impl(PyObject *module, int fd)
/*[clinic end generated code: output=484218cfdf21f281 input=79abb0f3fe6f1b75]*/
{
    return (PyObject *) fs_close(fd);
}

/*[clinic input]
promisedio.read
    fd: FD
    size: ssize_t = -1
    offset: off_t = -1
[clinic start generated code]*/

static inline PyObject *
promisedio_read_impl(PyObject *module, int fd, Py_ssize_t size,
                     Py_off_t offset)
/*[clinic end generated code: output=14b4f5ac084e7b12 input=70a0cf743f84fc91]*/
{
    if (size < 0) {
        if (offset >= 0) {
            PyErr_SetString(PyExc_ValueError,
                            "offset could only be specified in conjunction with the size parameter");
            return NULL;
        }
        return (PyObject *) fs_readall(fd);
    } else {
        return (PyObject *) fs_read(fd, size, offset);
    }
}

/*[clinic input]
promisedio.write
    fd: FD
    data: object
    offset: off_t = -1
[clinic start generated code]*/

static inline PyObject *
promisedio_write_impl(PyObject *module, int fd, PyObject *data,
                      Py_off_t offset)
/*[clinic end generated code: output=33443daca17d9d42 input=9d08477df3f20c78]*/
{
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) fs_write(fd, data, offset);
}

/*[clinic input]
promisedio.unlink
    path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_unlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=2e48182cf50c59de input=57de5ab5fcfccd18]*/
{
    return (PyObject *) fs_unlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.mkdir
    path: Path
    mode: int = 0o777
[clinic start generated code]*/

static inline PyObject *
promisedio_mkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=8c92fe323dead50a input=7ee16b48509dd5ab]*/
{
    return (PyObject *) fs_mkdir(PyBytes_AS_STRING(path), mode);
}
/*[clinic input]
promisedio.rmdir
    path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_rmdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=335e2e8ae0007d02 input=ac525908741e1a2b]*/
{
    return (PyObject *) fs_rmdir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.mkdtemp
    tpl: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_mkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=d9a646634b92cb6d input=3bd77b4e696b7dc4]*/
{
    return (PyObject *) fs_mkdtemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.mkstemp
    tpl: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_mkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=66432e2abd6de73b input=2a3626ee35287ceb]*/
{
    return (PyObject *) fs_mkstemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.scandir
    path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_scandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=7649a68b578af592 input=3f1532296bde072c]*/
{
    return (PyObject *) fs_scandir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.rename
    path: Path
    new_path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_rename_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=14602ae10fe18834 input=a8acdd2f39a2e794]*/
{
    return (PyObject *) fs_rename(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.fsync
    fd: FD
[clinic start generated code]*/

static inline PyObject *
promisedio_fsync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=d45b2fa19537741f input=be3ce52521d64549]*/
{
    return (PyObject *) fs_fsync(fd);
}

/*[clinic input]
promisedio.ftruncate
    fd: FD
    length: ssize_t
[clinic start generated code]*/

static inline PyObject *
promisedio_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length)
/*[clinic end generated code: output=8d27f754e1c81c99 input=bf1b1a55b2d4ab63]*/
{
    return (PyObject *) fs_ftruncate(fd, length);
}

/*[clinic input]
promisedio.fdatasync
    fd: FD
[clinic start generated code]*/

static inline PyObject *
promisedio_fdatasync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=d9327260948d82ea input=b745be78bc1b2a57]*/
{
    return (PyObject *) fs_fdatasync(fd);
}

/*[clinic input]
promisedio.copyfile
    path: Path
    new_path: Path
    flags: int = 0
[clinic start generated code]*/

static inline PyObject *
promisedio_copyfile_impl(PyObject *module, PyObject *path,
                         PyObject *new_path, int flags)
/*[clinic end generated code: output=1ef924c579f2a771 input=6042223cbe47d6f3]*/
{
    return (PyObject *) fs_copyfile(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.sendfile
    out_fd: FD
    in_fd: FD
    offset: off_t
    count: ssize_t
[clinic start generated code]*/

static inline PyObject *
promisedio_sendfile_impl(PyObject *module, int out_fd, int in_fd,
                         Py_off_t offset, Py_ssize_t count)
/*[clinic end generated code: output=4c7a33fd423b16d4 input=1c0436b68a935ecb]*/
{
    return (PyObject *) fs_sendfile(out_fd, in_fd, offset, count);
}

/*[clinic input]
promisedio.access
    path: Path
    mode: int
[clinic start generated code]*/

static inline PyObject *
promisedio_access_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=91c7c8ec28d7fc6e input=f073207770803da8]*/
{
    return (PyObject *) fs_access(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.chmod
    path: Path
    mode: int
[clinic start generated code]*/

static inline PyObject *
promisedio_chmod_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=7448ff201c6d4a07 input=abc0615429085635]*/
{
    return (PyObject *) fs_chmod(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.fchmod
    fd: FD
    mode: int
[clinic start generated code]*/

static inline PyObject *
promisedio_fchmod_impl(PyObject *module, int fd, int mode)
/*[clinic end generated code: output=3c5dd644355ab787 input=b31a3a1aef56a8a6]*/
{
    return (PyObject *) fs_fchmod(fd, mode);
}

/*[clinic input]
promisedio.utime
    path: Path
    atime: double
    mtime: double,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static inline PyObject *
promisedio_utime_impl(PyObject *module, PyObject *path, double atime,
                      double mtime, int follow_symlinks)
/*[clinic end generated code: output=ba1936e56f90341d input=fe6f289d1be121b7]*/
{
    if (follow_symlinks) {
        return (PyObject *) fs_utime(PyBytes_AS_STRING(path), atime, mtime);
    } else {
        return (PyObject *) fs_lutime(PyBytes_AS_STRING(path), atime, mtime);
    }
}

/*[clinic input]
promisedio.futime
    fd: FD
    atime: double
    mtime: double
[clinic start generated code]*/

static inline PyObject *
promisedio_futime_impl(PyObject *module, int fd, double atime, double mtime)
/*[clinic end generated code: output=95384ca2e21156b6 input=ca62fd72432c6674]*/
{
    return (PyObject *) fs_futime(fd, atime, mtime);
}

/*[clinic input]
promisedio.link
    path: Path
    new_path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_link_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=072c45f98c1fec26 input=063d87676e3fe9fc]*/
{
    return (PyObject *) fs_link(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.symlink
    path: Path
    new_path: Path
    *
    flags: int = 0
[clinic start generated code]*/

static inline PyObject *
promisedio_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                        int flags)
/*[clinic end generated code: output=f27c776fb4e967e1 input=842c5a24a3ab8b5a]*/
{
    return (PyObject *) fs_symlink(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.readlink
    path: Path
[clinic start generated code]*/

static inline PyObject *
promisedio_readlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=10d44834592c2911 input=dc317bb4642016c4]*/
{
    return (PyObject *) fs_readlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.set_timeout
    func: object
    timeout: double
    *
    unref: bool = False
[clinic start generated code]*/

static inline PyObject *
promisedio_set_timeout_impl(PyObject *module, PyObject *func, double timeout,
                            int unref)
/*[clinic end generated code: output=2086d3347b1ed122 input=45e7907e13826664]*/
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
    return timer_start(func, (uint64_t) (timeout * 1000), 0, unref);
}

/*[clinic input]
promisedio.set_interval
    func: object
    interval: double
    *
    unref: bool = False
[clinic start generated code]*/

static inline PyObject *
promisedio_set_interval_impl(PyObject *module, PyObject *func,
                             double interval, int unref)
/*[clinic end generated code: output=3d077e9a8457ac99 input=2774738377b73d06]*/
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
    return timer_start(func, timeout, timeout, unref);
}

/*[clinic input]
promisedio.clear_timeout
    timer: object
[clinic start generated code]*/

static inline PyObject *
promisedio_clear_timeout_impl(PyObject *module, PyObject *timer)
/*[clinic end generated code: output=74e3729a3f1233ad input=8ed924841921741b]*/
{
    if (timer_stop(timer)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.clear_interval
    timer: object
[clinic start generated code]*/

static inline PyObject *
promisedio_clear_interval_impl(PyObject *module, PyObject *timer)
/*[clinic end generated code: output=29696f06cdf7ad77 input=65a93076b09802c7]*/
{
    if (timer_stop(timer)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
promisedio.time
[clinic start generated code]*/

static inline PyObject *
promisedio_time_impl(PyObject *module)
/*[clinic end generated code: output=f3baf8ebabe0f475 input=ef4702cbf9b07e0e]*/
{
    return PyLong_FromUint64_t(uv_now(loop_get()));
}

/*[clinic input]
promisedio.hrtime
[clinic start generated code]*/

static inline PyObject *
promisedio_hrtime_impl(PyObject *module)
/*[clinic end generated code: output=414b25747aa7f3a3 input=50ec38aaab616da8]*/
{
    return PyLong_FromUint64_t(uv_hrtime());
}

/*[clinic input]
promisedio.getaddrinfo
    node: cstring(accept={NoneType})
    service: object
    family: int = 0
    type: int = 0
    proto: int = 0
    flags: int(c_default="AF_UNSPEC") = 0
[clinic start generated code]*/

static inline PyObject *
promisedio_getaddrinfo_impl(PyObject *module, const char *node,
                            PyObject *service, int family, int type,
                            int proto, int flags)
/*[clinic end generated code: output=41fab6a467055122 input=35b209c7ddf4262f]*/
{
    char buf[30];
    const char *service_ptr;
    if (PyLong_Check(service)) {
        long value = PyLong_AsLong(service);
        if (value == -1 && PyErr_Occurred())
            return NULL;
        PyOS_snprintf(buf, sizeof(buf), "%ld", value);
        service_ptr = buf;
    } else if (!cstring_optional_converter(service, &service_ptr)) {
        return NULL;
    }
    return (PyObject *) ns_getaddrinfo(node, service_ptr, family, type, proto, flags);
}

/*[clinic input]
promisedio.getnameinfo
    sockaddr: object
    flags: int
[clinic start generated code]*/

static inline PyObject *
promisedio_getnameinfo_impl(PyObject *module, PyObject *sockaddr, int flags)
/*[clinic end generated code: output=100155b57644db4b input=d2d58b701be7cfe5]*/
{
    const char *node;
    if (!PyTuple_Check(sockaddr)) {
        PyErr_SetString(PyExc_TypeError, "sockaddr must be a tuple");
        return NULL;
    }
    Py_ssize_t sockaddr_size = PyTuple_GET_SIZE(sockaddr);
    if (sockaddr_size < 2 || sockaddr_size > 4) {
        PyErr_SetString(PyExc_TypeError, "illegal sockaddr argument");
        return NULL;
    }
    if (!cstring_converter(PyTuple_GET_ITEM(sockaddr, 0), &node))
        return NULL;

    char buf[30];
    const char *service;
    if (PyLong_Check(PyTuple_GET_ITEM(sockaddr, 1))) {
        long value = PyLong_AsLong(PyTuple_GET_ITEM(sockaddr, 1));
        if (value == -1 && PyErr_Occurred())
            return NULL;
        PyOS_snprintf(buf, sizeof(buf), "%ld", value);
        service = buf;
    } else if (!cstring_converter(PyTuple_GET_ITEM(sockaddr, 1), &service))
        return NULL;

    unsigned int flowinfo, scope_id;
    flowinfo = scope_id = 0;
    if (sockaddr_size > 2) {
        if (!_PyLong_UnsignedInt_Converter(PyTuple_GET_ITEM(sockaddr, 2), &flowinfo))
            return NULL;
        if (sockaddr_size > 3) {
            if (!_PyLong_UnsignedInt_Converter(PyTuple_GET_ITEM(sockaddr, 3), &scope_id))
                return NULL;
        }
    }
    if (flowinfo > 0xfffff) {
        PyErr_SetString(PyExc_OverflowError,
                        "flowinfo must be 0-1048575.");
        return NULL;
    }

    return (PyObject *) ns_getnameinfo(node, service, flowinfo, scope_id, flags);
}

static void
module_free(PyObject *module)
{
}

static struct PyModuleDef promisediomodule = {
    PyModuleDef_HEAD_INIT,
    "promisedio",
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
    if (promise_module_init()) {
        goto except;
    }
    if (fs_module_init(m)) {
        goto except;
    }
    if (timer_module_init()) {
        goto except;
    }
    if (ns_module_init(m)) {
        goto except;
    }
    if (error_module_init(m)) {
        goto except;
    }
    goto finally;
    except:
    Py_CLEAR(m);
    finally:
    return m;
}
