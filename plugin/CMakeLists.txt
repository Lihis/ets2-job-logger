project(plugin VERSION 0.1.0)

set(SRCS
    include/jobplugin/PluginDefs.h

    src/Logger.cpp
    src/Logger.h

    src/main.cpp
    )

configure_file(include/jobplugin/Version.h.in "include/jobplugin/Version.h")

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
    add_library(${PROJECT_NAME} SHARED ${SRCS})

    set_target_properties(${PROJECT_NAME}
                          PROPERTIES
                          OUTPUT_NAME "ets2-job-logger"
                          PREFIX ""
                          )
endif()
add_library(ets2joblogger::plugin ALIAS ${PROJECT_NAME})

find_package(msgpack REQUIRED)
find_package(websocketpp REQUIRED)

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
                           PUBLIC
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                           PRIVATE
                           "${WEBSOCKETPP_INCLUDE_DIR}"
                           )

target_compile_options(${PROJECT_NAME}
                       PRIVATE
                       -DPLUGIN_INTERNAL=1
                       )

install(FILES
        INCLUDES DESTINATION include
        )
