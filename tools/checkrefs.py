import re
import sys


class Ref:
    def __init__(self, type):
        self.type = type
        self.refcnt = 1

    def __str__(self):
        return f"{self.type}({self.refcnt})"


def print_refs():
    for addr, ref in refs.items():
        print(f"{addr}\t{ref}")


def print_allocs():
    for addr in allocs:
        if addr in refs:
            print(f"{addr}\t{refs[addr]}")
        else:
            print(f"{addr}\tRAW")


def print_header(title):
    length = 40
    if not title:
        print("=" * length)
        return
    rest = length - len(title) - 2
    left_pad = rest // 2
    right_pad = rest - left_pad
    print("=" * left_pad, title, "=" * right_pad)


def print_stats():
    print_header("Refs")
    print_refs()
    print_header("Allocs")
    print_allocs()


def print_leaks():
    if refs:
        print_header("Leaks report (Refs)")
        print_refs()
    if allocs:
        print_header("Leaks report (Allocs)")
        print_allocs()
    if not (refs or allocs):
        print_header("Great! No leaks")

refs = {}
allocs = []

for line in sys.stdin:
    line = line.strip()
    print(line)
    if not line:
        continue
    if line == "#ALLOCSTAT":
        print_stats()
        continue
    if not line.startswith("#"):
        continue
    match = re.match(r"#(\w+)\s+\((.*)\)", line.split("--", 1)[0].strip())
    if not match:
        continue
    action, args = match.groups()
    args = [x.strip() for x in args.split(",")]
    if action == "MALLOC":
        allocs.append(args[0])
    elif action == "FREE":
        allocs.remove(args[0])
    elif action == "ALLOC":
        if args[0] in refs:
            print_header("Already allocated")
            print(action)
            print(args[0])
            print(refs)
            print_header("")
        else:
            refs[args[0]] = Ref(args[1])
    elif action == "DELETE":
        if args[0] not in refs:
            print_header("Missing object")
            print(action)
            print(args[0])
            print(refs)
            print_header("")
        else:
            del refs[args[0]]
    elif action in ("INCREF", "ENTER", "RESIZD"):
        if args[0] in refs:
            refs[args[0]].refcnt += 1
        else:
            refs[args[0]] = Ref(args[1])
    elif action in ("DECREF", "RESIZE"):
        if args[0] in refs:
            refs[args[0]].refcnt -= 1
            if refs[args[0]].refcnt == 0:
                del refs[args[0]]
        else:
            print_header("Missing object")
            print(action)
            print(args[0])
            print(refs)
            print_header("")


print_leaks()
