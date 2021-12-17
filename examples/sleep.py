import time
import sys
from promisedio import loop, timer, promise


print("started", time.time())


async def example1(timeout):
    print("example1", time.time())
    def f1(_):
        print("f1", time.time())
        return timer.sleep(1)

    await timer.sleep(timeout).then(f1)


for x in range(10):
    print(sys.getrefcount(None))
    promise.exec_async(example1(1))
    loop.run_forever()
    print(sys.getrefcount(None))
