cmake_minimum_required(VERSION 3.10.2)
project(software_renderer)

set(CMAKE_BUILD_TYPE Debug)
include_directories(src/include)
add_executable(test_run src/test_main.cpp src/buffer2d_test.cpp src/buffer2d.cpp src/targa.cpp src/targa_test.cpp)
