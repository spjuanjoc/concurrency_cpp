cmake_minimum_required(VERSION 3.19)

project(spawn_async_colored
  VERSION "0.0.4"
  DESCRIPTION "Example of running spawned async tasks with colored output"
  LANGUAGES CXX
)


# Options
option(ENABLE_WERROR  "Treat compiler warnings as errors" False)


add_executable(${PROJECT_NAME} main.cpp)


## Compiler options
include(../cmake/CompilerOptions.cmake)
set_compiler_options(${PROJECT_NAME})


## Find third-party packages
find_package(fmt REQUIRED)


## Target
target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE # These directories must be placed before other non-system directories
    ${fmt_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    pthread
    ${fmt_LIBRARIES}
)

