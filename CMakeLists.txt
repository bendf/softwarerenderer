cmake_minimum_required(VERSION 3.10.2)
project(software_renderer)

set(CMAKE_BUILD_TYPE Debug)
include_directories(src/include)
add_executable(test_run src/test_main.cpp src/buffer2d_test.cpp src/buffer2d.cpp src/targa.cpp src/targa_test.cpp src/model.cpp src/model_test.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()
