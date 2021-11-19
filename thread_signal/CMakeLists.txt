cmake_minimum_required(VERSION 3.20)

project(thread_signal VERSION "0.0.1" LANGUAGES CXX)

set(SOURCE_FILES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


## Setup Conan
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      )
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)
#conan_add_remote(NAME conancenter URL https://center.conan.io)
conan_check(REQUIRED)
conan_cmake_autodetect(settings)
#conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_SOURCE_DIR}/conanfile.txt BUILD missing SETTINGS ${settings})
conan_cmake_run(CONANFILE ../conanfile.txt BASIC_SETUP NO_OUTPUT_DIRS #[[BUILD missing]] SETTINGS ${settings})


## Find third-party packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})
find_package(fmt REQUIRED)
find_package(sigc++-3 REQUIRED)

set(PROJECT_OPTIONS
    -O3
    -Wpedantic
    -Wall
    -Wextra
    -Werror
    )

## Target
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    fmt::fmt
    sigc++-3::sigc++-3
    pthread
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_options(${PROJECT_NAME} INTERFACE ${PROJECT_OPTIONS})
