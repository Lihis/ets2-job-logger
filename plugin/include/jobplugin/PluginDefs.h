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

#ifndef ETS2_JOB_LOGGER_PLUGINDEFS_H
#define ETS2_JOB_LOGGER_PLUGINDEFS_H

/**
 * @file PluginDefs.h
 * @brief Shared header for plugin and application
 * @details Definition of data transported via WebSocket.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include <string>
#include <msgpack.hpp>

#define WEBSOCK_PORT 20210

enum class PacketType {
    Unknown = 0,
    Version = 1,
    Job = 2,
    Truck = 3,
    CargoDamage = 4,
    Fine = 5
};
MSGPACK_ADD_ENUM(PacketType)

enum class Game {
    Unknown = 0,
    ETS2 = 1,
    ATS = 2
};
MSGPACK_ADD_ENUM(Game)

#endif //ETS2_JOB_LOGGER_PLUGINDEFS_H
