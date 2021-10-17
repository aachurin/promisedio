# this file is only for clion (not for building)

cmake_minimum_required(VERSION 3.19)
project(promisedio C)

add_compile_options(-Werror)
set(CMAKE_C_STANDARD 11)

#message(STATUS "${PYTHON_INCLUDE_DIRS}")

include_directories(/Library/Frameworks/Python.framework/Versions/3.10/include/python3.10)
include_directories(/usr/local/Cellar/libuv/1.41.0/include)
include_directories(/usr/local/opt/openssl@1.1/include)
include_directories(src)

add_library(promisedio.error SHARED
            src/error/error.c)

add_library(promisedio.promise SHARED
            src/promise/promise.c)

add_library(promisedio.loop SHARED
            src/loop/loop.c)

add_library(promisedio.timer SHARED
            src/timer/timer.c)

add_library(promisedio.signal SHARED
            src/signal/signal.c)

add_library(promisedio.fs SHARED
            src/fs/fs.c)

add_library(promisedio.ns SHARED
            src/ns/ns.c)
