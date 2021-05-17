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
    PROMISEDIO_ARENAME_METHODDEF
    PROMISEDIO_AFSYNC_METHODDEF
    PROMISEDIO_AFTRUNCATE_METHODDEF
    PROMISEDIO_AFDATASYNC_METHODDEF
    PROMISEDIO_ACOPYFILE_METHODDEF
    PROMISEDIO_ASENDFILE_METHODDEF
    PROMISEDIO_AACCESS_METHODDEF
    PROMISEDIO_ACHMOD_METHODDEF
    PROMISEDIO_AFCHMOD_METHODDEF
    PROMISEDIO_AUTIME_METHODDEF
    PROMISEDIO_AFUTIME_METHODDEF
    PROMISEDIO_ALINK_METHODDEF
    PROMISEDIO_ASYMLINK_METHODDEF
    PROMISEDIO_AREADLINK_METHODDEF
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
    path: Path
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static PyObject *
promisedio_astat_impl(PyObject *module, PyObject *path, int follow_symlinks)
/*[clinic end generated code: output=2aa73717bcf3edb4 input=1eae525c018de230]*/
{
    if (follow_symlinks) {
        return (PyObject *) Fs_stat(PyBytes_AS_STRING(path));
    } else {
        return (PyObject *) Fs_lstat(PyBytes_AS_STRING(path));
    }
}

/*[clinic input]
promisedio.afstat
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_afstat_impl(PyObject *module, int fd)
/*[clinic end generated code: output=0f5b65d5830d63e3 input=25172e09480328b8]*/
{
    return (PyObject *) Fs_fstat(fd);
}

/*[clinic input]
promisedio.aseek
    fd: File
    pos: Py_off_t
    how: int
[clinic start generated code]*/

static PyObject *
promisedio_aseek_impl(PyObject *module, int fd, Py_off_t pos, int how)
/*[clinic end generated code: output=5dbd6ef4a11dd7a4 input=eb630ef969522f29]*/
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
    path: Path
    flags: str = "r"
    mode: int = 0o666
[clinic start generated code]*/

static PyObject *
promisedio_aopenfd_impl(PyObject *module, PyObject *path, const char *flags,
                        int mode)
/*[clinic end generated code: output=265bc1a4b4bffa85 input=c298e0688e538e1e]*/
{
    return (PyObject *) Fs_open(PyBytes_AS_STRING(path), flags, mode);
}

/*[clinic input]
promisedio.aclose
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_aclose_impl(PyObject *module, int fd)
/*[clinic end generated code: output=8bee8b49aeab6a3c input=4ba3d55d304e7ca5]*/
{
    return (PyObject *) Fs_close(fd);
}

/*[clinic input]
promisedio.aread
    fd: File
    size: Py_ssize_t = -1
    offset: Py_off_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_aread_impl(PyObject *module, int fd, Py_ssize_t size,
                      Py_off_t offset)
/*[clinic end generated code: output=fbd0538cc6d692f9 input=515a15988c9efa4d]*/
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
promisedio.awrite
    fd: File
    data: object
    offset: Py_off_t = -1
[clinic start generated code]*/

static PyObject *
promisedio_awrite_impl(PyObject *module, int fd, PyObject *data,
                       Py_off_t offset)
/*[clinic end generated code: output=2a3a75cf4cc9d083 input=9853702859d3253d]*/
{
    if (!PyBytes_Check(data)) {
        PyErr_SetString(PyExc_TypeError,
                        "bytes expected");
        return NULL;
    }
    return (PyObject *) Fs_write(fd, data, offset);
}

/*[clinic input]
promisedio.aunlink
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_aunlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=9f211a2a10ad0e95 input=f43076b9c13f9032]*/
{
    return (PyObject *) Fs_unlink(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.amkdir
    path: Path
    mode: int = 0o777
[clinic start generated code]*/

static PyObject *
promisedio_amkdir_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=c9a4643e07356ea7 input=80bcebcd64a86a3d]*/
{
    return (PyObject *) Fs_mkdir(PyBytes_AS_STRING(path), mode);
}
/*[clinic input]
promisedio.armdir
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_armdir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=059f06a8acdd7383 input=5d8241a86ca9fef6]*/
{
    return (PyObject *) Fs_rmdir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.amkdtemp
    tpl: Path
[clinic start generated code]*/

static PyObject *
promisedio_amkdtemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=e0c1506fcefab432 input=2fdc23cb719cda0d]*/
{
    return (PyObject *) Fs_mkdtemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.amkstemp
    tpl: Path
[clinic start generated code]*/

static PyObject *
promisedio_amkstemp_impl(PyObject *module, PyObject *tpl)
/*[clinic end generated code: output=c08feef6f593c303 input=ef138fe86e39b662]*/
{
    return (PyObject *) Fs_mkstemp(PyBytes_AS_STRING(tpl));
}

/*[clinic input]
promisedio.ascandir
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_ascandir_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=dee5ea789f520bfe input=76e10f96ad568b81]*/
{
    return (PyObject *) Fs_scandir(PyBytes_AS_STRING(path));
}

/*[clinic input]
promisedio.arename
    path: Path
    new_path: Path
[clinic start generated code]*/

static PyObject *
promisedio_arename_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=8314bc89d7ace107 input=2f627c52c557cac2]*/
{
    return (PyObject *) Fs_rename(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.afsync
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_afsync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=de753b1909d8167d input=917d6e81a3ae1ae4]*/
{
    return (PyObject *) Fs_fsync(fd);
}

/*[clinic input]
promisedio.aftruncate
    fd: File
    length: Py_ssize_t
[clinic start generated code]*/

static PyObject *
promisedio_aftruncate_impl(PyObject *module, int fd, Py_ssize_t length)
/*[clinic end generated code: output=7fe0bf09f8cb00f3 input=a549ab93b2095b33]*/
{
    return (PyObject *) Fs_ftruncate(fd, length);
}

/*[clinic input]
promisedio.afdatasync
    fd: File
[clinic start generated code]*/

static PyObject *
promisedio_afdatasync_impl(PyObject *module, int fd)
/*[clinic end generated code: output=0854c85e7d2532e2 input=a571990618eb26cc]*/
{
    return (PyObject *) Fs_fdatasync(fd);
}

/*[clinic input]
promisedio.acopyfile
    path: Path
    new_path: Path
    flags: int = 0
[clinic start generated code]*/

static PyObject *
promisedio_acopyfile_impl(PyObject *module, PyObject *path,
                          PyObject *new_path, int flags)
/*[clinic end generated code: output=d17b509bae6e8a59 input=0fc46b7c030f87c1]*/
{
    return (PyObject *) Fs_copyfile(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.asendfile
    out_fd: File
    in_fd: File
    offset: Py_off_t
    count: Py_ssize_t
[clinic start generated code]*/

static PyObject *
promisedio_asendfile_impl(PyObject *module, int out_fd, int in_fd,
                          Py_off_t offset, Py_ssize_t count)
/*[clinic end generated code: output=94931445577c5baf input=8c5896dfb8bb6927]*/
{
    return (PyObject *) Fs_sendfile(out_fd, in_fd, offset, count);
}

/*[clinic input]
promisedio.aaccess
    path: Path
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_aaccess_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=06ec58f52c4ec73d input=c0d6e430132a5b07]*/
{
    return (PyObject *) Fs_access(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.achmod
    path: Path
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_achmod_impl(PyObject *module, PyObject *path, int mode)
/*[clinic end generated code: output=14abefb6dfb1b84d input=deeb60b0daf84a06]*/
{
    return (PyObject *) Fs_chmod(PyBytes_AS_STRING(path), mode);
}

/*[clinic input]
promisedio.afchmod
    fd: File
    mode: int
[clinic start generated code]*/

static PyObject *
promisedio_afchmod_impl(PyObject *module, int fd, int mode)
/*[clinic end generated code: output=ffdab78d831112a1 input=82daad821e1d76b1]*/
{
    return (PyObject *) Fs_fchmod(fd, mode);
}

/*[clinic input]
promisedio.autime
    path: Path
    atime: double
    mtime: double,
    *
    follow_symlinks: bool = True
[clinic start generated code]*/

static PyObject *
promisedio_autime_impl(PyObject *module, PyObject *path, double atime,
                       double mtime, int follow_symlinks)
/*[clinic end generated code: output=aa26aa90367251e9 input=4f113111a1a216bf]*/
{
    if (follow_symlinks) {
        return (PyObject *) Fs_utime(PyBytes_AS_STRING(path), atime, mtime);
    } else {
        return (PyObject *) Fs_lutime(PyBytes_AS_STRING(path), atime, mtime);
    }
}

/*[clinic input]
promisedio.afutime
    fd: File
    atime: double
    mtime: double
[clinic start generated code]*/

static PyObject *
promisedio_afutime_impl(PyObject *module, int fd, double atime, double mtime)
/*[clinic end generated code: output=048b09965a628bff input=ebcf238347c3164b]*/
{
    return (PyObject *) Fs_futime(fd, atime, mtime);
}

/*[clinic input]
promisedio.alink
    path: Path
    new_path: Path
[clinic start generated code]*/

static PyObject *
promisedio_alink_impl(PyObject *module, PyObject *path, PyObject *new_path)
/*[clinic end generated code: output=3d951742b5e57c68 input=c0f4d206f37c55bb]*/
{
    return (PyObject *) Fs_link(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path));
}

/*[clinic input]
promisedio.asymlink
    path: Path
    new_path: Path
    *
    flags: int = 0
[clinic start generated code]*/

static PyObject *
promisedio_asymlink_impl(PyObject *module, PyObject *path,
                         PyObject *new_path, int flags)
/*[clinic end generated code: output=ab4477bc665b6b3d input=90e47ceaa5faa259]*/
{
    return (PyObject *) Fs_symlink(PyBytes_AS_STRING(path), PyBytes_AS_STRING(new_path), flags);
}

/*[clinic input]
promisedio.areadlink
    path: Path
[clinic start generated code]*/

static PyObject *
promisedio_areadlink_impl(PyObject *module, PyObject *path)
/*[clinic end generated code: output=ec3a42920771075e input=8318d84b761e40ca]*/
{
    return (PyObject *) Fs_readlink(PyBytes_AS_STRING(path));
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
    goto finally;
    except:
    Py_CLEAR(m);
    finally:
    return m;
}
