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

#ifndef ETS2_JOB_LOGGER_FINE_H
#define ETS2_JOB_LOGGER_FINE_H

/**
 * @file Fine.h
 * @brief Fine structure
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include <jobplugin/Serializable.h>
#include <msgpack.hpp>

enum class Fine {
    Unknown = 0,
    Crash = 1,
    AvoidSleeping = 2,
    WrongWay = 3,
    SpeedingCamera = 4,
    NoLights = 5,
    RedSignal = 6,
    Speeding = 7,
    AvoidWeighing = 8,
    IllegalTrailer = 9,
    AvoidInspection = 10,
    IllegalBorderCrossing = 11,
    HardShoulderViolation = 12,
    DamagedVehicleUsage = 13,
    Generic = 14
};
MSGPACK_ADD_ENUM(Fine)

struct fine_t : serializable_t {
    fine_t() : type(Fine::Unknown), amount(0) {
    }

    Fine type;
    int64_t amount;

    MSGPACK_DEFINE(type, amount);

#if WITH_SERIALIZATION
    Json::Value ToJson() const final;
    std::string ToString() const final;
#endif
};

#endif //ETS2_JOB_LOGGER_FINE_H
