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

#include <jobplugin/Job.h>

Json::Value job_t::ToJson() const {
    Json::Value root;
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
    root["distance"] = distance.ToJson();
    root["cargo"] = cargo.ToJson();
    root["trailer"] = trailer.ToJson();
    root["truck"] = truck.ToJson();
    root["source"] = source.ToJson();
    root["destination"] = destination.ToJson();

    return root;
}

std::string job_t::ToString() const {
    return serializable_t::ToString(this);
}