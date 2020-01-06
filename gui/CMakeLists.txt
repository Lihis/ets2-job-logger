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
include(${wxWidgets_USE_FILE})
if (MSVC)
    find_package(PNG REQUIRED)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost REQUIRED COMPONENTS random)
else()
    find_package(Boost REQUIRED COMPONENTS system)
endif()
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
if (MSVC)
    find_package(jsoncpp CONFIG REQUIRED)
    set(JSONCPP_LIBRARIES jsoncpp_lib_static)
    set(JSONCPP_INCLUDE_DIRS jsoncpp_lib_static)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP jsoncpp REQUIRED)
endif()

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

if (MINGW)
    set(WIN32_LIBS -lws2_32)
elseif (WIN32)
    find_library(WS2_32_LIB ws2_32.lib REQUIRED)
    find_library(GDI32_LIB gdi32.lib REQUIRED)
    find_library(ADVAPI32_LIB advapi32.lib REQUIRED)
    find_library(CRYPT32_LIB crypt32.lib REQUIRED)
    find_library(USER32_LIB user32.lib REQUIRED)
    set(WIN32_LIBS ${WS2_32_LIB} ${GDI32_LIB} ${ADVAPI32_LIB} ${CRYPT32_LIB} ${USER32_LIB})
endif()

target_link_libraries(ets2-job-logger
                      PRIVATE
                      ets2joblogger::plugin
                      ${wxWidgets_LIBRARIES}
                      ${PNG_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${WIN32_LIBS}
                      ${ZLIB_LIBRARIES}
                      ${OPENSSL_LIBRARIES}
                      ${CURL_LIBRARIES}
                      ${JSONCPP_LIBRARIES}
                      )

target_include_directories(ets2-job-logger
                           PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           "${WEBSOCKETPP_INCLUDE_DIR}"
                           "${wxWidgets_INCLUDE_DIRS}"
                           "${PNG_INCLUDE_DIRS}"
                           "${Boost_INCLUDE_DIRS}"
                           "${ZLIB_INCLUDE_DIRS}"
                           "${OPENSSL_INCLUDE_DIRS}"
                           "${CURL_INCLUDE_DIRS}"
                           "${JSONCPP_INCLUDE_DIRS}"
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
