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

#include "Settings.h"
#include "PluginInstaller.h"
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#ifdef __linux__
#include <fstream>
#elif _WIN32
#include <wx/msw/registry.h>
#endif

void Settings::SetETS2Path(const wxString &path) {
    m_ets2_path = path;
}

wxString Settings::GetETS2Path() const {
    return m_ets2_path;
}

void Settings::SetATSPath(const wxString &path) {
    m_ats_path = path;
}

wxString Settings::GetATSPath() const {
    return m_ats_path;
}

void Settings::SetURL(const wxString &url) {
    m_url = url;
}

wxString Settings::GetURL() const {
    return m_url;
}

void Settings::SetToken(const wxString &token) {
    m_token = token;
}

wxString Settings::GetToken() const {
    return m_token;
}

bool Settings::SetStartOnStartup(bool enabled) {
    bool ret = enabled ? EnableStartOnStartup() : DisableStartOnStartup();

    return ret;
}

bool Settings::GetStartOnStartup() const {
    bool ret;

#ifdef __linux__
    ret = wxFileExists(getAutostartDir() + "/ets2-job-logger.desktop");
#elif _WIN32
    wxRegKey startup(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    ret = startup.HasValue("ETS2JobLogger");
#elif __APPLE__
#warning "Settings::GetStartOnBoot() not implemented"
    ret = false;
#endif

    return ret;
}

void Settings::SetRunInBackground(bool enabled) {
    m_run_in_background = enabled;
}

bool Settings::GetRunInBackground() const {
    return m_run_in_background;
}

bool Settings::SettingsLoad() {
    wxString confFile;
    wxFileConfig *config;

    confFile = getConfigFile();
    if (confFile.empty()) {
        return false;
    }

    config = new wxFileConfig(wxEmptyString, wxEmptyString, confFile);

    config->SetPath("ets2");
    m_ets2_path = config->Read("path", "");

    config->SetPath("ats");
    m_ats_path = config->Read("path", "");

    config->SetPath("api");
    m_url = config->Read("url", "");
    if (!m_url.empty()) {
        if (m_url[m_url.length() - 1] != '/') {
            m_url.append('/');
        }
    }
    m_token = config->Read("token", "");

    config->SetPath("miscellaneous");
    m_run_in_background = config->Read("runBackground", true);

    bool pluginPathOk = (!m_ets2_path.empty() || !m_ats_path.empty());

    if (!m_ets2_path.empty()) {
        pluginPathOk &= PluginInstaller::CheckGamePath(m_ets2_path);
    }

    if (!m_ats_path.empty()) {
        pluginPathOk &= PluginInstaller::CheckGamePath(m_ats_path);
    }

    delete config;

    return (pluginPathOk && !m_url.empty() && !m_token.empty());
}

bool Settings::SettingsSave() {
    wxString confFile;
    wxFileConfig *config;

    confFile = getConfigFile();
    if (confFile.empty()) {
        return false;
    }

    if (!wxDirExists(wxPathOnly(confFile))) {
        if (!wxMkdir(wxPathOnly(confFile))) {
            return false;
        }
    }

    config = new wxFileConfig(wxEmptyString, wxEmptyString, confFile);
    config->SetPath("ets2");
    config->Write("path", m_ets2_path);

    config->SetPath("ats");
    config->Write("path", m_ats_path);

    config->SetPath("api");
    config->Write("url", m_url);
    config->Write("token", m_token);

    config->SetPath("miscellaneous");
    config->Write("runBackground", m_run_in_background);

    delete config;

    return true;
}

bool Settings::EnableStartOnStartup() {
    wxString executable = wxStandardPaths::Get().GetExecutablePath();

#ifdef __linux__
    wxString desktopFile = getAutostartDir() + "/ets2-job-logger.desktop";

    std::string content("[Desktop Entry]\n"
                        "Encoding=UTF-8\n"
                        "Type=Application\n"
                        "Name=ETS2 Job Logger\n"
                        "Comment=ETS2 and ATS job logger\n"
                        "Exec=" + executable + " --minimized\n"
                                               "StartupNotify=false\n"
                                               "Terminal=false\n"
                                               "Hidden=false");

    std::ofstream fd(desktopFile, std::ios::out);
    if (fd.is_open()) {
        fd << content;
        fd.close();
        return true;
    } else {
        return false;
    }
#elif _WIN32
    wxString keyName("ETS2JobLogger");
    wxRegKey startup(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    return startup.SetValue(keyName, executable.append(" --minimized"));
#elif __APPLE__
#warning "Settings::SetStartOnStartup() not implemented"
    return true;
#endif
}

bool Settings::DisableStartOnStartup() {
    bool ret = true;

#ifdef __linux__
    wxString desktopFile = getAutostartDir() + "/ets2-job-logger.desktop";

    if (wxFileExists(desktopFile)) {
        ret = wxRemoveFile(desktopFile);
    }
#elif _WIN32
    wxString keyName("ETS2JobLogger");
    wxRegKey startup(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    if (startup.HasValue(keyName)) {
        ret = startup.DeleteValue(keyName);
    }
#elif __APPLE__
#warning "Settings::DisableStartOnStartup(): not implemented"
#endif

    return ret;
}

wxString Settings::getConfigFile() const {
    wxString path;

    path = getConfigDir();
    if (path.empty()) {
        return "";
    } else {
        path += wxFileName::GetPathSeparator() +  wxString("ets2-job-logger");
    }

    return path + wxFileName::GetPathSeparator() + wxString("config.ini");
}

wxString Settings::getConfigDir() const {
#ifdef __linux__
    char *path = std::getenv("XDG_DATA_HOME");
    if (!path) {
        path = std::getenv("HOME");
        if (path) {
            return std::string(path).append("/.local/share").c_str();
        }
    }

    return path;
#else
    return wxStandardPaths::Get().GetUserConfigDir();
#endif
}

#ifdef __linux__
wxString Settings::getAutostartDir() const {
    std::string path;

    char *tmp = std::getenv("XDG_CONFIG_HOME");
    if (tmp) {
        path = tmp;
    } else {
        tmp = std::getenv("HOME");
        if (tmp) {
            path = std::string(tmp).append("/.config");
        }
    }
    path.append("/autostart");

    if (!wxDirExists(path)) {
        wxFileName::Mkdir(path);
    }

    return path;
}
#endif
