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
    ascandir
    )

os.register_at_fork(before=check_can_fork)

DIRENT_UNKNOWN = 0
DIRENT_FILE = 1
DIRENT_DIR = 2
DIRENT_LINK = 3
DIRENT_FIFO = 4
DIRENT_SOCKET = 5
DIRENT_CHAR = 6
DIRENT_BLOCK = 7

def run():
    try:
        run_loop()
    finally:
        close_loop()
