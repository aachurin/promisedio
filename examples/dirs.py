from promisedio import exec_async, run, amkdir, armdir, amkdtemp, ascandir


async def mkrmdir():
    await amkdir("demo1")
    print (await ascandir("."))
    await armdir("demo1")


async def mkdir_temp():
    name = await amkdtemp("example.XXXXXX")
    print(name)
    await armdir(name)


exec_async(mkrmdir())
exec_async(mkdir_temp())

run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount, _printmeminfo
print(_getallocatedobjectscount())
_printmeminfo()
