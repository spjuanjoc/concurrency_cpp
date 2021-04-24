cmake_minimum_required(VERSION 3.19)
project(parallel_algorithms)

add_executable(${PROJECT_NAME} main.cpp)

set(PROJECT_OPTIONS
    -O3
    -Wpedantic
    -Wall
    -Wextra
    -Werror
    )

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})
find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME} pthread fmt::fmt)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} INTERFACE ${PROJECT_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
