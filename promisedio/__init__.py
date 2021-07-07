import os
from . _cext import (
    _getallocatedobjectscount,
    process_promise_chain,
    deferred,
    exec_async,
    run_loop,
    close_loop,
    use_python_default_sigint,
    check_can_fork,
    _inspectloop
)


def run():
    try:
        run_loop()
    finally:
        close_loop()


os.register_at_fork(before=check_can_fork)
