cmake_minimum_required(VERSION 3.7)
project(ets2-job-logger)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

if(MSVC OR MINGW)
    configure_file(packaging/Windows/InnoSetup.iss.in InnoSetup.iss @ONLY)
endif()

add_subdirectory(scs-sdk)
add_subdirectory(plugin)
add_subdirectory(gui)
