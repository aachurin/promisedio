from promisedio import exec_async, run, ns


async def example1():
    print(await ns.getaddrinfo("магазин.картины24.рф", 8080))


async def example2():
    print(41)
    print(await ns.getnameinfo(("91.218.229.12", "8080"), 0))
    print(42)


exec_async(example1())
exec_async(example2())

run()

# uncomment to see debug info
from promisedio import _getallocatedobjectscount, _printmeminfo
print(_getallocatedobjectscount())
_printmeminfo()
