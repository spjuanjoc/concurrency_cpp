cmake_minimum_required(VERSION 2.8)

project(async3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)

add_executable(${PROJECT_NAME} "main.cpp")
