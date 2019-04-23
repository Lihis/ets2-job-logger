project(ets2-job-logger-gui VERSION 0.1.0)

set(SOURCES
    Logger/JobSender.cpp
    Logger/JobSender.h

    Logger/PluginInstaller.cpp
    Logger/PluginInstaller.h

    Logger/Settings.cpp
    Logger/Settings.h

    base/MainWindow.cpp
    base/MainWindow.h

    base/SettingsWindow.cpp
    base/SettingsWindow.h

    Application.cpp
    Application.h

    MainWindow.cpp
    MainWindow.h

    SettingsWindow.cpp
    SettingsWindow.h

    main.cpp
    )

find_package(wxWidgets 3.0 REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
pkg_check_modules(JSONCPP jsoncpp REQUIRED)
include(${wxWidgets_USE_FILE})

if (NOT WIN32)
    if (PLUGIN_PATH)
        set(PLUGIN_INSTALL_PATH ${PLUGIN_PATH})
    else()
        set(PLUGIN_INSTALL_PATH /etc/ets2-job-logger)
    endif()
endif()

configure_file(Version.h.in Version.h)
configure_file(Logger/PluginInstallerDefs.h.in PluginInstallerDefs.h)

add_executable(ets2-job-logger WIN32 ${SOURCES})

if (WIN32)
    set(WIN32_LIBS -lws2_32)
endif()

target_link_libraries(ets2-job-logger
                      PRIVATE
                      ets2joblogger::plugin
                      ${wxWidgets_LIBRARIES}
                      ${WIN32_LIBS}
                      ${JSONCPP_LIBRARIES}
                      ${CURL_LIBRARIES}
                      ${OPENSSL_CRYPTO_LIBRARY}
                      )

target_include_directories(ets2-job-logger
                           PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           "${WEBSOCKETPP_INCLUDE_DIR}"
                           "${wxWidgets_INCLUDE_DIRS}"
                           "${JSONCPP_INCLUDE_DIRS}"
                           "${CURL_INCLUDE_DIRS}"
                           )

target_compile_options(ets2-job-logger
                       PRIVATE
                       ${wxWidgets_CFLAGS_OTHER}
                       ${wxWidgets_CFLAGS_OTHER}
                       ${JSON_CPP_CFLAGS_OTHER}
                       )

if (DEVELOPMENT)
    target_compile_options(ets2-job-logger
                           PRIVATE
                           -DDEVELOPMENT=1
                           )
endif()
