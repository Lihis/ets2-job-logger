cmake_minimum_required(VERSION 3.7)
project(ets2-job-logger)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

add_subdirectory(scs-sdk)
add_subdirectory(plugin)
if(NOT MINGW)
    add_subdirectory(gui)
endif()
