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

Application::Application() :
m_minimized(false),
m_checker(nullptr),
m_ipc(nullptr),
m_window(nullptr) {
    SetAppName("ets2-job-logger");
    SetAppDisplayName("ETS2 Job Logger");

    Connect(wxID_SETUP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Application::on_show_settings));
    Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Application::on_exit));
}

void Application::ShowWindow() {
    m_window->Raise();
    m_window->Show(true);
}

bool Application::OnInit() {
    if (!wxApp::OnInit()) {
        return false;
    }

    m_checker = new wxSingleInstanceChecker;
    if (m_checker->IsAnotherRunning()) {
        wxClient client;
        wxConnectionBase *connection = client.MakeConnection("localhost", "4242", "window");
        if (connection) {
            connection->Execute("raise");
        }
        delete connection;
        delete m_checker;
        return false;
    }

    m_ipc = new IPCServer(this);
    m_ipc->Create("4242");

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

    std::map<wxString, wxString> games = {  { "ETS2", GetETS2Path() } , { "ATS", GetATSPath() } };
    for (const auto &game : games) {
        if (game.second.empty()) {
            continue;
        }

        wxString error;
        if (!PluginInstaller::MaybeUpdate(game.first, game.second, error)) {
            wxMessageBox(error, "Error", wxOK, nullptr);
            if (ShowSettings() != wxID_OK) {
                wxMessageBox("Plugin needs to be installed, exiting..", "Error", wxOK, nullptr);
                return false;
            }
        }
    }

    m_window = new MainWindow(this);
    SetTopWindow(m_window);
    if (!m_minimized) {
        ShowWindow();
    }

    return m_window->start();
}

void Application::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(CMD_DESCRIPTION);
    parser.SetSwitchChars("-");
}

bool Application::OnCmdLineParsed(wxCmdLineParser &parser) {
    m_minimized = parser.Found("m");

    return true;
}

void Application::on_show_settings(wxCommandEvent &/*event*/) {
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

    delete m_checker;
    delete m_ipc;
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
