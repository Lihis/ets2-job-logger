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
#include "Logger/Settings.h"
#include "MainWindow.h"

class Application : public wxApp, public Settings {
public:
    Application();

    Application(Application const&) = delete;
    Application& operator=(Application const &) = delete;

protected:
    /**
     * Application init
     *
     * @return true, if settings are valid and plugin is installed
     */
    bool OnInit() final;

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

    MainWindow *m_window;
};

#endif //ETS2_JOB_LOGGER_APPLICATION_H
