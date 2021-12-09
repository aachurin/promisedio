import time
from promisedio import loop, timer, promise


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


promise.exec_async(example1(5))
loop.run_until_complete()
