cmake_minimum_required(VERSION 3.0)

project(asyncVector)

aux_source_directory(. SRC_LIST)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} ${SRC_LIST})
