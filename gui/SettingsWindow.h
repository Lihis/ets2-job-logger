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
 * @file SettingsWindow.h
 * @brief Applications settings window.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_SETTINGSWINDOW_H
#define ETS2_JOB_LOGGER_SETTINGSWINDOW_H

#include "base/SettingsWindow.h"
#include "Logger/Settings.h"
#include "MainWindow.h"

class SettingsWindow : public base::SettingsWindow {
public:
    SettingsWindow(Settings *settings, MainWindow *mainWindow);

protected:
    /**
     * Install ETS2 checkbox check/uncheck event
     *
     * @param event
     */
    void on_install_ets2(wxCommandEvent &event) final;

    /**
     * Install ATS checkbox check/uncheck event
     *
     * @param event
     */
    void on_install_ats(wxCommandEvent &event) final;

    /**
     * Set settings when OK button is clicked and close dialog
     * @fixme Validation should be done on Settings side
     */
    void on_click_ok(wxCommandEvent &event) final;

private:
    Settings *m_settings;
    MainWindow *m_mainWindow;
};


#endif //ETS2_JOB_LOGGER_SETTINGSWINDOW_H
