/****************************************************************************
 * Copyright (c) 2019 Tomi Lähteenmäki <lihis@lihis.net>                    *
 *                                                                          *
 * This program is free software; you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,               *
 * MA 02110-1301, USA.                                                      *
 ****************************************************************************/

#include "PluginInstaller.h"
#include "PluginInstallerDefs.h"
#ifdef __linux__
#include <wx/filefn.h>
#endif
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <fstream>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

PluginInstaller::Platform get_game_platform(const wxString &game_path) {
    PluginInstaller::Platform platform;

#if defined(__linux__) || defined(__APPLE__)
    if (wxDirExists(game_path + "/bin/win_x64")) {
        platform = PluginInstaller::Wine;
    } else {
        platform = PluginInstaller::Linux;
    }
#elif _WIN32
    (void)game_path;
    platform = PluginInstaller::Windows;
#endif

    return platform;
}

wxString get_plugin_extension(const PluginInstaller::Platform &platform) {
    wxString extension;

    switch (platform) {
        case PluginInstaller::Linux:
            extension = ".so";
            break;
        case PluginInstaller::Wine:
        case PluginInstaller::Windows:
            extension = ".dll";
            break;
        case PluginInstaller::macOS:
            // TODO: Must use .so extension? https://forum.scssoft.com/viewtopic.php?t=184132#p711388
            extension = ".so";
            break;
        default:
            break;
    }

    return extension;
}

wxString get_plugin_file_path(const PluginInstaller::Platform &platform) {
    wxString path;

    path = get_local_plugin_path() + wxFileName::GetPathSeparator();

#ifndef DEVELOPMENT
    switch (platform) {
        case PluginInstaller::Linux:
            path.append("linux_x64");
            break;
        case PluginInstaller::Wine:
        case PluginInstaller::Windows:
            path.append("win_x64");
            break;
        case PluginInstaller::macOS:
            path.append("macOS");
            break;
        default:
            break;
    }
    path.append(wxFileName::GetPathSeparator());
#endif

    path.append("ets2-job-logger" + get_plugin_extension(platform));

    return path;
}

wxString get_game_bin_directory(const PluginInstaller::Platform &platform,
                                 const wxString &game_path) {
    wxString path = game_path;

    switch (platform) {
        case PluginInstaller::Linux:
            path.append("/bin/linux_x64/");
            break;
        case PluginInstaller::Wine:
            path.append("/bin/win_x64/");
            break;
        case PluginInstaller::Windows:
            path.append(R"(\bin\win_x64\)");
            break;
        case PluginInstaller::macOS:
            path.append("/Contents/MacOS/");
            break;
        default:
            break;
    }

    return path;
}

wxString get_sha256(const wxString &file) {
    wxFile f(file, wxFile::OpenMode::read);
    if (!f.IsOpened()){
        return "";
    }

    char buffer[4096];
    unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    while (!f.Eof()) {
        auto read = f.Read(buffer, 4096);
        SHA256_Update(&ctx, buffer, read);
    }

    SHA256_Final(hash, &ctx);
    f.Close();

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (auto &c : hash) {
        ss << std::setw(2) << static_cast<unsigned int>(c);
    }

    return ss.str();
}

bool PluginInstaller::needs_update(const wxString &game_path,
                                   bool &needs_update, wxString &error) {
    Platform platform = get_game_platform(game_path);
    wxString pluginFile = get_plugin_file_path(platform);
    wxString gameBinDir = get_game_bin_directory(platform, game_path);
    wxString gamePluginDir = gameBinDir + "plugins";
    wxString installedPlugin = gamePluginDir + wxFileName::GetPathSeparator() +
            wxString("ets2-job-logger") + get_plugin_extension(platform);


    if (!wxFileExists(pluginFile)) {
        error = "Plugin file missing, check your program installation.";
        return false;
    }

    if (!wxFileExists(installedPlugin)) {
        needs_update = true;
        return true;
    }

    wxString localSHA = get_sha256(pluginFile);
    wxString installedSHA = get_sha256(installedPlugin);
    if (localSHA.empty() || installedSHA.empty()) {
        error = "Failed to determine plugins SHA256 sum.";
        return false;
    }

    needs_update = (installedSHA != localSHA);

    return true;
}

bool PluginInstaller::update(const wxString &game_path, wxString &error) {
    bool ret = true;
    Platform platform = get_game_platform(game_path);
    wxString pluginFile = get_plugin_file_path(platform);
    wxString gameBinDir = get_game_bin_directory(platform, game_path);
    wxString gamePluginDir = gameBinDir + "plugins";
    wxString installPath = gamePluginDir + wxFileName::GetPathSeparator() +
            wxString("ets2-job-logger") + get_plugin_extension(platform);

    if (!wxDirExists(gameBinDir)) {
        error = "ETS2 path does not seem correct, please select root of ETS2 directory";
        return false;
    }

    if (!wxDirExists(gamePluginDir)) {
        if (!wxMkdir(gamePluginDir)) {
            error = "Failed to create plugins directory.";
            return false;
        }
    }

    if (!wxCopyFile(pluginFile, installPath)) {
        error = "Failed to install plugin.";
        ret = false;
    }

    return ret;
}
