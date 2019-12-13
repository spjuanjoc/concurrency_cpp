cmake_minimum_required(VERSION 3.0)

project(threads2)

aux_source_directory(. SRC_LIST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(${PROJECT_NAME} ${SRC_LIST})
