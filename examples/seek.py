from promisedio import exec_async, run, aseek


async def seek_example():
    print(await aseek(0, 0, 1))


exec_async(seek_example())

run()

# uncomment to see debug info
# from promisedio import _getallocatedobjectscount, _printmeminfo
# print(_getallocatedobjectscount())
# _printmeminfo()
