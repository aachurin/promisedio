from promisedio import loop, fs, promise


async def example1():
    await fs.mkdir("demo1")
    print(await fs.scandir("."))
    await fs.rmdir("demo1")


async def example2():
    name = await fs.mkdtemp("example.XXXXXX")
    print(name)
    await fs.rmdir(name)


promise.exec_async(example1())
promise.exec_async(example2())
loop.run_until_complete()
