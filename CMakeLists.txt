cmake_minimum_required(VERSION 3.10.2)
project(software_renderer)

include_directories(src/include)
add_executable(targa_test src/test_main.cpp src/targa_test.cpp)
