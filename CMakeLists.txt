cmake_minimum_required(VERSION 3.10.2)
project(software_renderer)

set(CMAKE_BUILD_TYPE Debug)
include_directories(src/include)
add_executable(buffer2d_test src/test_main.cpp src/buffer2d_test.cpp src/buffer2d.cpp)
add_executable(targa_test src/test_main.cpp src/targa.cpp src/targa_test.cpp)
add_executable(model_test src/test_main.cpp src/model.cpp src/model_test.cpp)
add_executable(raster_test src/test_main.cpp src/raster.cpp src/raster_test.cpp)
add_executable(scratch src/scratch.cpp)
add_executable(rvalue src/rvalue.cpp)
add_executable(integrate_test src/test_main.cpp src/integrate_test.cpp src/buffer2d.cpp src/model.cpp src/raster.cpp src/targa.cpp)
#add_executable(model_test src/test_main.cpp src/model.cpp src/model_test.cpp)
#add_executable(render_test src/test_main.cpp src/render.cpp src/render_test.cpp)
#add_executable(raster_test src/test_main.cpp src/raster.cpp src/raster_test.cpp src/targa.cpp)
#add_executable(test_run src/test_main.cpp src/buffer2d_test.cpp src/buffer2d.cpp src/targa.cpp src/targa_test.cpp src/model.cpp src/model_test.cpp src/render.cpp )

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()
