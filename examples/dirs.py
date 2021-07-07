from promisedio import _getallocatedobjectscount
from promisedio import exec_async, run, fs


async def example1():
    await fs.mkdir("demo1")
    print (await fs.scandir("."))
    await fs.rmdir("demo1")


async def example2():
    name = await fs.mkdtemp("example.XXXXXX")
    print(name)
    await fs.rmdir(name)


print("ALLOCS", _getallocatedobjectscount())

exec_async(example1())

print("ALLOCS", _getallocatedobjectscount())

exec_async(example2())

run()

print("ALLOCS", _getallocatedobjectscount())
