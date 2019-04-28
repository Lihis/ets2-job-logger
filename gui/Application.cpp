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

#include "Application.h"
#include "SettingsWindow.h"
#include "Logger/PluginInstaller.h"
#include <wx/msgdlg.h>

Application::Application() : m_window(nullptr) {
    SetAppName("ets2-job-logger");
    SetAppDisplayName("ETS2 Job Logger");

    Connect(wxID_SETUP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Application::on_show_settings));
    Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Application::on_exit));
}

bool Application::OnInit() {
    if (!SettingsLoad()) {
        if (ShowSettings() != wxID_OK) {
            wxMessageBox("Valid settings are required, exiting..", "Error", wxOK, nullptr);
            return false;
        } else {
            if (!SettingsSave()) {
                wxMessageBox("Failed to save settings.", "Error", wxOK, nullptr);
                return false;
            }
        }
    }

    bool needs_update;
    wxString error;
    std::vector<wxString> game_paths = { GetETS2Path(), GetATSPath() };
    for (const auto &path : game_paths) {
        if (path.empty()) {
            continue;
        }

        if (!PluginInstaller::needs_update(path, needs_update, error)) {
            wxMessageBox(error, "Error", wxOK, nullptr);
            return false;
        } else if (needs_update) {
            if (!PluginInstaller::update(path, error)) {
                wxMessageBox(error, "Error", wxOK, nullptr);
                return false;
            }
        }
    }

    m_window = new MainWindow(this);
    m_window->Show(true);
    SetTopWindow(m_window);

    return m_window->start();
}

void Application::on_show_settings(wxCommandEvent &event/*event*/) {
    ShowSettings();
}

void Application::on_exit(wxCommandEvent &event) {
    bool exit = true;

    if (m_window) {
        exit = m_window->can_close();
    }

    if (exit) {
        Exit();
    } else {
        event.Skip();
    }
}

int Application::ShowSettings() {
    int ret;

    auto dialog = new SettingsWindow(this, m_window);
    ret = dialog->ShowModal();
    if (ret == wxID_OK) {
        if (!SettingsSave()) {
            wxMessageBox("Failed to save settings.", "Error", wxOK, m_window);
        }
    }
    delete dialog;

    return ret;
}
