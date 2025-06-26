cmake_minimum_required(VERSION 3.15)

project(thread_signal
  VERSION "0.0.2"
  DESCRIPTION "Signals with sigc++"
  LANGUAGES CXX
)

set(SOURCE_FILES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# Options
option(ENABLE_WERROR  "Treat compiler warnings as errors" False)
option(ENABLE_TESTS   "Enable tests" False)



## Compiler options
include(../cmake/CompilerOptions.cmake)
set_compiler_options(${PROJECT_NAME})


## Find third-party packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})
find_package(fmt REQUIRED)
find_package(sigc++-3 REQUIRED)


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
    ${sigc++-3_LIBRARIES}
)

#target_compile_options(${PROJECT_NAME} INTERFACE ${PROJECT_OPTIONS})
