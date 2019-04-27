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
#include <wx/fileconf.h>
#include <wx/filename.h>
#ifndef __linux__
#include <wx/stdpaths.h>
#endif

void Settings::SetETS2Path(const wxString &path) {
    m_ets2_path = path;
}

wxString Settings::GetETS2Path() const {
    return m_ets2_path;
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

bool Settings::SettingsLoad() {
    wxString confFile;
    wxFileConfig *config;

    confFile = getConfigFile();
    if (confFile.empty()) {
        return false;
    }

    config = new wxFileConfig(wxEmptyString, wxEmptyString, confFile);

    config->SetPath("game");
    m_ets2_path = config->Read("ets2", "");

    config->SetPath("api");
    m_url = config->Read("url", "");
    if (!m_url.empty()) {
        if (m_url[m_url.length() - 1] != '/') {
            m_url.append('/');
        }
    }
    m_token = config->Read("token", "");

    return (!m_ets2_path.empty() && !m_url.empty() && !m_token.empty());
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
    config->SetPath("game");
    config->Write("ets2", m_ets2_path);
    config->SetPath("api");
    config->Write("url", m_url);
    config->Write("token", m_token);
    delete config;

    return true;
}

wxString Settings::getConfigFile() const {
    wxString path;

    path = getConfigDir();
    if (path.empty()) {
        return "";
    } else {
        path += wxFileName::GetPathSeparator() +  wxString("ets2-telemetry-server");
    }

    return path + wxFileName::GetPathSeparator() + wxString("config.ini");
}

wxString Settings::getConfigDir() const {
#ifdef __linux__
    char *path = std::getenv("XDG_CONFIG_HOME");
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
