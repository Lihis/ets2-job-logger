project(ets2-job-logger-gui VERSION 0.1.0)

set(SOURCES
    Logger/JobSender.cpp
    Logger/JobSender.h

    Logger/PluginInstaller.cpp
    Logger/PluginInstaller.h

    Logger/Settings.cpp
    Logger/Settings.h

    base/AboutDialog.cpp
    base/AboutDialog.h

    base/MainWindow.cpp
    base/MainWindow.h

    base/SettingsWindow.cpp
    base/SettingsWindow.h

    AboutDialog.cpp
    AboutDialog.h

    Application.cpp
    Application.h

    MainWindow.cpp
    MainWindow.h

    SettingsWindow.cpp
    SettingsWindow.h

    main.cpp
    )

if(WIN32)
    enable_language(RC)
    set(SOURCES ${SOURCES} resources/icon/icon.rc)
endif()

find_package(wxWidgets 3.0 REQUIRED)
include(${wxWidgets_USE_FILE})
if(MSVC)
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

if(NOT WIN32)
    if (PLUGIN_PATH)
        set(PLUGIN_INSTALL_PATH ${PLUGIN_PATH})
    else()
        set(PLUGIN_INSTALL_PATH /etc/ets2-job-logger)
    endif()
endif()

include(GetGitRevisionDescription)
git_describe(GIT_SHA1)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING ${GIT_SHA1} 0 7 GIT_SHA1)
git_local_changes(GIT_STATUS)
if(${GIT_STATUS} STREQUAL "DIRTY")
    set(GIT_SHA1 "${GIT_SHA1}-${GIT_STATUS}")
endif()
configure_file(Version.h.in Version.h)
configure_file(Logger/PluginInstallerDefs.h.in PluginInstallerDefs.h)
if(MSVC OR MINGW)
    configure_file(version.iss.in version.iss)
endif()

add_executable(ets2-job-logger WIN32 ${SOURCES})

if(MINGW)
    set(WIN32_LIBS -lws2_32 -lcrypt32)
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
                           )

target_compile_options(ets2-job-logger
                       PRIVATE
                       ${wxWidgets_CFLAGS_OTHER}
                       ${JSONCPP_CFLAGS_OTHER}
                       )

if(DEVELOPMENT)
    target_compile_options(ets2-job-logger
                           PRIVATE
                           -DDEVELOPMENT=1
                           )
endif()

if(UNIX AND NOT APPLE)
    install(FILES resources/icon/16x16.png DESTINATION share/icons/hicolor/16x16/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/24x24.png DESTINATION share/icons/hicolor/24x24/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/32x32.png DESTINATION share/icons/hicolor/32x32/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/48x48.png DESTINATION share/icons/hicolor/48x48/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/64x64.png DESTINATION share/icons/hicolor/64x64/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/128x128.png DESTINATION share/icons/hicolor/128x128/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/256x256.png DESTINATION share/icons/hicolor/256x256/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/512x512.png DESTINATION share/icons/hicolor/512x512/apps RENAME ets2-job-logger.png)
    install(FILES resources/icon/icon.svg DESTINATION share/icons/hicolor/scalable/apps RENAME ets2-job-logger.svg)
endif()
