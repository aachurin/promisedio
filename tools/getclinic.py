import sys

import os
import subprocess


def download(folder, version):
    cpp_py = os.path.join(folder, "cpp.py")
    clinic_py = os.path.join(folder, "clinic.py")
    subprocess.run([
        "curl", f"https://raw.githubusercontent.com/python/cpython/{version}/Tools/clinic/cpp.py", "-o", cpp_py
    ], check=True)
    subprocess.run([
        "curl", f"https://raw.githubusercontent.com/python/cpython/{version}/Tools/clinic/clinic.py", "-o", clinic_py],
        check=True
    )


if __name__ == "__main__":
    download(os.path.dirname(__file__), *sys.argv[1:])
