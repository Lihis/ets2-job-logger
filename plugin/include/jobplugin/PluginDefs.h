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
 * @file PluginDefs.h
 * @brief Shared header for plugin and application
 * @details Definition of data transported via WebSocket.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_PLUGINDEFS_H
#define ETS2_JOB_LOGGER_PLUGINDEFS_H

#include "jobplugin/Version.h"
#include <string>
#include <msgpack.hpp>
#ifndef PLUGIN_INTERNAL
#include <json/value.h>
#endif

#define WEBSOCK_PORT 20210

enum class PacketType {
    Unknown = 0,
    Version = 1,
    Job = 2,
    JobPartial = 3,
    Truck = 4
};
MSGPACK_ADD_ENUM(PacketType)

enum class Game {
    Unknown = 0,
    ETS2 = 1,
    ATS = 2
};
MSGPACK_ADD_ENUM(Game)

struct version_t {
    explicit version_t(uint8_t major = 0, uint8_t minor = 0, uint8_t patch = 0) {
        this->major = 0;
        this->minor = 0;
        this->patch = 0;
    }

    uint8_t major;
    uint8_t minor;
    uint8_t patch;

    MSGPACK_DEFINE(major, minor, patch);
};

struct truck_t {
    truck_t() {
        odometer = 0.f;
        fuel = 0.f;
    }

    float odometer;
    float fuel;

    MSGPACK_DEFINE(odometer, fuel);
};

struct job_partial_t {
    explicit job_partial_t(float drivenKm = 0.f,
                           float fuelConsumed = 0.f,
                           float trailerDamage = 0.f) {
        this->drivenKm = drivenKm;
        this->fuelConsumed = fuelConsumed;
        this->trailerDamage = trailerDamage;
    }

    float drivenKm;
    float fuelConsumed;
    float trailerDamage;

    MSGPACK_DEFINE(drivenKm, fuelConsumed, trailerDamage);
};

struct job_t {
    explicit job_t(const Game &game = Game::Unknown) {
        this->game = game;
        onJob = false;
        delivered = false;
        drivenKm = 0.f;
        fuelConsumed = 0.f;
        income = 0;
        trailer = {};
        cargo = {};
        source = {};
        destination = {};
    }

    Game game;
    bool onJob;
    bool delivered;

    float drivenKm;
    float fuelConsumed;

    uint64_t income;

    struct trailer_t {
        trailer_t() {
            connected = false;
            damage = 0.f;
        }

        bool connected;
        float damage;
        MSGPACK_DEFINE(connected, damage)
    } trailer;

    struct cargo_t {
        cargo_t() {
            name = "-";
            id = "";
            mass = 0.f;
        }

        std::string name;
        std::string id;
        float mass;
        MSGPACK_DEFINE(name, id, mass)
    } cargo;

    struct source_t {
        source_t() {
            city = "-";
            cityId = "";
            company = "-";
            companyId = "";
        }

        std::string city;
        std::string cityId;
        std::string company;
        std::string companyId;
        MSGPACK_DEFINE(city, cityId, company, companyId);
    } source;

    struct destination_t {
        destination_t() {
            city = "-";
            cityId = "";
            company = "-";
            companyId = "";
        }
        std::string city;
        std::string cityId;
        std::string company;
        std::string companyId;
        MSGPACK_DEFINE(city, cityId, company, companyId);
    } destination;

    MSGPACK_DEFINE(game, onJob, delivered, drivenKm, fuelConsumed, income, trailer, cargo, source, destination);

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root) const {
        std::string game_name;

        switch (game) {
            case Game::ETS2:
                game_name = "ets2";
                break;
            case Game::ATS:
                game_name = "ats";
                break;
            case Game::Unknown:
            default:
                break;
        }

        root["game"] = game_name;
        root["onJob"] = onJob;
        root["delivered"] = delivered;
        root["distanceDriven"] = drivenKm;
        root["fuelConsumed"] = fuelConsumed;
        root["income"] = income;
        root["trailerDamage"] = trailer.damage;
        root["cargoName"] = cargo.name;
        root["cargoId"] = cargo.id;
        root["cargoMass"] = cargo.mass;
        root["sourceCity"] = source.city;
        root["sourceCityId"] = source.cityId;
        root["sourceCompany"] = source.company;
        root["sourceCompanyId"] = source.companyId;
        root["destinationCity"] = destination.city;
        root["destinationCityId"] = destination.cityId;
        root["destinationCompany"] = destination.company;
        root["destinationCompanyId"] = destination.companyId;
    }
#endif
};

static bool operator==(job_t const &lhs, job_t const &rhs) {
    return lhs.onJob == rhs.onJob;
}

#endif //ETS2_JOB_LOGGER_PLUGINDEFS_H
