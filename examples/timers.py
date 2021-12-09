import time
import signal
from promisedio import timer, loop, promise

def here(_, __):
    print("HERE!")

signal.signal(signal.SIGUSR1, here)
print("start", time.time())
timer.sleep(10).then(lambda x: print("DONE"))

async def example1():
    print("example1 start", time.time())
    def test1():
        assert 0, "Should not be here"

    def test2():
        print("test2", time.time())

    def test3():
        print("@ALLOC_STATS")
        print("test3", time.time())

    timer.set_timeout(test1, 11, unref=True)
    timer.set_timeout(test2, 5)
    timer.set_interval(test3, 1., unref=True)
    print("example1 end", time.time())


promise.exec_async(example1())
loop.run_until_complete()
print("end", time.time())
