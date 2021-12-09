import os
import sysconfig
from setuptools import setup
from setuptools.extension import Extension

define_macros = {}
undef_macros = []
env = os.environ.get

if env("BUILD_DEBUG_LOG"):
    define_macros["BUILD_DEBUG_LOG"] = "1"
    undef_macros += ["NDEBUG"]

if env("BUILD_DEBUG_MEM"):
    define_macros["BUILD_DEBUG_MEM"] = "1"
    undef_macros += ["NDEBUG"]

if env("BUILD_DISABLE_FREELISTS"):
    define_macros["BUILD_DISABLE_FREELISTS"] = "1"

define_macros["Py_BUILD_CORE"] = "1"
define_macros = list(define_macros.items())
py_libs = sysconfig.get_config_var("LIBDIR")

common_ext_params = {
    "define_macros": define_macros,
    "undef_macros": undef_macros,
    "include_dirs": ["src"],
    "extra_compile_args": ["-O2"]
}

extensions = [
    Extension(
        "promisedio.error",
        sources=["src/error/error.c"],
        libraries=["uv"],
        **common_ext_params
    ),
    Extension(
        "promisedio.promise",
        sources=["src/promise/promise.c"],
        **common_ext_params
    ),
    Extension(
        "promisedio.loop",
        sources=["src/loop/loop.c"],
        libraries=["uv"],
        **common_ext_params
    ),
    Extension(
        "promisedio.timer",
        sources=["src/timer/timer.c"],
        libraries=["uv"],
        **common_ext_params
    ),
    Extension(
        "promisedio.signal",
        sources=["src/signal/signal.c"],
        **common_ext_params
    ),
    Extension(
        "promisedio.fs",
        sources=["src/fs/fs.c"],
        libraries=["uv"],
        **common_ext_params
    ),
    Extension(
        "promisedio.ns",
        sources=["src/ns/ns.c"],
        library_dirs=[py_libs],
        libraries=["uv"],
        **common_ext_params
    ),
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
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: Implementation :: CPython",
        "License :: OSI Approved :: MIT License",
        ],
    python_requires=">=3.10"
    )
