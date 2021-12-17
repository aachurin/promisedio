from promisedio import promise, loop


async def example1():
    lock = promise.Lock()
    await lock.acquire()
    await lock.acquire()


promise.exec_async(example1())
loop.run_until_complete()
