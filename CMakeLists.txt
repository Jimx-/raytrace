cmake_minimum_required(VERSION 3.8)
project(raytrace)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -msse4.1")

set(CMAKE_CXX_STANDARD 11)

include_directories(${raytrace_SOURCE_DIR}/jsoncpp/include)

set(SOURCE_FILES main.cpp frame_buffer.cpp frame_buffer.h scene.cpp scene.h object.cpp object.h vector3.cpp light.cpp light.h sphere.cpp sphere.h material.cpp material.h camera.cpp camera.h texture.cpp texture.h const.h triangle.cpp triangle.h config_reader.cpp jsoncpp/dist/jsoncpp.cpp libbmpread/bmpread.c)
add_executable(raytrace ${SOURCE_FILES})
