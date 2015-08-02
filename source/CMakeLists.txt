cmake_minimum_required(VERSION 3.2)

project(so.version)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(SOURCE_FILES
    version.cpp
    version_parser.cpp
)

add_library(so.version STATIC ${SOURCE_FILES})
include_directories(../include)
