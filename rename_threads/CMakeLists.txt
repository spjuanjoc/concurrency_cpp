cmake_minimum_required(VERSION 3.19)

project(rename_threads
  VERSION "0.0.3"
  DESCRIPTION "Put names to threads"
  LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

# Options
option(ENABLE_WERROR  "Treat compiler warnings as errors" False)
option(ENABLE_TESTS   "Enable tests" False)


## Compiler options
include(../cmake/CompilerOptions.cmake)
set_compiler_options(${PROJECT_NAME})


## Find third-party packages
find_package(fmt REQUIRED)


## Target
target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE # These directories must be placed before other non-system directories
    ${fmt_INCLUDE_DIRS}
  PUBLIC
    include
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    pthread
    ${fmt_LIBRARIES}
)
