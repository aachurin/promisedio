import re
from clinic import *
from clinic import main as clinic_main
from capluse import main as capsule_main


_readme_content = []


def get_readme():
    global _readme_content
    if not _readme_content:
        with open("README.md", "rt") as f:
            _readme_content = f.read()
    return _readme_content.splitlines()


def get_readme_description(func):
    readme = get_readme()
    result = []
    for num, text in enumerate(readme):
        if text.startswith(func + "("):
            start_found = False
            for line in readme[num + 1:]:
                if not start_found:
                    if line.startswith("```"):
                        start_found = True
                    continue
                if line.startswith("`") or line.startswith("#"):
                    break
                line = re.sub(r"_[^\s]+_", lambda x: x.group()[1:-1], line)
                result.append(line)
            break
    if not result:
        print("No docstring for %s" % func)
    return "\n".join(result)


docstring_for_c_string = CLanguage.docstring_for_c_string


class DocstringHolder:
    pass


def docstring_for_c_string_from_readme(self, f):
    df = DocstringHolder()
    match = re.search(r"@doc\[.*]", f.docstring)
    if match:
        df.docstring = f.docstring.replace(match.group(), "\n" + get_readme_description(match.group()[5:-1]).strip())
    else:
        name = f"{f.cls.name}.{f.name}" if f.cls else f.name
        df.docstring = f.docstring.rstrip("\n") + "\n\n" + get_readme_description(name).strip()
    return docstring_for_c_string(self, df)


CLanguage.docstring_for_c_string = docstring_for_c_string_from_readme


def rebuild_func(fn, consts):
    code = type(fn.__code__)(fn.__code__.co_argcount,
                             fn.__code__.co_posonlyargcount,
                             fn.__code__.co_kwonlyargcount,
                             fn.__code__.co_nlocals,
                             fn.__code__.co_stacksize,
                             fn.__code__.co_flags,
                             fn.__code__.co_code,
                             consts,
                             fn.__code__.co_names,
                             fn.__code__.co_varnames,
                             fn.__code__.co_filename,
                             fn.__code__.co_name,
                             fn.__code__.co_firstlineno,
                             fn.__code__.co_lnotab,
                             fn.__code__.co_freevars,
                             fn.__code__.co_cellvars
                             )
    new_fn = type(fn)(code, fn.__globals__, fn.__name__, fn.__defaults__, fn.__closure__)
    new_fn.__kwdefaults__ = fn.__kwdefaults__
    return new_fn


def hack_clanguage_output_templates():
    consts = []
    for v in CLanguage.output_templates.__code__.co_consts:
        if isinstance(v, str) and "static {impl_return_type}" in v:
            v = "Py_LOCAL_INLINE({impl_return_type})\n{c_basename}_impl({impl_parameters})\n"
        consts.append(v)
    CLanguage.output_templates = rebuild_func(CLanguage.output_templates, tuple(consts))


hack_clanguage_output_templates()


class Path_converter(CConverter):
    type = "PyObject *"
    converter = "PyUnicode_FSConverter"
    c_default = "NULL"

    def cleanup(self):
        return f"Py_XDECREF({self.name});"


class cstring_converter(CConverter):
    type = "const char *"
    converter = "cstring_converter"
    c_default = "NULL"

    def converter_init(self, *, accept=None):
        if accept == {NoneType}:
            self.converter = "cstring_optional_converter"
        elif accept is not None:
            fail("cstring_converter: illegal 'accept' argument " + repr(accept))


class ssize_t_converter(CConverter):
    type = "Py_ssize_t"
    converter = "ssize_t_converter"


class fd_converter(CConverter):
    type = "int"
    converter = "fd_converter"


class off_t_converter(CConverter):
    type = "Py_off_t"
    converter = "off_t_converter"


class inet_addr_converter(CConverter):
    type = "sockaddr_any"
    converter = "inet_addr_converter"
    impl_by_reference = True


class uid_t_converter(CConverter):
    type = "uid_t"
    converter = "uid_converter"


class gid_t_converter(CConverter):
    type = "gid_t"
    converter = "gid_converter"


if __name__ == "__main__":
    capsule_main(sys.argv[1:])
    clinic_main(["--make", "--srcdir"] + sys.argv[1:])
