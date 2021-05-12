import time
from promisedio import asleep, exec_async, run


print("started", time.time())


async def example1(timeout):
    print("example1", time.time())

    def f1(_):
        print("f1", time.time())
        return asleep(3)

    def f2(_):
        print("f2", time.time())

    asleep(timeout * 2).then(f2)

    await asleep(timeout).then(f1)
    print("done", time.time())


exec_async(example1(5))

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _printmeminfo
# print(_getallocatedobjectscount())
# _printmeminfo()
