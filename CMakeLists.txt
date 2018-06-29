cmake_minimum_required(VERSION 3.10.2)
project(software_renderer)

set(CMAKE_BUILD_TYPE Debug)
include_directories(src/include)
add_executable(targa_test src/test_main.cpp src/targa_test.cpp src/targa.cpp src/model.cpp)
