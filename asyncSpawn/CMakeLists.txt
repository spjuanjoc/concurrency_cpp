cmake_minimum_required(VERSION 3.0)

project(asyncSpawn)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)

add_executable(${PROJECT_NAME} "main.cpp")
