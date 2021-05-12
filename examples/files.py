from promisedio import exec_async, run, aopen, aunlink, amkstemp


async def readwrite():
    f = await aopen("demo1.txt", "wb")
    try:
        await f.write(b"Some text1")
    finally:
        await f.close()
    f = await aopen("demo1.txt", "rb")
    try:
        print(await f.read())
    finally:
        await f.close()
    await aunlink("demo1.txt")


async def readwrite_temp():
    fd = await amkstemp("example1.XXXXXX")
    f = await aopen(fd)
    try:
        await f.write(b"Some text2")
        await f.seek(0)
        print(await f.read())
    finally:
        await f.close()


exec_async(readwrite())
exec_async(readwrite_temp())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _memdebug
# print(_getallocatedobjectscount())
# _memdebug()
