import time
from promisedio import exec_async, run, timer


print("started", time.time())


async def example1(timeout):
    print("example1", time.time())

    def f1(_):
        print("f1", time.time())
        return timer.sleep(3)

    def f2(_):
        print("f2", time.time())

    timer.sleep(timeout * 2).then(f2)

    await timer.sleep(timeout).then(f1)
    print("done", time.time())


exec_async(example1(5))

run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount
print(_getallocatedobjectscount())
