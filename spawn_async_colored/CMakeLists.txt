cmake_minimum_required(VERSION 3.19)
project(spawn_async_colored
  VERSION "0.0.3"
  DESCRIPTION "Example of running spawned async tasks with colored output"
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME} main.cpp)


## Setup Conan
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
  )
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_autodetect(settings)
conan_check(REQUIRED)
conan_cmake_run(CONANFILE ../conanfile.txt
  BASIC_SETUP NO_OUTPUT_DIRS
  BUILD missing
  SETTINGS ${settings}
)

## Find third-party packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})
find_package(fmt REQUIRED)


## Target
target_link_libraries(${PROJECT_NAME} pthread fmt::fmt)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
