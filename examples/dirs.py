from promisedio import exec_async, run, amkdir, armdir


async def readwrite():
    await amkdir("demo1")
    await armdir("demo1")

exec_async(readwrite())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _memdebug
# print(_getallocatedobjectscount())
# _memdebug()
