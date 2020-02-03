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
     * Test @p path points to valid game path
     *
     * @param path
     * @return bool - true if plugin exists, false otherwise
     */
    bool CheckGamePath(const wxString &path);

    /**
     * Check if plugin needs to be updated
     *
     * @param game_path - Path to game directory
     * @param needs_update - Set to true if plugin needs to be updated
     * @param error - Error message
     * @return bool - true if no errors encountered, false otherwise
     */
    bool NeedsUpdate(const wxString &game_path, bool &needs_update, wxString &error);

    /**
     * Update plugin in game directory
     *
     * @param game_path - Path to game directory
     * @param error - Error message
     * @return bool - true if plugin updated successfully, false otherwise
     */
    bool Update(const wxString &game_path, wxString &error);

    /**
     * Check if plugin needs to be updated and update if needed
     *
     * @param game - Name of the game for display in error
     * @param path - Path to root of game install directory
     * @param error
     * @return bool - true on success, false otherwise
     */
    bool MaybeUpdate(const wxString &game, const wxString &path, wxString &error);
}

#endif //ETS2_JOB_LOGGER_PLUGININSTALLER_H
