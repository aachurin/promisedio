from promisedio import exec_async, run, aopen, aunlink, amkstemp, aread, arename, amkdir, armdir


async def example1():
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
    await arename("demo1.txt", "demo2.txt")
    await aunlink("demo2.txt")


async def example2():
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


async def example3():
    f = await amkdir("demo3")
    try:
        await aopen("demo3")
    finally:
        await armdir("demo3")


exec_async(example1())
exec_async(example2())
exec_async(example3())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _printmeminfo
# print(_getallocatedobjectscount())
# _printmeminfo()
