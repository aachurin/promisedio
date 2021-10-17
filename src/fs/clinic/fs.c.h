/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(fsmodule_stat__doc__,
"stat($module, /, path, *, follow_symlinks=True)\n"
"--\n"
"\n"
"Get the status of a file. Return a StatObj object.\n"
"\n"
"This function normally follows symlinks; to stat a symlink add the argument follow_symlinks=False\n"
"\n"
"Equivalent to [stat(2)](https://man7.org/linux/man-pages/man2/stat.2.html) \n"
"              [lstat(2)](https://man7.org/linux/man-pages/man2/lstat.2.html).");

#define FSMODULE_STAT_METHODDEF    \
    {"stat", (PyCFunction)(void(*)(void))fsmodule_stat, METH_FASTCALL|METH_KEYWORDS, fsmodule_stat__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_stat_impl(PyObject *module, PyObject *path, int follow_symlinks);

static PyObject *
fsmodule_stat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "stat", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
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
    return_value = fsmodule_stat_impl(module, path, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_fstat__doc__,
"fstat($module, /, fd)\n"
"--\n"
"\n"
"Get the status of the file descriptor fd. \n"
"\n"
"Return StatObj object.\n"
"\n"
"Equivalent to [fstat(2)](https://man7.org/linux/man-pages/man2/fstat.2.html).");

#define FSMODULE_FSTAT_METHODDEF    \
    {"fstat", (PyCFunction)(void(*)(void))fsmodule_fstat, METH_FASTCALL|METH_KEYWORDS, fsmodule_fstat__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_fstat_impl(PyObject *module, int fd);

static PyObject *
fsmodule_fstat(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fstat", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = fsmodule_fstat_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_seek__doc__,
"seek($module, /, fd, pos, how)\n"
"--\n"
"\n"
"Set the current position of file descriptor fd to position pos, modified by how:\n"
"- SEEK_SET or 0 to set the position relative to the beginning of the file;\n"
"- SEEK_CUR or 1 to set it relative to the current position;\n"
"- SEEK_END or 2 to set it relative to the end of the file.\n"
"\n"
"Return the new cursor position in bytes, starting from the beginning.\n"
"\n"
"Equivalent to [lseek(2)](https://man7.org/linux/man-pages/man2/lseek.2.html).");

#define FSMODULE_SEEK_METHODDEF    \
    {"seek", (PyCFunction)(void(*)(void))fsmodule_seek, METH_FASTCALL|METH_KEYWORDS, fsmodule_seek__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_seek_impl(PyObject *module, int fd, Py_off_t pos, int how);

static PyObject *
fsmodule_seek(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "pos", "how", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "seek", 0};
    PyObject *argsbuf[3];
    int fd;
    Py_off_t pos;
    int how;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!off_t_converter(args[1], &pos)) {
        goto exit;
    }
    how = _PyLong_AsInt(args[2]);
    if (how == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = fsmodule_seek_impl(module, fd, pos, how);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_openfd__doc__,
"openfd($module, /, path, flags, mode=438)\n"
"--\n"
"\n"
"Open the file path and set various flags according to flags and possibly its mode according to mode.\n"
"\n"
"Return the file descriptor for the newly opened file. The new file descriptor is non-inheritable.\n"
"\n"
"Equivalent to [open(2)](https://man7.org/linux/man-pages/man2/open.2.html).");

#define FSMODULE_OPENFD_METHODDEF    \
    {"openfd", (PyCFunction)(void(*)(void))fsmodule_openfd, METH_FASTCALL|METH_KEYWORDS, fsmodule_openfd__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_openfd_impl(PyObject *module, PyObject *path, int flags, int mode);

static PyObject *
fsmodule_openfd(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "flags", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "openfd", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    int flags;
    int mode = 438;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    flags = _PyLong_AsInt(args[1]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    mode = _PyLong_AsInt(args[2]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_openfd_impl(module, path, flags, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_open__doc__,
"open($module, /, path, mode=\'r\', closefd=True)\n"
"--\n"
"\n"
"Open file and return a corresponding file object. If the file cannot be opened, an OSError is raised. \n"
"\n"
"Equivalent to python [open](https://docs.python.org/3/library/functions.html#open) (binary mode only).");

#define FSMODULE_OPEN_METHODDEF    \
    {"open", (PyCFunction)(void(*)(void))fsmodule_open, METH_FASTCALL|METH_KEYWORDS, fsmodule_open__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_open_impl(PyObject *module, PyObject *path, const char *mode,
                   int closefd);

static PyObject *
fsmodule_open(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", "closefd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "open", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path;
    const char *mode = "r";
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
        if (!cstring_converter(args[1], &mode)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    closefd = _PyLong_AsInt(args[2]);
    if (closefd == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_open_impl(module, path, mode, closefd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_read__doc__,
"read($module, /, fd, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read from file descriptor fd until we have size characters or we hit EOF.\n"
"\n"
"If offset is present and is not negative, read at most size bytes from file descriptor fd at a position of offset,\n"
"leaving the file offset unchanged.\n"
"\n"
"Equivalent to [read(2)](https://man7.org/linux/man-pages/man2/read.2.html)\n"
"              [preadv(2)](https://man7.org/linux/man-pages/man2/preadv.2.html)");

#define FSMODULE_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))fsmodule_read, METH_FASTCALL|METH_KEYWORDS, fsmodule_read__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_read_impl(PyObject *module, int fd, Py_ssize_t size,
                   Py_off_t offset);

static PyObject *
fsmodule_read(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    int fd;
    Py_ssize_t size = -1;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        if (!ssize_t_converter(args[1], &size)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_read_impl(module, fd, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_write__doc__,
"write($module, /, fd, data, offset=-1)\n"
"--\n"
"\n"
"Write the data to file descriptor fd.\n"
"\n"
"If offset is present and is not negative, write the data to file descriptor fd at position of offset, \n"
"leaving the file offset unchanged.\n"
"\n"
"Return the number of bytes actually written.\n"
"\n"
"Equivalent to [write(2)](https://man7.org/linux/man-pages/man2/write.2.html)\n"
"              [pwritev(2)](https://man7.org/linux/man-pages/man2/pwritev.2.html)");

#define FSMODULE_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))fsmodule_write, METH_FASTCALL|METH_KEYWORDS, fsmodule_write__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_write_impl(PyObject *module, int fd, PyObject *data,
                    Py_off_t offset);

static PyObject *
fsmodule_write(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    int fd;
    PyObject *data;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    data = args[1];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_write_impl(module, fd, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_close__doc__,
"close($module, /, fd)\n"
"--\n"
"\n"
"Close file descriptor fd.\n"
"\n"
"Equivalent to [close(2)](https://man7.org/linux/man-pages/man2/close.2.html).");

#define FSMODULE_CLOSE_METHODDEF    \
    {"close", (PyCFunction)(void(*)(void))fsmodule_close, METH_FASTCALL|METH_KEYWORDS, fsmodule_close__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_close_impl(PyObject *module, int fd);

static PyObject *
fsmodule_close(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "close", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = fsmodule_close_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_closefd__doc__,
"closefd($module, /, fd)\n"
"--\n"
"\n"
"");

#define FSMODULE_CLOSEFD_METHODDEF    \
    {"closefd", (PyCFunction)(void(*)(void))fsmodule_closefd, METH_FASTCALL|METH_KEYWORDS, fsmodule_closefd__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_closefd_impl(PyObject *module, int fd);

static PyObject *
fsmodule_closefd(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "closefd", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = fsmodule_closefd_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_unlink__doc__,
"unlink($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the file path.\n"
"\n"
"Equivalent to [unlink(2)](https://man7.org/linux/man-pages/man2/unlink.2.html).");

#define FSMODULE_UNLINK_METHODDEF    \
    {"unlink", (PyCFunction)(void(*)(void))fsmodule_unlink, METH_FASTCALL|METH_KEYWORDS, fsmodule_unlink__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_unlink_impl(PyObject *module, PyObject *path);

static PyObject *
fsmodule_unlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "unlink", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = fsmodule_unlink_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_mkdir__doc__,
"mkdir($module, /, path, mode=511)\n"
"--\n"
"\n"
"Create a directory named path with numeric mode mode.\n"
"\n"
"If the directory already exists, FileExistsError is raised.\n"
"\n"
"On some systems, mode is ignored.\n"
"\n"
"Equivalent to [mkdir(2)](https://man7.org/linux/man-pages/man2/mkdir.2.html).");

#define FSMODULE_MKDIR_METHODDEF    \
    {"mkdir", (PyCFunction)(void(*)(void))fsmodule_mkdir, METH_FASTCALL|METH_KEYWORDS, fsmodule_mkdir__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkdir_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
fsmodule_mkdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkdir", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *path = NULL;
    int mode = 511;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_mkdir_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_rmdir__doc__,
"rmdir($module, /, path)\n"
"--\n"
"\n"
"Remove (delete) the directory path. \n"
"\n"
"If the directory does not exist or is not empty, an FileNotFoundError or an OSError is raised respectively.\n"
"\n"
"Equivalent to [rmdir(2)](https://man7.org/linux/man-pages/man2/rmdir.2.html).");

#define FSMODULE_RMDIR_METHODDEF    \
    {"rmdir", (PyCFunction)(void(*)(void))fsmodule_rmdir, METH_FASTCALL|METH_KEYWORDS, fsmodule_rmdir__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_rmdir_impl(PyObject *module, PyObject *path);

static PyObject *
fsmodule_rmdir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "rmdir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = fsmodule_rmdir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_mkdtemp__doc__,
"mkdtemp($module, /, tpl)\n"
"--\n"
"\n"
"Generate a uniquely named temporary directory from template tpl.\n"
"\n"
"The last six characters of template must be XXXXXX and these are replaced with a string that makes\n"
"the directory name unique.\n"
"\n"
"Return the modified template string.\n"
"\n"
"Equivalent to [mkdtemp(3)](https://man7.org/linux/man-pages/man3/mkdtemp.3.html).");

#define FSMODULE_MKDTEMP_METHODDEF    \
    {"mkdtemp", (PyCFunction)(void(*)(void))fsmodule_mkdtemp, METH_FASTCALL|METH_KEYWORDS, fsmodule_mkdtemp__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkdtemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
fsmodule_mkdtemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkdtemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &tpl)) {
        goto exit;
    }
    return_value = fsmodule_mkdtemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(fsmodule_mkstemp__doc__,
"mkstemp($module, /, tpl)\n"
"--\n"
"\n"
"Generate a unique temporary filename from template.\n"
"\n"
"This function creates and opens the file, and returns an open file descriptor for the file.\n"
"The last six characters of template must be \"XXXXXX\" and these are replaced with a string that makes \n"
"the filename unique.\n"
"\n"
"Return the file descriptor and modified template string.\n"
"\n"
"Equivalent to [mkstemp(3)](https://man7.org/linux/man-pages/man3/mkstemp.3.html).");

#define FSMODULE_MKSTEMP_METHODDEF    \
    {"mkstemp", (PyCFunction)(void(*)(void))fsmodule_mkstemp, METH_FASTCALL|METH_KEYWORDS, fsmodule_mkstemp__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_mkstemp_impl(PyObject *module, PyObject *tpl);

static PyObject *
fsmodule_mkstemp(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"tpl", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mkstemp", 0};
    PyObject *argsbuf[1];
    PyObject *tpl = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &tpl)) {
        goto exit;
    }
    return_value = fsmodule_mkstemp_impl(module, tpl);

exit:
    /* Cleanup for tpl */
    Py_XDECREF(tpl);

    return return_value;
}

PyDoc_STRVAR(fsmodule_scandir__doc__,
"scandir($module, /, path)\n"
"--\n"
"\n"
"Return a sequence of the entries in the directory given by path (entry_type, entry_name).  \n"
"\n"
"Special entries \'.\' and \'..\' are not included.\n"
"\n"
"Equivalent to [scandir(3)](https://man7.org/linux/man-pages/man3/scandir.3.html).");

#define FSMODULE_SCANDIR_METHODDEF    \
    {"scandir", (PyCFunction)(void(*)(void))fsmodule_scandir, METH_FASTCALL|METH_KEYWORDS, fsmodule_scandir__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_scandir_impl(PyObject *module, PyObject *path);

static PyObject *
fsmodule_scandir(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "scandir", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = fsmodule_scandir_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_rename__doc__,
"rename($module, /, path, new_path)\n"
"--\n"
"\n"
"Rename the file or directory path to new_path.\n"
"\n"
"Equivalent to [rename(2)](https://man7.org/linux/man-pages/man2/rename.2.html).");

#define FSMODULE_RENAME_METHODDEF    \
    {"rename", (PyCFunction)(void(*)(void))fsmodule_rename, METH_FASTCALL|METH_KEYWORDS, fsmodule_rename__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_rename_impl(PyObject *module, PyObject *path, PyObject *new_path);

static PyObject *
fsmodule_rename(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "rename", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    PyObject *new_path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    return_value = fsmodule_rename_impl(module, path, new_path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_fsync__doc__,
"fsync($module, /, fd)\n"
"--\n"
"\n"
"Force write of file with file descriptor fd to disk.\n"
"\n"
"Equivalent to [fsync(2)](https://man7.org/linux/man-pages/man2/fsync.2.html).");

#define FSMODULE_FSYNC_METHODDEF    \
    {"fsync", (PyCFunction)(void(*)(void))fsmodule_fsync, METH_FASTCALL|METH_KEYWORDS, fsmodule_fsync__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_fsync_impl(PyObject *module, int fd);

static PyObject *
fsmodule_fsync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fsync", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = fsmodule_fsync_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_ftruncate__doc__,
"ftruncate($module, /, fd, length)\n"
"--\n"
"\n"
"Truncate the file corresponding to file descriptor fd, so that it is at most length bytes in size.\n"
"\n"
"Equivalent to [ftruncate(2)](https://man7.org/linux/man-pages/man2/ftruncate.2.html).");

#define FSMODULE_FTRUNCATE_METHODDEF    \
    {"ftruncate", (PyCFunction)(void(*)(void))fsmodule_ftruncate, METH_FASTCALL|METH_KEYWORDS, fsmodule_ftruncate__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_ftruncate_impl(PyObject *module, int fd, Py_ssize_t length);

static PyObject *
fsmodule_ftruncate(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "length", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "ftruncate", 0};
    PyObject *argsbuf[2];
    int fd;
    Py_ssize_t length;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!ssize_t_converter(args[1], &length)) {
        goto exit;
    }
    return_value = fsmodule_ftruncate_impl(module, fd, length);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_fdatasync__doc__,
"fdatasync($module, /, fd)\n"
"--\n"
"\n"
"Force write of file with file descriptor fd to disk. Does not force update of metadata.\n"
"\n"
"Equivalent to [fdatasync(2)](https://man7.org/linux/man-pages/man2/fdatasync.2.html).");

#define FSMODULE_FDATASYNC_METHODDEF    \
    {"fdatasync", (PyCFunction)(void(*)(void))fsmodule_fdatasync, METH_FASTCALL|METH_KEYWORDS, fsmodule_fdatasync__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_fdatasync_impl(PyObject *module, int fd);

static PyObject *
fsmodule_fdatasync(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fdatasync", 0};
    PyObject *argsbuf[1];
    int fd;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    return_value = fsmodule_fdatasync_impl(module, fd);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_copyfile__doc__,
"copyfile($module, /, path, new_path, flags=0)\n"
"--\n"
"\n"
"Copy a file from path to new_path. \n"
"\n"
"Supported flags are: \n"
"- COPYFILE_EXCL\n"
"- COPYFILE_FICLONE\n"
"- COPYFILE_FICLONE_FORCE\n"
"\n"
"For more information, see [uvfs_copyfile](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fscopyfile).");

#define FSMODULE_COPYFILE_METHODDEF    \
    {"copyfile", (PyCFunction)(void(*)(void))fsmodule_copyfile, METH_FASTCALL|METH_KEYWORDS, fsmodule_copyfile__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_copyfile_impl(PyObject *module, PyObject *path, PyObject *new_path,
                       int flags);

static PyObject *
fsmodule_copyfile(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "copyfile", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    PyObject *new_path = NULL;
    int flags = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    flags = _PyLong_AsInt(args[2]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_copyfile_impl(module, path, new_path, flags);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_sendfile__doc__,
"sendfile($module, /, out_fd, in_fd, offset, count)\n"
"--\n"
"\n"
"Copy count bytes from file descriptor in_fd to file descriptor out_fd starting at offset.\n"
"\n"
"Return the number of bytes sent.\n"
"\n"
"Equivalent to [sendfile(2)](https://man7.org/linux/man-pages/man2/sendfile.2.html).");

#define FSMODULE_SENDFILE_METHODDEF    \
    {"sendfile", (PyCFunction)(void(*)(void))fsmodule_sendfile, METH_FASTCALL|METH_KEYWORDS, fsmodule_sendfile__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_sendfile_impl(PyObject *module, int out_fd, int in_fd,
                       Py_off_t offset, Py_ssize_t count);

static PyObject *
fsmodule_sendfile(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"out_fd", "in_fd", "offset", "count", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "sendfile", 0};
    PyObject *argsbuf[4];
    int out_fd;
    int in_fd;
    Py_off_t offset;
    Py_ssize_t count;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 4, 4, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &out_fd)) {
        goto exit;
    }
    if (!fd_converter(args[1], &in_fd)) {
        goto exit;
    }
    if (!off_t_converter(args[2], &offset)) {
        goto exit;
    }
    if (!ssize_t_converter(args[3], &count)) {
        goto exit;
    }
    return_value = fsmodule_sendfile_impl(module, out_fd, in_fd, offset, count);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_access__doc__,
"access($module, /, path, mode)\n"
"--\n"
"\n"
"Use the real uid/gid to test for access to path.\n"
"\n"
"Mode should be F_OK to test the existence of path, or it can be the inclusive OR of one or more of \n"
"R_OK, W_OK, and X_OK to test permissions. \n"
"\n"
"Return True if access is allowed, False if not.\n"
"\n"
"Equivalent to [access(2)](https://man7.org/linux/man-pages/man2/access.2.html).");

#define FSMODULE_ACCESS_METHODDEF    \
    {"access", (PyCFunction)(void(*)(void))fsmodule_access, METH_FASTCALL|METH_KEYWORDS, fsmodule_access__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_access_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
fsmodule_access(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "access", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = fsmodule_access_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_chmod__doc__,
"chmod($module, /, path, mode)\n"
"--\n"
"\n"
"Change the mode of path to the numeric mode.\n"
"\n"
"See [stat module](https://docs.python.org/3/library/stat.html#stat.S_ISUID) for available mode.\n"
"\n"
"Equivalent to [chmod(2)](https://man7.org/linux/man-pages/man2/chmod.2.html).");

#define FSMODULE_CHMOD_METHODDEF    \
    {"chmod", (PyCFunction)(void(*)(void))fsmodule_chmod, METH_FASTCALL|METH_KEYWORDS, fsmodule_chmod__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_chmod_impl(PyObject *module, PyObject *path, int mode);

static PyObject *
fsmodule_chmod(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "chmod", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = fsmodule_chmod_impl(module, path, mode);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_fchmod__doc__,
"fchmod($module, /, fd, mode)\n"
"--\n"
"\n"
"Change the mode of the file given by fd to the numeric mode.\n"
"\n"
"See [stat module](https://docs.python.org/3/library/stat.html#stat.S_ISUID) for available mode.\n"
"\n"
"Equivalent to [fchmod(2)](https://man7.org/linux/man-pages/man2/fchmod.2.html).");

#define FSMODULE_FCHMOD_METHODDEF    \
    {"fchmod", (PyCFunction)(void(*)(void))fsmodule_fchmod, METH_FASTCALL|METH_KEYWORDS, fsmodule_fchmod__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_fchmod_impl(PyObject *module, int fd, int mode);

static PyObject *
fsmodule_fchmod(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "mode", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fchmod", 0};
    PyObject *argsbuf[2];
    int fd;
    int mode;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    mode = _PyLong_AsInt(args[1]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = fsmodule_fchmod_impl(module, fd, mode);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_utime__doc__,
"utime($module, /, path, atime, mtime, *, follow_symlinks=True)\n"
"--\n"
"\n"
"Set the access and modified times of the file specified by path.\n"
"\n"
"This function normally follows symlinks.\n"
"\n"
"Equivalent to [utime(2)](https://man7.org/linux/man-pages/man2/utime.2.html)\n"
"              [lutimes(2)](https://man7.org/linux/man-pages/man3/lutimes.3.html).");

#define FSMODULE_UTIME_METHODDEF    \
    {"utime", (PyCFunction)(void(*)(void))fsmodule_utime, METH_FASTCALL|METH_KEYWORDS, fsmodule_utime__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_utime_impl(PyObject *module, PyObject *path, double atime,
                    double mtime, int follow_symlinks);

static PyObject *
fsmodule_utime(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "atime", "mtime", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "utime", 0};
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 3;
    PyObject *path = NULL;
    double atime;
    double mtime;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (PyFloat_CheckExact(args[1])) {
        atime = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        atime = PyFloat_AsDouble(args[1]);
        if (atime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (PyFloat_CheckExact(args[2])) {
        mtime = PyFloat_AS_DOUBLE(args[2]);
    }
    else
    {
        mtime = PyFloat_AsDouble(args[2]);
        if (mtime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    follow_symlinks = PyObject_IsTrue(args[3]);
    if (follow_symlinks < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = fsmodule_utime_impl(module, path, atime, mtime, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_futime__doc__,
"futime($module, /, fd, atime, mtime)\n"
"--\n"
"\n"
"Set the access and modified times of the file given by fd.\n"
"\n"
"Equivalent to [futimes(3)](https://man7.org/linux/man-pages/man3/futimes.3.html)");

#define FSMODULE_FUTIME_METHODDEF    \
    {"futime", (PyCFunction)(void(*)(void))fsmodule_futime, METH_FASTCALL|METH_KEYWORDS, fsmodule_futime__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_futime_impl(PyObject *module, int fd, double atime, double mtime);

static PyObject *
fsmodule_futime(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "atime", "mtime", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "futime", 0};
    PyObject *argsbuf[3];
    int fd;
    double atime;
    double mtime;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (PyFloat_CheckExact(args[1])) {
        atime = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        atime = PyFloat_AsDouble(args[1]);
        if (atime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    if (PyFloat_CheckExact(args[2])) {
        mtime = PyFloat_AS_DOUBLE(args[2]);
    }
    else
    {
        mtime = PyFloat_AsDouble(args[2]);
        if (mtime == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
    return_value = fsmodule_futime_impl(module, fd, atime, mtime);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_link__doc__,
"link($module, /, path, new_path)\n"
"--\n"
"\n"
"Create a hard link pointing to path named new_path.\n"
"\n"
"Equivalent to [link(2)](https://man7.org/linux/man-pages/man2/link.2.html)");

#define FSMODULE_LINK_METHODDEF    \
    {"link", (PyCFunction)(void(*)(void))fsmodule_link, METH_FASTCALL|METH_KEYWORDS, fsmodule_link__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_link_impl(PyObject *module, PyObject *path, PyObject *new_path);

static PyObject *
fsmodule_link(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "link", 0};
    PyObject *argsbuf[2];
    PyObject *path = NULL;
    PyObject *new_path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    return_value = fsmodule_link_impl(module, path, new_path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_symlink__doc__,
"symlink($module, /, path, new_path, *, flags=0)\n"
"--\n"
"\n"
"Create a symbolic link pointing to path named new_path.\n"
"\n"
"On Windows the flags parameter can be specified to control how the symlink will be created:\n"
" - SYMLINK_DIR: indicates that path points to a directory.\n"
" - SYMLINK_JUNCTION: request that the symlink is created using junction points.\n"
"\n"
"Equivalent to [symlink(2)](https://man7.org/linux/man-pages/man2/symlink.2.html)");

#define FSMODULE_SYMLINK_METHODDEF    \
    {"symlink", (PyCFunction)(void(*)(void))fsmodule_symlink, METH_FASTCALL|METH_KEYWORDS, fsmodule_symlink__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_symlink_impl(PyObject *module, PyObject *path, PyObject *new_path,
                      int flags);

static PyObject *
fsmodule_symlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "new_path", "flags", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "symlink", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 2;
    PyObject *path = NULL;
    PyObject *new_path = NULL;
    int flags = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 2, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[1], &new_path)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    flags = _PyLong_AsInt(args[2]);
    if (flags == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = fsmodule_symlink_impl(module, path, new_path, flags);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);
    /* Cleanup for new_path */
    Py_XDECREF(new_path);

    return return_value;
}

PyDoc_STRVAR(fsmodule_readlink__doc__,
"readlink($module, /, path)\n"
"--\n"
"\n"
"Return a string representing the path to which the symbolic link points.\n"
"\n"
"Equivalent to [readlink(2)](https://man7.org/linux/man-pages/man2/readlink.2.html)");

#define FSMODULE_READLINK_METHODDEF    \
    {"readlink", (PyCFunction)(void(*)(void))fsmodule_readlink, METH_FASTCALL|METH_KEYWORDS, fsmodule_readlink__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_readlink_impl(PyObject *module, PyObject *path);

static PyObject *
fsmodule_readlink(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "readlink", 0};
    PyObject *argsbuf[1];
    PyObject *path = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    return_value = fsmodule_readlink_impl(module, path);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

#if !defined(MS_WINDOWS)

PyDoc_STRVAR(fsmodule_chown__doc__,
"chown($module, /, path, uid, gid, *, follow_symlinks=True)\n"
"--\n"
"\n"
"");

#define FSMODULE_CHOWN_METHODDEF    \
    {"chown", (PyCFunction)(void(*)(void))fsmodule_chown, METH_FASTCALL|METH_KEYWORDS, fsmodule_chown__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_chown_impl(PyObject *module, PyObject *path, uid_t uid, gid_t gid,
                    int follow_symlinks);

static PyObject *
fsmodule_chown(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"path", "uid", "gid", "follow_symlinks", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "chown", 0};
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 3;
    PyObject *path = NULL;
    uid_t uid;
    gid_t gid;
    int follow_symlinks = 1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!PyUnicode_FSConverter(args[0], &path)) {
        goto exit;
    }
    if (!uid_converter(args[1], &uid)) {
        goto exit;
    }
    if (!gid_converter(args[2], &gid)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_kwonly;
    }
    follow_symlinks = PyObject_IsTrue(args[3]);
    if (follow_symlinks < 0) {
        goto exit;
    }
skip_optional_kwonly:
    return_value = fsmodule_chown_impl(module, path, uid, gid, follow_symlinks);

exit:
    /* Cleanup for path */
    Py_XDECREF(path);

    return return_value;
}

#endif /* !defined(MS_WINDOWS) */

#if !defined(MS_WINDOWS)

PyDoc_STRVAR(fsmodule_fchown__doc__,
"fchown($module, /, fd, uid, gid)\n"
"--\n"
"\n"
"");

#define FSMODULE_FCHOWN_METHODDEF    \
    {"fchown", (PyCFunction)(void(*)(void))fsmodule_fchown, METH_FASTCALL|METH_KEYWORDS, fsmodule_fchown__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_fchown_impl(PyObject *module, int fd, uid_t uid, gid_t gid);

static PyObject *
fsmodule_fchown(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"fd", "uid", "gid", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "fchown", 0};
    PyObject *argsbuf[3];
    int fd;
    uid_t uid;
    gid_t gid;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!fd_converter(args[0], &fd)) {
        goto exit;
    }
    if (!uid_converter(args[1], &uid)) {
        goto exit;
    }
    if (!gid_converter(args[2], &gid)) {
        goto exit;
    }
    return_value = fsmodule_fchown_impl(module, fd, uid, gid);

exit:
    return return_value;
}

#endif /* !defined(MS_WINDOWS) */

PyDoc_STRVAR(fsmodule_FileIO_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Close this handle.\n"
"\n"
"This method has no effect if the file is already closed. Once the file is closed,\n"
"any operation on the file (e.g. reading or writing) will raise a ValueError.\n"
"As a convenience, it is allowed to call this method more than once; only the first\n"
"call, however, will have an effect.\n"
"\n"
"");

#define FSMODULE_FILEIO_CLOSE_METHODDEF    \
    {"close", (PyCFunction)fsmodule_FileIO_close, METH_NOARGS, fsmodule_FileIO_close__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_close_impl(FileIO *self);

static PyObject *
fsmodule_FileIO_close(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return fsmodule_FileIO_close_impl(self);
}

PyDoc_STRVAR(fsmodule_FileIO_fileno__doc__,
"fileno($self, /)\n"
"--\n"
"\n"
"Return the underlying file descriptor (an integer).\n"
"\n"
"");

#define FSMODULE_FILEIO_FILENO_METHODDEF    \
    {"fileno", (PyCFunction)fsmodule_FileIO_fileno, METH_NOARGS, fsmodule_FileIO_fileno__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_fileno_impl(FileIO *self);

static PyObject *
fsmodule_FileIO_fileno(FileIO *self, PyObject *Py_UNUSED(ignored))
{
    return fsmodule_FileIO_fileno_impl(self);
}

PyDoc_STRVAR(fsmodule_FileIO_read__doc__,
"read($self, /, size=-1, offset=-1)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.\n"
"\n"
"");

#define FSMODULE_FILEIO_READ_METHODDEF    \
    {"read", (PyCFunction)(void(*)(void))fsmodule_FileIO_read, METH_FASTCALL|METH_KEYWORDS, fsmodule_FileIO_read__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_read_impl(FileIO *self, Py_ssize_t size, Py_off_t offset);

static PyObject *
fsmodule_FileIO_read(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"size", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "read", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    Py_ssize_t size = -1;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        if (!ssize_t_converter(args[0], &size)) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (!off_t_converter(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_FileIO_read_impl(self, size, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_FileIO_write__doc__,
"write($self, /, data, offset=-1)\n"
"--\n"
"\n"
"Write the given bytes to the IO handle.\n"
"\n"
"Returns the number of bytes written, which is always the length of data in bytes.\n"
"\n"
"");

#define FSMODULE_FILEIO_WRITE_METHODDEF    \
    {"write", (PyCFunction)(void(*)(void))fsmodule_FileIO_write, METH_FASTCALL|METH_KEYWORDS, fsmodule_FileIO_write__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_write_impl(FileIO *self, PyObject *data, Py_off_t offset);

static PyObject *
fsmodule_FileIO_write(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"data", "offset", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "write", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *data;
    Py_off_t offset = -1;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    data = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (!off_t_converter(args[1], &offset)) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_FileIO_write_impl(self, data, offset);

exit:
    return return_value;
}

PyDoc_STRVAR(fsmodule_FileIO_seek__doc__,
"seek($self, /, offset, whence=0)\n"
"--\n"
"\n"
"Read at most size bytes, returned as bytes.\n"
"\n"
"");

#define FSMODULE_FILEIO_SEEK_METHODDEF    \
    {"seek", (PyCFunction)(void(*)(void))fsmodule_FileIO_seek, METH_FASTCALL|METH_KEYWORDS, fsmodule_FileIO_seek__doc__},

Py_LOCAL_INLINE(PyObject *)
fsmodule_FileIO_seek_impl(FileIO *self, Py_off_t offset, int whence);

static PyObject *
fsmodule_FileIO_seek(FileIO *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"offset", "whence", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "seek", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    Py_off_t offset;
    int whence = 0;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!off_t_converter(args[0], &offset)) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    whence = _PyLong_AsInt(args[1]);
    if (whence == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional_pos:
    return_value = fsmodule_FileIO_seek_impl(self, offset, whence);

exit:
    return return_value;
}

#ifndef FSMODULE_CHOWN_METHODDEF
    #define FSMODULE_CHOWN_METHODDEF
#endif /* !defined(FSMODULE_CHOWN_METHODDEF) */

#ifndef FSMODULE_FCHOWN_METHODDEF
    #define FSMODULE_FCHOWN_METHODDEF
#endif /* !defined(FSMODULE_FCHOWN_METHODDEF) */
/*[clinic end generated code: output=568ea0d33a70c90c input=a9049054013a1b77]*/
