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

#ifndef ETS2_JOB_LOGGER_TRUCK_H
#define ETS2_JOB_LOGGER_TRUCK_H

/**
 * @file Truck.h
 * @brief Truck information
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include <msgpack.hpp>
#include <jobplugin/Brand.h>
#include <jobplugin/Position.h>
#include <jobplugin/Serializable.h>

struct truck_t : serializable_t {
    truck_t() : id(), name(), wheels(0), brand(), odometer(-1.f), fuel(-1.f), speed(0.f), position() {
    }

    std::string id;
    std::string name;
    uint32_t wheels;
    brand_t brand;
    float odometer;
    float fuel;
    float speed;
    position_t position;

    MSGPACK_DEFINE(id, name, wheels, brand, odometer, fuel, speed, position);

#if WITH_SERIALIZATION
    Json::Value ToJson() const final;
    std::string ToString() const final;
#endif
};

#endif //ETS2_JOB_LOGGER_TRUCK_H
