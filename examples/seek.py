from promisedio import loop, fs, promise


async def seek_example():
    print(await fs.seek(0, 0, 1))


promise.exec_async(seek_example())
loop.run_until_complete()
