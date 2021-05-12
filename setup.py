import os
from setuptools import setup
from setuptools.extension import Extension


if os.environ.get("BUILD_WITH_DEBUG"):
    define_macros = [("DEBUG_OUTPUT", "stderr")]
    undef_macros = ["NDEBUG"]
else:
    define_macros = []
    undef_macros = []

extensions = [
    Extension(
        "promisedio._cext",
        define_macros=define_macros,
        undef_macros=undef_macros,
        sources=[
            "src/memory.c",
            "src/promise.c",
            "src/loop.c",
            "src/timer.c",
            "src/fs.c",
            "src/fileio.c",
            "src/module.c",
        ],
        libraries=["uv"],
        extra_compile_args=["-O2"]
    )
]

setup(
    name="promisedio",
    author="Andrey Churin",
    author_email="aachurin@google.com",
    description="High-performance asynchronous promise-based IO.",
    url="https://github.com/aachurin/promisedio",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    ext_modules=extensions,
    version="1.0",
    packages=["promisedio"],
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: Implementation :: CPython",
        "License :: OSI Approved :: MIT License",
        ],
    python_requires=">=3.8"
    )
