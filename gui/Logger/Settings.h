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
 * @file Settings.h
 * @brief Application settings.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_SETTINGS_H
#define ETS2_JOB_LOGGER_SETTINGS_H

#include <wx/string.h>

class Settings {
public:
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const &) = delete;

    /**
     * Set ETS2 game path
     *
     * @param path
     */
    void SetETS2Path(const wxString &path);

    /**
     * Get ETS2 game path
     *
     * @return
     */
    wxString GetETS2Path() const;

    /**
     * Set API url
     *
     * @param url
     */
    void SetURL(const wxString &url);

    /**
     * Get API url
     *
     * @return
     */
    wxString GetURL() const;

    /**
     * Set API token
     *
     * @param token
     */
    void SetToken(const wxString &token);

    /**
     * Get API token
     *
     * @return
     */
    wxString GetToken() const;

protected:
    Settings() = default;
    ~Settings() = default;

    /**
     * Load settings
     *
     * @return true if loaded and valid settings, false otherwise
     */
    bool SettingsLoad();

    /**
     * Save settings
     *
     * @return true if saved successfully, false otherwise
     */
    bool SettingsSave();

private:
    /**
     * Get path to configuration file
     *
     * @return
     */
    wxString getConfigFile() const;

    /**
     * Get path to directory where configuration file is stored
     *
     * @return
     */
    wxString getConfigDir() const;

    wxString m_ets2_path;
    wxString m_url;
    wxString m_token;
};

#endif //ETS2_JOB_LOGGER_SETTINGS_H
