cmake_minimum_required(VERSION 3.19)
project(promisedio C)
find_package(PythonInterp 3.9 REQUIRED)
find_package(PythonLibs 3.9 REQUIRED)

set(CMAKE_C_STANDARD 11)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(/usr/local/Cellar/libuv/1.41.0/include)
include_directories(src)

add_library(_io SHARED
        src/memory.c
        src/promise.c
        src/loop.c
        src/timer.c
        src/fs.c
        src/fileio.c
        src/module.c)
