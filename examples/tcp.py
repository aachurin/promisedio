import ssl
from promisedio import loop, ns, promise, timer


async def example1():
    stream = await ns.open_connection(("87.250.250.242", 443), ssl=ssl.SSLContext())
    print(stream.getsockname())
    print(stream.getpeername())
    await stream.write(b"GET / HTTP 1.1\n\n")
    print(await stream.read())
    await stream.shutdown()


promise.exec_async(example1())
loop.run_forever()
