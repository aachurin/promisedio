import time
from promisedio import exec_async, run, timer


print("started", time.time())


async def example1(timeout):
    def test1():
        print("should not be here")

    def test2():
        print("here", time.time())

    def test3():
        print("interval", time.time())

    # timer.set_timeout(test1, timeout, unref=True)
    timer.set_timeout(test2, timeout / 2.)
    # timer.set_interval(test3, 1., unref=True)
    print("done", time.time())

exec_async(example1(5))
run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount, _printmeminfo
print(_getallocatedobjectscount())
_printmeminfo()
