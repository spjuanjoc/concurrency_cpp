cmake_minimum_required(VERSION 2.8)

project(threadsSpawn)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
add_executable(${PROJECT_NAME} "main.cpp")
