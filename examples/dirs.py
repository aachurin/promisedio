from promisedio import exec_async, run, fs


async def example1():
    await fs.mkdir("demo1")
    print (await fs.scandir("."))
    await fs.rmdir("demo1")


async def example2():
    name = await fs.mkdtemp("example.XXXXXX")
    print(name)
    await fs.rmdir(name)


exec_async(example1())
exec_async(example2())

run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount, _printmeminfo
print(_getallocatedobjectscount())
_printmeminfo()
