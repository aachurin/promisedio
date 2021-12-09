from promisedio import loop, fs, promise


async def example1():
    f = await fs.open("demo1.txt", "wb")
    try:
        await f.write(b"Some text1")
    finally:
        await f.close()
    f = await fs.open("demo1.txt", "rb")
    try:
        print(await f.read())
    finally:
        await f.close()
    await fs.rename("demo1.txt", "demo2.txt")
    await fs.unlink("demo2.txt")


async def example2():
    fd, name = await fs.mkstemp("example1.XXXXXX")
    try:
        print(fd, name)
        f = await fs.open(fd)
        try:
            await f.write(b"Some text2")
            await f.seek(0)
            print(await f.read())
        finally:
            await f.close()
    finally:
        await fs.unlink(name)


async def example3():
    f = await fs.mkdir("demo3")
    try:
        await fs.open("demo3")
    except OSError as e:
        print(type(e), e.errno, e.strerror)
    finally:
        await fs.rmdir("demo3")


promise.exec_async(example1())
promise.exec_async(example2())
promise.exec_async(example3())

loop.run_until_complete()
print("@ALLOC_STATS")
