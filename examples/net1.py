import socket
from promisedio import exec_async, run, _ns


async def example1():
    print(await _ns.getaddrinfo("магазин.картины24.рф", 8080))
    # print(await ns.getaddrinfo("fe80::b8e6:5b24:9357:9e1c%utun3", "https"))
    print(await _ns.getnameinfo(("91.218.229.12", 8080), 0))


async def example2():
    print(await _ns.getnameinfo(("91.218.229.12", 8080), 0))


exec_async(example1())
exec_async(example2())

run()

from promisedio import _getallocatedobjectscount
print("@ALLOC_STATS:", _getallocatedobjectscount())
