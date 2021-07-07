from promisedio import exec_async, run, fs


async def seek_example():
    print(await fs.seek(0, 0, 1))


exec_async(seek_example())

run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount
print(_getallocatedobjectscount())
