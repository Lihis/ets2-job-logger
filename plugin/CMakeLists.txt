project(plugin VERSION 0.1.0)

add_library(${PROJECT_NAME}
            SHARED
            include/jobplugin/PluginDefs.h

            src/Logger.cpp
            src/Logger.h

            src/main.cpp
            )

configure_file(include/jobplugin/Version.h.in "include/jobplugin/Version.h")

add_library(ets2joblogger::plugin ALIAS ${PROJECT_NAME})

find_package(msgpack REQUIRED)
find_package(websocketpp REQUIRED)

if (WIN32)
    target_compile_options(${PROJECT_NAME}
                           PRIVATE
                           -DUNICODE
                           )
endif()

if(MINGW)
    set(WIN32_LIBS
        -lwsock32
        -lws2_32
        -static-libgcc
        -static-libstdc++
        -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
        )
endif()

set_target_properties(${PROJECT_NAME}
                      PROPERTIES
                      OUTPUT_NAME "ets2-job-logger"
                      PREFIX ""
                      )

target_link_libraries(${PROJECT_NAME}
                      PRIVATE
                      scs::sdk
                      ${WIN32_LIBS}
                      )

target_include_directories(${PROJECT_NAME}
                           INTERFACE
                           "${CMAKE_CURRENT_LIST_DIR}/include"
                           PUBLIC
                           "${CMAKE_CURRENT_BINARY_DIR}/include"
                           PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/include"
                           "${WEBSOCKETPP_INCLUDE_DIR}"
                           )

target_compile_options(${PROJECT_NAME}
                       PRIVATE
                       -DPLUGIN_INTERNAL=1
                       )

install(FILES
        INCLUDES DESTINATION include
        )
