/****************************************************************************
 * Copyright (c) 2022 Tomi Lähteenmäki <lihis@lihis.net>                    *
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

#ifndef ETS2_JOB_LOGGER_POSITION_H
#define ETS2_JOB_LOGGER_POSITION_H

/**
 * @file Position.h
 * @brief Position information
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include <msgpack.hpp>
#include <jobplugin/Serializable.h>

struct position_t : serializable_t {
    position_t() : x(0.f), y(0.f), z(0.f), heading(0.f) {
    }

    double x;
    double y;
    double z;
    float heading;

    MSGPACK_DEFINE(x, y, z, heading);

#if WITH_SERIALIZATION
    Json::Value ToJson() const final;
    std::string ToString() const final;
#endif
};

#endif //ETS2_JOB_LOGGER_POSITION_H
