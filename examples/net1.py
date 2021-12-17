from promisedio import ns, loop, promise


async def example1():
    print(await ns.getaddrinfo("магазин.картины24.рф", 8080))
    # print(await ns.getaddrinfo("fe80::b8e6:5b24:9357:9e1c%utun3", "https"))
    print(await ns.getnameinfo(("91.218.229.12", 8080), 0))


async def example2():
    print(await ns.getnameinfo(("91.218.229.12", 8080), 0))


promise.exec_async(example1())
promise.exec_async(example2())

loop.run_forever()
