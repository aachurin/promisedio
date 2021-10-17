import re
import sys

allocated_objects = set()
allocated_requests = set()
allocated_handles = set()

alloc_patterns = (
    r"PyObject_GC_New\([^)]*\)\s*->\s*(.*)",
    r"PyObject_New\([^)]*\)\s*->\s*(.*)",
    r"PyMem_Malloc\([^)]*\)\s*->\s*(.*)",
)

dealloc_patterns = (
    r"PyObject_GC_Del\(([^)]*)\)",
    r"PyObject_Del\(([^)]*)\)",
    r"PyMem_Free\(([^)]*)\)",
)

for line in sys.stdin:
    line = line.strip()
    print(line)
    if not line:
        continue
    match = [re.search(pat, line).group(1) for pat in alloc_patterns if re.search(pat, line)]
    if match:
        allocated_objects.add(match[0])
        continue
    match = re.search(r"Request_New\([^)]*\)\s*->\s*(.*)", line)
    if match:
        allocated_requests.add(match.group(1))
        continue
    match = re.search(r"Handle_New\([^)]*\)\s*->\s*(.*)", line)
    if match:
        allocated_handles.add(match.group(1))
        continue
    match = [re.search(pat, line).group(1) for pat in dealloc_patterns if re.search(pat, line)]
    if match:
        try:
            allocated_objects.remove(match[0])
        except KeyError:
            print("--- Invalid free:", match[0])
        continue
    match = re.search(r"Request_Close\(([^)]*)\)", line)
    if match:
        try:
            allocated_requests.remove(match.group(1))
        except KeyError:
            print("--- Invalid request close:", match.group(1))
        continue
    match = re.search(r"Handle_Close\(([^)]*)\)", line)
    if match:
        try:
            allocated_handles.remove(match.group(1))
        except KeyError:
            print("--- Invalid handle close:", match.group(1))
        continue
    if "@ALLOC_STATS" in line:
        print("Allocated objects:", list(allocated_objects))
        print("Open requests:", list(allocated_requests))
        print("Open handles:", list(allocated_handles))


print("@FINAL_ALLOC_STATS:")
print("Allocated objects:", list(allocated_objects))
print("Open requests:", list(allocated_requests))
print("Open handles:", list(allocated_handles))
