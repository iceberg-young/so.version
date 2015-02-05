cmake_minimum_required(VERSION 2.8.4)

project(version)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES example.cpp)

add_subdirectory(source)
add_executable(version ${SOURCE_FILES})
add_dependencies(version so.version)
target_link_libraries(version so.version)
