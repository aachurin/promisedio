from promisedio import exec_async, run, aopen, aunlink


async def readwrite():
    f = await aopen("demo1.txt", "wb")
    await f.write(b"Some text1")
    await f.close()
    f = await aopen("demo1.txt", "rb")
    print(await f.read())
    await f.close()
    await aunlink("demo1.txt")

exec_async(readwrite())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _memdebug
# print(_getallocatedobjectscount())
# _memdebug()
