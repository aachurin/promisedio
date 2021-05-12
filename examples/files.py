from promisedio import exec_async, run, aopen, aunlink, amkstemp, aread


async def readwrite():
    try:
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
    finally:
        await aunlink("demo1.txt")


async def readwrite_temp():
    fd, name = await amkstemp("example1.XXXXXX")
    try:
        print(fd, name)
        f = await aopen(fd)
        try:
            await f.write(b"Some text2")
            await f.seek(0)
            print(await f.read())
        finally:
            await f.close()
    finally:
        await aunlink(name)


async def read_stdin():
    print(await aread(0))  # Send EOF to stop reading (Ctrl+D)


exec_async(readwrite())
exec_async(readwrite_temp())
exec_async(read_stdin())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _printmeminfo
# print(_getallocatedobjectscount())
# _printmeminfo()
