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

#ifndef ETS2_JOB_LOGGER_JOB_H
#define ETS2_JOB_LOGGER_JOB_H

/**
 * @file Job.h
 * @brief Job information
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include <msgpack.hpp>
#include <jobplugin/PluginDefs.h>
#include <jobplugin/Truck.h>
#include <jobplugin/Trailer.h>
#include <jobplugin/Cargo.h>
#include <jobplugin/Distance.h>
#include <jobplugin/Source.h>
#include <jobplugin/Destination.h>

enum class JobStatus {
    FreeAsWind = 0,
    OnJob = 1,
    Cancelled = 2,
    Delivered = 3
};
MSGPACK_ADD_ENUM(JobStatus)

enum class JobType {
    Unknown = 0,
    CargoMarket = 1,
    QuickJob = 2,
    FreightMarket = 3,
    ExternalContract = 4,
    ExternalMarket = 5
};
MSGPACK_ADD_ENUM(JobType)

struct job_t : serializable_t {
    explicit job_t(const Game &game = Game::Unknown) {
        this->game = game;
        status = JobStatus::FreeAsWind;
        type = JobType::Unknown;
        isSpecial = false;
        income = 0;
        revenue = 0;
        xp = 0;
        timeSpend = 0;
        maxSpeed = 0.f;
        fuelConsumed = 0.f;
        autoPark = false;
        autoLoad = false;
        distance = {};
        trailer = {};
        cargo = {};
        source = {};
        destination = {};
    }

    void setSpecial(bool special)
    {
        isSpecial = special;
        source.isSpecial = special;
        destination.isSpecial = special;
    }

    Game game;
    JobStatus status;
    JobType type;
    bool isSpecial;
    uint64_t income;
    int64_t revenue;
    int32_t xp;
    uint32_t timeSpend;
    float maxSpeed;
    float fuelConsumed;
    bool autoPark;
    bool autoLoad;

    distance_t distance;

    trailer_t trailer;
    truck_t truck;
    cargo_t cargo;

    source_t source;
    destination_t destination;

    MSGPACK_DEFINE(game, status, type, isSpecial, income, revenue, xp, timeSpend, maxSpeed, fuelConsumed, autoPark, autoLoad, distance, trailer, truck, cargo, source, destination);

#if WITH_SERIALIZATION
    Json::Value ToJson() const final;
    std::string ToString() const final;
#endif
};

#endif //ETS2_JOB_LOGGER_JOB_H
