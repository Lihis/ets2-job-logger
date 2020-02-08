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

#include <jobplugin/Version.h>
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
    Truck = 3,
    CargoDamage = 4
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
        this->major = major;
        this->minor = minor;
        this->patch = patch;
    }

    uint8_t major;
    uint8_t minor;
    uint8_t patch;

    MSGPACK_DEFINE(major, minor, patch);
};

struct id_name_t {
    id_name_t() : id(), name() {
    }

    std::string id;
    std::string name;

    MSGPACK_DEFINE(id, name);

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root) const {
        root["id"] = id;
        root["name"] = name;
    }
#endif
};
typedef struct id_name_t city_t;
typedef struct id_name_t company_t;
typedef struct id_name_t brand_t;

struct source_destination_t {
    city_t city;
    company_t company;

    MSGPACK_DEFINE(city, company);

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root, bool is_special) const {
        root["city"] = Json::Value();
        city.Serialize(root["city"]);

        if (!is_special) {
            root["company"] = Json::Value();
            company.Serialize(root["company"]);
        }
    }
#endif
};
typedef source_destination_t source_t;
typedef source_destination_t destination_t;

struct position_t {
    position_t() : x(0.f), y(0.f), z(0.f), heading(0.f) {
    }

    double x;
    double y;
    double z;
    float heading;

    MSGPACK_DEFINE(x, y, z, heading);

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root) const {
        root["x"] = x;
        root["y"] = y;
        root["z"] = z;
        root["heading"] = heading;
    }
#endif
};

struct truck_t {
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

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root) const {
        root["id"] = id;
        root["name"] = name;
        root["wheels"] = Json::Value::UInt(wheels);
        root["brand"] = Json::Value();
        brand.Serialize(root["brand"]);
    }
#endif
};

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

struct job_t {
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

    struct distance_t {
        distance_t() : driven(0.f), planned(0) {
        }

        float driven;
        uint32_t planned;

        MSGPACK_DEFINE(driven, planned);
    } distance;

    struct trailer_t {
        trailer_t() : id(), accessoryId() {}

        std::string id;
        std::string accessoryId;

        MSGPACK_DEFINE(id, accessoryId);
    } trailer;

    truck_t truck;

    struct cargo_t {
        cargo_t() : id(), name(), mass(0.f), damage(0.f) {
        }

        std::string id;
        std::string name;
        float mass;
        float damage;
        MSGPACK_DEFINE(name, id, mass, damage)
    } cargo;

    source_t source;
    destination_t destination;

    MSGPACK_DEFINE(game, status, type, isSpecial, income, revenue, xp, timeSpend, maxSpeed, fuelConsumed, autoPark, autoLoad, distance, trailer, truck, cargo, source, destination);

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
                break;
        }

        root["game"] = game_name;
        root["status"] = (uint8_t)status;
        root["type"] = (uint8_t)type;
        root["isSpecial"] = isSpecial;
        root["income"] = Json::Value::UInt64(income);
        root["revenue"] = Json::Value::Int64(revenue);
        root["xp"] = Json::Value::Int(xp);
        root["time"] = Json::Value::UInt(timeSpend);
        root["maxSpeed"] = maxSpeed;
        root["fuelConsumed"] = fuelConsumed;
        root["autoPark"] = autoPark;
        root["autoLoad"] = autoLoad;

        Json::Value distanceObj;
        distanceObj["driven"] = distance.driven;
        distanceObj["planned"] = distance.planned;
        root["distance"] = distanceObj;

        Json::Value cargoObj;
        cargoObj["name"] = cargo.name;
        cargoObj["id"] = cargo.id;
        cargoObj["mass"] = cargo.mass;
        cargoObj["damage"] = cargo.damage;
        root["cargo"] = cargoObj;

        Json::Value trailerObj;
        trailerObj["id"] = trailer.id;
        trailerObj["accessoryId"] = trailer.accessoryId;
        root["trailer"] = trailerObj;

        root["truck"] = Json::Value();
        truck.Serialize(root["truck"]);

        root["source"] = Json::Value();
        source.Serialize(root["source"], isSpecial);

        root["destination"] = Json::Value();
        destination.Serialize(root["destination"], isSpecial);
    }
#endif
};

#endif //ETS2_JOB_LOGGER_PLUGINDEFS_H
