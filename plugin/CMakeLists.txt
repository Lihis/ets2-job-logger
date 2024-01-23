project(plugin VERSION 0.1.0)

if(MSVC)
    find_package(jsoncpp CONFIG REQUIRED)
    set(JSONCPP_LIBRARIES jsoncpp_lib_static)
    set(JSONCPP_INCLUDE_DIRS jsoncpp_lib_static)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP jsoncpp REQUIRED)
endif()
find_package(msgpack NAMES msgpack msgpack-cxx msgpackc-cxx REQUIRED)
find_package(websocketpp REQUIRED)

set(HDRS
    include/jobplugin/Brand.h
    include/jobplugin/Cargo.h
    include/jobplugin/City.h
    include/jobplugin/Company.h
    include/jobplugin/Destination.h
    include/jobplugin/Distance.h
    include/jobplugin/Fine.h
    include/jobplugin/IdName.h
    include/jobplugin/Job.h
    include/jobplugin/PluginDefs.h
    include/jobplugin/Position.h
    include/jobplugin/Serializable.h
    include/jobplugin/Source.h
    include/jobplugin/SourceDestination.h
    include/jobplugin/Trailer.h
    include/jobplugin/Truck.h
    )

set(SERIALIZATION_SRCS
    ${HDRS}
    src/Cargo.cpp
    src/Distance.cpp
    src/Fine.cpp
    src/IdName.cpp
    src/Position.cpp
    src/SourceDestination.cpp
    src/Trailer.cpp
    src/Truck.cpp
    src/Job.cpp)

set(PLUGIN_SRCS
    ${HDRS}
    src/Logger.cpp
    src/Logger.h

    src/main.cpp
    )

configure_file(include/jobplugin/Version.h.in "include/jobplugin/Version.h")

add_library(serialization OBJECT ${SERIALIZATION_SRCS})
add_library(ets2joblogger::serialization ALIAS serialization)
target_include_directories(serialization
                           PUBLIC
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                           ${JSONCPP_INCLUDE_DIRS}
                           )
target_link_libraries(serialization
                      INTERFACE
                      ${JSONCPP_LIBRARIES}
                      )
target_compile_options(serialization
                       PUBLIC
                       -DWITH_SERIALIZATION=1
                       )
set_property(TARGET serialization PROPERTY POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    add_library(${PROJECT_NAME} OBJECT ${SRCS})
    add_library(plugin_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}> plugin.def)
    add_library(plugin_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

    set_target_properties(plugin_shared
                          PROPERTIES
                          OUTPUT_NAME "ets2-job-logger"
                          PREFIX ""
                          )

    set_target_properties(plugin_static
                          PROPERTIES
                          OUTPUT_NAME "ets2-job-logger"
                          PREFIX ""
                          )

    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
else()
    add_library(${PROJECT_NAME} SHARED ${PLUGIN_SRCS})

    set_target_properties(${PROJECT_NAME}
                          PROPERTIES
                          OUTPUT_NAME "ets2-job-logger"
                          PREFIX ""
                          )
endif()
add_library(ets2joblogger::plugin ALIAS ${PROJECT_NAME})
add_dependencies(plugin serialization)

if(WIN32)
    target_compile_options(${PROJECT_NAME}
                           PRIVATE
                           -DUNICODE
                           )
endif()

if(MINGW)
    target_link_libraries(${PROJECT_NAME}
                          PRIVATE
                          -lwsock32
                          -lws2_32
                          -static-libgcc
                          -static-libstdc++
                          -Wl,-Bstatic -lstdc++ -lpthread -lssp -Wl,-Bdynamic
                          )
endif()

target_link_libraries(${PROJECT_NAME}
                      PRIVATE
                      scs::sdk
                      )

target_include_directories(${PROJECT_NAME}
                           PRIVATE
                           "${WEBSOCKETPP_INCLUDE_DIR}"
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                           )

install(FILES
        INCLUDES DESTINATION include
        )
