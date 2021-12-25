import ssl
from promisedio import loop, ns, promise, timer

n = 0

def new_connection(stream):
    global n
    print("We are here!!!!")
    print(stream)
    if n == 3:
        server.close()
    n += 1


server = ns.start_server(new_connection, ("127.0.0.1", 8090))


loop.run_forever()
