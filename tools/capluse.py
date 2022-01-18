import os
import re
import sys
import hashlib
import argparse

code_header = "// Auto-generated\n\n"

def main(params):
    parser = argparse.ArgumentParser()
    parser.add_argument("root")
    args = parser.parse_args(params)
    modules = [name for name in os.listdir(args.root) if os.path.isdir(os.path.join(args.root, name))]
    if "capsule" in modules:
        modules.remove("capsule")
    else:
        os.mkdir(os.path.join(args.root, "capsule"))
    for module in modules:
        module_path = os.path.join(args.root, module)
        files = [
            name
            for name in os.listdir(module_path)
            if name.endswith(".c") and os.path.isfile(os.path.join(module_path, name))
        ]
        generate_auto_files(args.root, module, files)


def error(msg, key, decl):
    print(key)
    print(decl)
    raise ValueError(msg)


def parse_c_file(data):
    result = {}
    functions = re.findall(r"CAPSULE_API\((.*),\s*(.*)\)([^{;]*)", data)
    for key, ret, decl in functions:
        key = key.strip()
        if not re.match(r"^[a-zA-Z_][a-zA-Z0-9_]+$", key):
            error(key, decl, "Invalid key")
        ret = ret.strip()
        decl = decl.strip()
        match = re.match(r"(.*)\(([\s\S]*)\)", decl)
        if not match:
            error(key, decl, "Invalid declaration")
        funcname, funcargs = match.groups()
        funcname = funcname.strip()
        funcargs = [x.strip() for x in funcargs.strip().split(",")]
        result.setdefault(key.lower(), []).append({
            "name": funcname,
            "ret": ret,
            "args": funcargs
        })
    return result


def generate_auto_files(root, module, files):
    functions = {}
    for file in files:
        with open(os.path.join(root, module, file), "rt") as f:
            data = f.read()
        result = parse_c_file(data)
        if result:
            for key, value in result.items():
                functions.setdefault(key, []).extend(value)

    if not functions:
        return

    hash_keys = {}
    for api_key, funcs in functions.items():
        hash_keys[api_key] = hashlib.md5(repr(funcs).encode("utf-8")).hexdigest()

    with open(os.path.join(root, "capsule", module + ".h"), "wt") as f1:
        with open(os.path.join(root, module, "capsule.h"), "wt") as f2:
            f1.write(code_header)
            f1.write(f"#ifndef PROMISEDIO_{module.upper()}_API\n")
            f1.write(f"#define PROMISEDIO_{module.upper()}_API\n\n")
            if os.path.exists(os.path.join(root, module, f"{module}.h")):
                f1.write(f'#include "{module}/{module}.h"\n\n')
            f2.write(code_header)
            for api_key, funcs in functions.items():
                hash_key = api_key + "_" + hash_keys[api_key]
                f1.write(f"static int {hash_key}__api_loaded = 0;\n")
                f1.write(f"static void *{hash_key}__api[{len(funcs)}];\n\n")
                f1.write(f"#define {api_key.upper()} {hash_key}\n\n")
                f2.write(f"#define {api_key.upper()} {hash_key}\n\n")
                f2.write(f"#define {api_key.upper()}_CAPSULE {{\\\n")
                for index, func in enumerate(funcs):
                    ret = func["ret"]
                    name = func["name"]
                    args = list(func["args"])
                    func_id = name.upper() + "_ID"
                    f1.write(f"#define {func_id} {index}\n")
                    f2.write(f"  [{index}] = {name},\\\n")
                    has_state = "_ctx_var" in args
                    if has_state:
                        args.remove("_ctx_var")
                    has_args = bool(args)
                    if has_state:
                        args.insert(0, "void*")
                    if has_args:
                        f1.write(f"#define {name}(...) \\\n")
                    else:
                        f1.write(f"#define {name}() \\\n")
                    varargs = []
                    if has_state:
                        varargs.append(f"_ctx->{hash_key}__ctx")
                    if has_args:
                        varargs.append("__VA_ARGS__")
                    args = ", ".join(args)
                    varargs = ", ".join(varargs)
                    # f1.write(f"  (*({ret} (*) ({args}))(_ctx->{hash_key}__api[{func_id}]))( \\\n")
                    f1.write(f"  (({ret} (*) ({args}))({hash_key}__api[{func_id}]))( \\\n")
                    f1.write(f"    {varargs})\n\n")
                f2.write("}\n\n")
            f1.write("#endif\n")


if __name__ == "__main__":
    main(sys.argv[1:])
