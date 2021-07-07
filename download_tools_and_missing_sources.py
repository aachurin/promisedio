import os
import distutils.sysconfig
import requests
import tempfile
import subprocess
import json


version = "3.9"
base = os.path.dirname(__file__)


class Func:
    def __init__(self, name, ifdef=None):
        self.name = name
        self.ifdef = ifdef


def get_content(path, binary=False):
    url = f"https://raw.githubusercontent.com/python/cpython/{version}/{path}"
    return requests.get(url).content


def get_source_code(from_, to_, c_names=None):
    code = get_content(from_)
    if c_names:
        with tempfile.NamedTemporaryFile(suffix=".c") as f:
            f.write(code)
            include = [
                f"-I{distutils.sysconfig.get_config_var('INCLUDEDIR')}",
                f"-I{distutils.sysconfig.get_config_var('INCLUDEPY')}"
            ]
            defines = [f"-D{d['ifdef']}=1" for d in c_names.values() if d.get("ifdef")]
            ret = subprocess.run(
                ["clang", "-Xclang", "-ast-dump=json", *include, *defines, f.name],
                capture_output=True
            )
            dat = json.loads(ret.stdout)
        code_pieces = []
        for node in dat["inner"]:
            if node["kind"].endswith("Decl") and node.get("name") in c_names:
                ifdef = c_names[node["name"]].get("ifdef")
                piece = code[node["range"]["begin"]["offset"]: node["range"]["end"]["offset"] + 1]
                if node["kind"] != "FunctionDecl":
                    piece += b";"
                if ifdef:
                    piece = b"#ifdef " + ifdef.encode("utf-8") + b"\n" + piece + b"\n#endif"
                code_pieces.append(piece)

        code = b"\n\n".join(code_pieces)

    with open(os.path.join(base, to_), "wt") as f:
        f.write(code.decode("utf-8"))


def download():
    # tools
    get_source_code("Tools/clinic/cpp.py", "tools/cpp.py")
    get_source_code("Tools/clinic/clinic.py", "tools/clinic.py")
    # sources
    get_source_code("Modules/socketmodule.h", "src/python/socketmodule.h")
    get_source_code(
        "Modules/socketmodule.c", "src/python/socketmodule.c",
        c_names={   # I don't want to reimplement this functions )))
            "makebdaddr": {
                "ifdef": "USE_BLUETOOTH"
            },
            "setbdaddr": {
                "ifdef": "USE_BLUETOOTH"
            },
            "makesockaddr": {},
            "sock_decode_hostname": {}
        }
    )


if __name__ == "__main__":
    download()
