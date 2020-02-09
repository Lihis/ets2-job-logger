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

#include "Logger.h"
#include <cmath>
#include <msgpack.hpp>
#include <scs/common/scssdk_telemetry_common_configs.h>
#include <scs/common/scssdk_telemetry_common_gameplay_events.h>
#ifdef _WIN32
#include <windows.h>
#include <memoryapi.h>
#include <winbase.h>
#endif

Logger::Logger(const Game &game) :
m_game(game),
m_paused(true),
m_job(game),
m_truckLastSent(std::chrono::steady_clock::now())
{
    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.clear_error_channels(websocketpp::log::alevel::all);

    m_server.init_asio();
    m_server.set_reuse_addr(true);

    m_server.set_open_handler(websocketpp::lib::bind(&Logger::on_open, this, websocketpp::lib::placeholders::_1));
    m_server.set_close_handler(websocketpp::lib::bind(&Logger::on_close, this, websocketpp::lib::placeholders::_1));
}

Logger::~Logger() {
    stop();
}

void Logger::start() {
    LockGuard lock(m_lock);

    if (!m_server.is_listening()) {
        m_server.listen(WEBSOCK_PORT);
        m_server.start_accept();
        m_thread = std::thread(&Logger::run, this);
    }
}

void Logger::stop() {
    LockGuard lock(m_lock);

    if (m_server.is_listening()) {
        m_server.stop_listening();
        m_server.stop();
        m_thread.join();
    }
}

SCSAPI_VOID Logger::odometer(const scs_value_t *const km) {
    m_job.truck.odometer = km ? km->value_float.value : 0.f;
}

SCSAPI_VOID Logger::fuel(const scs_value_t *const liters) {
    m_job.truck.fuel = liters ? liters->value_float.value : 0.f;
}

SCSAPI_VOID Logger::speed(const scs_value_t *const speed) {
    m_job.truck.speed = speed ? speed->value_float.value : 0.f;
}

SCSAPI_VOID Logger::truckPlacement(const scs_value_t *const placement) {
    if (!placement) {
        return;
    }

    scs_value_dvector_t position = placement->value_dplacement.position;
    m_job.truck.position.x = position.x;
    m_job.truck.position.y = position.y;
    m_job.truck.position.z = position.z;

    scs_value_euler_t orientation = placement->value_dplacement.orientation;
    m_job.truck.position.heading = orientation.heading;
}

SCSAPI_VOID Logger::cargoDamage(const scs_value_t *const damage) {
    float prev = m_job.cargo.damage;
    float cur = damage ? damage->value_float.value : 0.f;
    auto epsilon = std::numeric_limits<float>::epsilon();

    if (std::fabs(prev - cur) > epsilon * std::max(std::fabs(prev), std::fabs(cur))) {
        std::stringstream buffer;
        msgpack::pack(buffer, PacketType::CargoDamage);
        msgpack::pack(buffer, cur);
        send(buffer.str());
    }
}

SCSAPI_VOID Logger::frameEnd(const void *const /*event_info*/) {
    if (m_paused) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_truckLastSent).count() >= 1000) {
        send_truck_info();
        m_truckLastSent = now;
    }
}

SCSAPI_VOID Logger::eventPaused(const void *const /*event_info*/) {
    m_paused = true;
}

SCSAPI_VOID Logger::eventStarted(const void *const /*event_info*/) {
    m_paused = false;
}

SCSAPI_VOID Logger::configuration(const scs_telemetry_configuration_t *event_info) {
    std::string event_id(event_info->id);

    if (event_id == SCS_TELEMETRY_CONFIG_truck) {
        if (!event_info->attributes->name) {
            return;
        }

        for (auto attr = event_info->attributes; attr->name; attr++) {
            LockGuard lock(m_lock);
            std::string name(attr->name);

            if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_id) {
                m_job.truck.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_name) {
                m_job.truck.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count) {
                m_job.truck.wheels = attr->value.value_u32.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand) {
                m_job.truck.brand.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id) {
                m_job.truck.brand.id = attr->value.value_string.value;
            }
        }
        m_shouldSend++;
    } else if (event_id == SCS_TELEMETRY_CONFIG_trailer) {
        if (!event_info->attributes->name) {
            return;
        }

        for (auto attr = event_info->attributes; attr->name; attr++) {
            LockGuard lock(m_lock);
            std::string name(attr->name);

            if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_id) {
                m_job.trailer.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id) {
                m_job.trailer.accessoryId = attr->value.value_string.value;
            }
        }
        m_shouldSend++;
    } else if (event_id == SCS_TELEMETRY_CONFIG_job) {
        if (!event_info->attributes->name) {
            return;
        }

        for (auto attr = event_info->attributes; attr->name; attr++) {
            LockGuard lock(m_lock);
            std::string name(attr->name);

            if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo) {
                m_job.cargo.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id) {
                m_job.cargo.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass) {
                m_job.cargo.mass = attr->value.value_float.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city) {
                m_job.source.city.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city_id) {
                m_job.source.city.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city) {
                m_job.destination.city.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city_id) {
                m_job.destination.city.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company) {
                m_job.source.company.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company_id) {
                m_job.source.company.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company) {
                m_job.destination.company.name = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company_id) {
                m_job.destination.company.id = attr->value.value_string.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_income) {
                m_job.income = attr->value.value_u64.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_planned_distance_km) {
                m_job.distance.planned = attr->value.value_u32.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_special_job) {
                m_job.isSpecial = attr->value.value_bool.value;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_job_market) {
                std::string type(attr->value.value_string.value);
                if (type == "cargo_market") {
                    m_job.type = JobType::CargoMarket;
                } else if (type == "quick_job") {
                    m_job.type = JobType::QuickJob;
                } else if (type == "freight_market") {
                    m_job.type = JobType::FreightMarket;
                } else if (type == "external_contracts") {
                    m_job.type = JobType::ExternalContract;
                } else if (type == "external_market") {
                    m_job.type = JobType::ExternalMarket;
                }
            }
        }

        if (m_job.status == JobStatus::FreeAsWind) {
            m_shouldSend++;
            m_job.status = JobStatus::OnJob;
        }
    }

    if (m_shouldSend >= 3) {
        send_job();
    }
}

SCSAPI_VOID Logger::gameplay(const scs_telemetry_gameplay_event_t *event_info) {
    std::string event_id(event_info->id);

    if (event_id == SCS_TELEMETRY_GAMEPLAY_EVENT_job_cancelled) {
        m_job.status = JobStatus::Cancelled;
    } else if (event_id == SCS_TELEMETRY_GAMEPLAY_EVENT_job_delivered) {
        m_job.status = JobStatus::Delivered;
    } else {
        return;
    }

    for (auto attr = event_info->attributes; attr->name; attr++) {
        std::string name(attr->name);

        if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_revenue) {
            m_job.revenue = attr->value.value_s64.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_earned_xp) {
            m_job.xp = attr->value.value_s32.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_cargo_damage) {
            m_job.cargo.damage = attr->value.value_float.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_distance_km) {
            m_job.distance.driven = attr->value.value_float.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_delivery_time) {
            m_job.timeSpend = attr->value.value_u32.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_auto_park_used) {
            m_job.autoPark = attr->value.value_bool.value;
        } else if (name == SCS_TELEMETRY_GAMEPLAY_EVENT_ATTRIBUTE_auto_load_used) {
            m_job.autoLoad = attr->value.value_bool.value;
        }
    }

    if (m_job.status != JobStatus::FreeAsWind && m_job.status != JobStatus::OnJob) {
        send_job();
        m_job = job_t(m_game);
    }
}

void Logger::run() {
    m_server.run();
}

void Logger::send_version() {
    version_t version(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_PATCH);
    std::stringstream buffer;
    msgpack::pack(buffer, PacketType::Version);
    msgpack::pack(buffer, version);

    LockGuard lock(m_lock);
    send(buffer.str());
}

void Logger::send_job() {
    LockGuard lock(m_lock);
    m_shouldSend = 0;

    std::stringstream buffer;
    msgpack::pack(buffer, PacketType::Job);
    msgpack::pack(buffer, m_job);

    send(buffer.str());
}

void Logger::send_truck_info() {
    LockGuard lock(m_lock);

    std::stringstream buffer;
    msgpack::pack(buffer, PacketType::Truck);
    msgpack::pack(buffer, m_job.truck);

    send(buffer.str());
}

void Logger::send(const std::string &data) {
    std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>>::iterator it;

    for (it = m_connections.begin(); it != m_connections.end(); ++it) {
        try {
            m_server.send(*it, data, websocketpp::frame::opcode::binary);
        } catch (const std::exception &/*exception*/) {
        }
    }
}

void Logger::on_open(const websocketpp::connection_hdl &hdl) {
    {
        LockGuard lock(m_lock);
        m_connections.insert(hdl);
    }
    send_version();
    send_job();
}

void Logger::on_close(const websocketpp::connection_hdl &hdl) {
    LockGuard lock(m_lock);
    m_connections.erase(hdl);
}
