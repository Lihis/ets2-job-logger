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
#ifdef _WIN32
#include <windows.h>
#include <memoryapi.h>
#include <winbase.h>
#endif

Logger::Logger() :
m_last_sent(0),
m_truck(),
m_job(),
m_odometerOnStart(-1.f)
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
    m_truck.odometer = km ? km->value_float.value : -1.f;

    if (m_job.onJob && (m_odometerOnStart < 0.f)) {
        m_odometerOnStart = m_truck.odometer;
    }

    if (m_job.onJob && !m_job.delivered) {
        m_job.drivenKm = (m_truck.odometer - m_odometerOnStart);
    }
}

SCSAPI_VOID Logger::fuel(const scs_value_t *const liters) {
    float fuel = liters ? liters->value_float.value : -1.f;

    if (m_odometerOnStart < 0.f) {
        m_truck.fuel = fuel;
        return;
    }

    if (m_job.onJob && !m_job.delivered) {
        if (fuel < m_truck.fuel) {
            m_job.fuelConsumed += std::fabs(m_truck.fuel - fuel);
        }
    }
    m_truck.fuel = fuel;
}

SCSAPI_VOID Logger::trailerConnected(const scs_value_t *const connected) {
    m_job.trailer.connected = connected ? connected->value_bool.value : false;
}

SCSAPI_VOID Logger::trailerDamage(const scs_value_t *const damage) {
    m_job.trailer.damage = damage ? (damage->value_float.value * 100.f) : 0.f;
}

SCSAPI_VOID Logger::frameStart(const scs_telemetry_frame_start_t *event_info) {
    scs_timestamp_t timestamp = event_info->paused_simulation_time;

    if (event_info->flags == SCS_TELEMETRY_FRAME_START_FLAG_timer_restart) {
        m_last_sent = 1000000;
    }

    if (!m_job.onJob || ((timestamp - m_last_sent) < 1000000)) {
        return;
    }

    send_job_partial();
    m_last_sent = timestamp;
}

SCSAPI_VOID Logger::configuration(const scs_telemetry_configuration_t *event_info) {
    std::string event_id(event_info->id);

    if (event_id == SCS_TELEMETRY_CONFIG_job) {
        uint8_t found = 0;

        for (auto attr = event_info->attributes; attr->name; attr++) {
            LockGuard lock(m_lock);
            std::string name(attr->name);

            if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo) {
                m_job.cargo.name = attr->value.value_string.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass) {
                m_job.cargo.mass = attr->value.value_float.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city) {
                m_job.source.city = attr->value.value_string.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city) {
                m_job.destination.city = attr->value.value_string.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company) {
                m_job.source.company = attr->value.value_string.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company) {
                m_job.destination.company = attr->value.value_string.value;
                found++;
            } else if (name == SCS_TELEMETRY_CONFIG_ATTRIBUTE_income) {
                m_job.income = attr->value.value_u64.value;
                found++;
            }
        }

        bool onJob = (found == 7);
        if (m_job.onJob && onJob) {
            m_odometerOnStart = -1.f;
            m_truck.fuel = -1.f;
            m_job.drivenKm = 0.f;
            m_job.fuelConsumed = 0.f;
        }
        m_job.onJob = onJob;
        m_job.delivered = (found == 0);

        send_job();

        if (m_job.delivered) {
            m_job = {};
            m_odometerOnStart = -1.f;
        }
    }
}

void Logger::run() {
    m_server.run();
}

void Logger::send_version() {
    version_t version(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_PATCH);
    std::stringstream buffer;
    msgpack::pack(buffer, JobPlugin::Version);
    msgpack::pack(buffer, version);

    LockGuard lock(m_lock);
    send(buffer.str());
}

void Logger::send_job() {
    LockGuard lock(m_lock);

    std::stringstream buffer;
    msgpack::pack(buffer, JobPlugin::Job);
    msgpack::pack(buffer, m_job);

    send(buffer.str());
}

void Logger::send_job_partial() {
    LockGuard lock(m_lock);

    std::stringstream buffer;
    msgpack::pack(buffer, JobPlugin::JobPartial);
    msgpack::pack(buffer, job_partial_t(m_job.drivenKm, m_job.fuelConsumed, m_job.trailer.damage));

    send(buffer.str());
}

void Logger::send(const std::string &data) {
    std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>>::iterator it;

    for (it = m_connections.begin(); it != m_connections.end(); ++it) {
        try {
            m_server.send(*it, data, websocketpp::frame::opcode::binary);
        } catch (const std::exception &exception) {
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
