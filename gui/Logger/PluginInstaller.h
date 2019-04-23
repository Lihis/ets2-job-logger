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
 * @file PluginInstaller.h
 * @brief Handle plugin updating
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_PLUGININSTALLER_H
#define ETS2_JOB_LOGGER_PLUGININSTALLER_H

#include <wx/string.h>

namespace PluginInstaller {
    enum Platform {
        Linux = 0,
        Wine = 1,
        Windows = 2,
        macOS = 3
    };

    /**
     * Check if plugin needs to be updated
     *
     * @param game_path - Path to game directory
     * @param needs_update - Set to true if plugin needs to be updated
     * @param error - Error message
     * @return - true if no errors encountered, false otherwise
     */
    bool needs_update(const wxString &game_path, bool &needs_update, wxString &error);

    /**
     * Update plugin in game directory
     *
     * @param game_path - Path to game directory
     * @param error - Error message
     * @return - true if plugin updated successfully, false otherwise
     */
    bool update(const wxString &game_path, wxString &error);
}

#endif //ETS2_JOB_LOGGER_PLUGININSTALLER_H
