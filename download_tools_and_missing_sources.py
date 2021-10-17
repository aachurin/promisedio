import os
import re
import sysconfig
import requests
import tempfile
import subprocess
import hashlib
import json


base = os.path.dirname(__file__)
hash_store = {}


def get_content(ver, path):
    print(f"Download {ver}: {path}")
    url = f"https://raw.githubusercontent.com/python/cpython/{ver}/{path}"
    return requests.get(url).content


def remove_comments(code):
    new_code = []
    comment_block = False
    empty_line = False
    for line in code.splitlines():
        stripped_line = line.strip()
        if stripped_line.startswith(b"//"):
            continue
        if stripped_line.startswith(b"/*"):
            comment_block = True
        if comment_block:
            if stripped_line.endswith(b"*/"):
                comment_block = False
            continue
        if empty_line and not stripped_line:
            continue
        empty_line = not stripped_line
        new_code.append(line)
    return b"\n".join(new_code)


def ensure_dict(d):
    if not isinstance(d, dict):
        return {}
    return d


def get_source_code(ver, from_, to_, base=None, c_names=None, exclude_c_names=None,
                    wrap_guards=True, stub_template=None):
    code = get_content(ver, from_)
    if c_names or exclude_c_names:
        with tempfile.NamedTemporaryFile(suffix=".c") as f:
            f.write(code)
            options = [
                f"-I{sysconfig.get_config_var('INCLUDEDIR')}",
                f"-I{sysconfig.get_config_var('INCLUDEPY')}"
            ]
            if c_names:
                options += [f"-D{d['ifdef']}=1" for d in c_names.values() if ensure_dict(d).get("ifdef")]
            ret = subprocess.run(
                ["clang", "-Xclang", "-ast-dump=json", *options, f.name],
                capture_output=True
            )
            dat = json.loads(ret.stdout)
        if c_names:
            code_pieces = []
            for node in dat["inner"]:
                if node["kind"].endswith("Decl") and node.get("name") in c_names:
                    ifdef = ensure_dict(c_names[node["name"]]).get("ifdef")
                    only_source_hash = ensure_dict(c_names[node["name"]]).get("only_source_hash")
                    start = node["range"]["begin"]["offset"]
                    end = node["range"]["end"]["offset"]
                    if node["kind"] != "TypedefDecl":
                        end += 1
                    piece = code[start: end]
                    if node["kind"] == "TypedefDecl":
                        piece += node["name"].encode("utf-8")
                    if node["kind"] != "FunctionDecl":
                        piece += b";"
                    if ifdef:
                        piece = b"#ifdef " + ifdef.encode("utf-8") + b"\n" + piece + b"\n#endif"
                    if only_source_hash:
                        piece_hash = hashlib.md5(piece).hexdigest()[:8].encode("utf-8")
                        piece = b"\n".join(b"// %s" % line for line in piece.splitlines())
                        piece = b"#define %s_hash 0x%s\n%s" % (node["name"].encode("utf-8"), piece_hash, piece)
                    code_pieces.append(piece)
            code = b"\n\n".join(code_pieces)
        else:
            code = bytearray(code)
            blocks = []
            for node in dat["inner"]:
                if node["kind"].endswith("Decl") and node.get("name") in exclude_c_names:
                    start = node["range"]["begin"]["offset"]
                    end = node["range"]["end"]["offset"]
                    if node["kind"] == "FunctionDecl":
                        end += 1
                    blocks.append((start, end))
            blocks.reverse()
            for start, end in blocks:
                code[start: end] = b"//"

    code = remove_comments(code)
    code_hash = hashlib.md5(code).hexdigest()
    filename = os.path.join(base, to_) if base else to_
    dirname = os.path.dirname(filename)
    if dirname:
        os.makedirs(dirname, exist_ok=True)
    if code_hash not in hash_store:
        if wrap_guards:
            guard = to_.replace("/", "_").replace(".", "_").upper().encode("utf-8")
            code = b"#ifndef %s\n#define %s\n\n%s\n\n#endif" % (guard, guard, code)
        with open(filename, "wt") as f:
            f.write(code.decode("utf-8"))
        hash_store[code_hash] = to_
    else:
        template = stub_template or '#include "{to_}"'
        with open(filename, "wt") as f:
            f.write(template.format(to_=hash_store[code_hash]))


def download():
    # base branch
    last_tag = "v3.10.0"

    # tools
    get_source_code(last_tag, "Tools/clinic/cpp.py", "tools/cpp.py")
    get_source_code(last_tag, "Tools/clinic/clinic.py", "tools/clinic.py")

    # sources
    tags = [
        last_tag
    ]
    for tag in sorted(tags):
        ver = ".".join(re.search(r"v(\d+).(\d+).(\d+)", tag).groups())
        get_source_code(
            tag,
            "Modules/socketmodule.h",
            f"pysource/{ver}_socketmodule.h",
            base="src",
            exclude_c_names=[
                "sock_addr_t",
                "PySocketSockObject",
                "PySocketModule_APIObject"
            ]
        )
        get_source_code(
            tag,
            "Modules/socketmodule.c",
            f"pysource/{ver}_socketmodule.c",
            base="src",
            c_names={
                "makebdaddr": {
                    "ifdef": "USE_BLUETOOTH"
                },
                "setbdaddr": {
                    "ifdef": "USE_BLUETOOTH"
                },
                "makesockaddr": True,
                "sock_decode_hostname": True
            }
        )
        get_source_code(
            tag,
            "Modules/_ssl.c",
            f"pysource/{ver}_ssl.h",
            base="src",
            c_names={
                "PySSLContext": True,
                "py_ssl_error": True,
                "py_ssl_server_or_client": True,
                "py_ssl_version": True,
                "py_proto_version": True,
                "newPySSLSocket": {
                    "only_source_hash": True
                },
                "PySSLSocket": True,
                "PySSLMemoryBIO": True
            }
        )
        get_source_code(
            tag,
            "Objects/capsule.c",
            f"pysource/{ver}_capsule.h",
            base="src",
            c_names={
                "PyCapsule": True
            }
        )

if __name__ == "__main__":
    download()
