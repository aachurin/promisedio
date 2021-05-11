from promisedio import asleep, exec_async, run


async def example1(timeout, value):
    await asleep(timeout)
    print("example", value, "done")


exec_async(example1(4, 1))
exec_async(example1(3, 2))

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _memdebug
# print(_getallocatedobjectscount())
# _memdebug()
