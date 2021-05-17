import os
from . _cext import (
    _getallocatedobjectscount,
    _printmeminfo,
    process_promise_chain,
    deferred,
    exec_async,
    run_loop,
    close_loop,
    use_python_default_sigint,
    check_can_fork,
    _inspectloop,
    asleep,
    astat,
    afstat,
    aopenfd,
    aclose,
    aread,
    awrite,
    aopen,
    aseek,
    aunlink,
    amkdir,
    armdir,
    amkdtemp,
    amkstemp,
    ascandir,
    arename,
    afsync,
    aftruncate,
    acopyfile,
    asendfile,
    aaccess,
    achmod,
    afchmod,
    autime,
    afutime,
    alink,
    asymlink,
    areadlink,
    COPYFILE_EXCL,
    COPYFILE_FICLONE,
    COPYFILE_FICLONE_FORCE,
    DIRENT_UNKNOWN,
    DIRENT_FILE,
    DIRENT_DIR,
    DIRENT_LINK,
    DIRENT_FIFO,
    DIRENT_SOCKET,
    DIRENT_CHAR,
    DIRENT_BLOCK,
    F_OK,
    R_OK,
    W_OK,
    X_OK,
    SYMLINK_DIR,
    SYMLINK_JUNCTION
)


def run():
    try:
        run_loop()
    finally:
        close_loop()


os.register_at_fork(before=check_can_fork)
