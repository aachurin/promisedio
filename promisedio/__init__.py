import os
from . _cext import (
    _getallocatedobjectscount,
    _memdebug,
    process_promise_chain,
    deferred,
    exec_async,
    sleep,
    run_loop,
    close_loop,
    use_python_default_sigint,
    check_can_fork,
    _inspectloop,
    astat,
    afstat,
    aopenfd,
    aclose,
    aread,
    awrite,
    aopen,
    aseek
    )

os.register_at_fork(before=check_can_fork)
