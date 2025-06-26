cmake_minimum_required(VERSION 3.19)

project(async_pool
  VERSION "0.0.4"
  DESCRIPTION "Thread pool pattern examples in C++"
  LANGUAGES CXX)

# Options
option(ENABLE_WERROR  "Treat compiler warnings as errors" False)
option(ENABLE_TESTS   "Enable tests" False)


add_executable(${PROJECT_NAME}
  src/main.cpp
)

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


# Tests
if (ENABLE_TESTS)
  message("Tests enabled")
  enable_testing() # Must be placed before add_subdirectory
  add_subdirectory(test)
endif ()
