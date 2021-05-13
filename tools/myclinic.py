import re
from clinic import *

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
            for line in readme[num + 2:]:
                if line.startswith("`") or line.startswith("#"):
                    break
                line = re.sub(r"_[^\s]+_", lambda x: x.group()[1:-1], line)
                result.append(line)
            break
    return "\n".join(result)


docstring_for_c_string = CLanguage.docstring_for_c_string


class DocstringHolder:
    pass


def docstring_for_c_string_from_readme(self, f):
    df = DocstringHolder()
    df.docstring = f.docstring.rstrip("\n") + "\n\n" + get_readme_description(f.name).strip()
    return docstring_for_c_string(self, df)


CLanguage.docstring_for_c_string = docstring_for_c_string_from_readme


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
