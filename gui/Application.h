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

/**
 * @file Application.h
 * @brief Application base.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_APPLICATION_H
#define ETS2_JOB_LOGGER_APPLICATION_H

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/snglinst.h>
#include <wx/ipc.h>
#include "Logger/Settings.h"
#include "MainWindow.h"

static const wxCmdLineEntryDesc CMD_DESCRIPTION[] = {
    {
        wxCMD_LINE_SWITCH,
        "m", "minimized", "Start minimized",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL
    },
    {
        wxCMD_LINE_SWITCH,
        "h", "help", "Displays this help and exit.",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
    },
    wxCMD_LINE_DESC_END
};

class IPCConnection;
class IPCServer;

class Application : public wxApp, public Settings {
public:
    Application();

    Application(Application const&) = delete;
    Application& operator=(Application const &) = delete;

    /**
     * Show main window
     */
    void ShowWindow();

protected:
    /**
     * Application init
     *
     * @return true, if settings are valid and plugin is installed
     */
    bool OnInit() final;

    /**
     * Command line init
     *
     * @param parser
     */
    void OnInitCmdLine(wxCmdLineParser &parser) final;

    /**
     * Parse command line arguments
     *
     * @param parser
     * @return bool - true if arguments were valid, false otherwise
     */
    bool OnCmdLineParsed(wxCmdLineParser &parser) final;

    /**
     * Show SettingsWindow event
     *
     * @param event
     */
    void on_show_settings(wxCommandEvent &event);

    /**
     * On application exit event
     *
     * @param event
     */
    void on_exit(wxCommandEvent &event);

private:
    /**
     * Show settings window and if OK clicked, save settings
     *
     * @return wxID_*
     */
    int ShowSettings();

    bool m_minimized;
    wxSingleInstanceChecker* m_checker;
    IPCServer *m_ipc;
    MainWindow *m_window;
};

class IPCConnection : public wxConnection {
public:
    explicit IPCConnection(Application *app) : m_app(app) {};

protected:
    bool OnExec(const wxString &topic, const wxString &data) override {
        if (topic.compare("window") == 0 && data.compare("raise") == 0) {
            m_app->ShowWindow();
            return true;
        }

        return false;
    }

private:
    Application *m_app;
};

class IPCServer : public wxServer {
public:
    explicit IPCServer(Application *app) : m_app(app) {};

    wxConnectionBase *OnAcceptConnection(const wxString &/*topic*/) override {
        return new IPCConnection(m_app);
    }

private:
    Application *m_app;
};

#endif //ETS2_JOB_LOGGER_APPLICATION_H
