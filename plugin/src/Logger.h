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
 * @file Logger.h
 * @brief Send data received via callbacks to application
 * @details Sends data over WebSocket to the connected application
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_PLUGIN_LOGGER_H
#define ETS2_JOB_LOGGER_PLUGIN_LOGGER_H

#include "jobplugin/PluginDefs.h"
#include <scs/scssdk_value.h>
#include <scs/scssdk_telemetry_event.h>
#include <thread>
#include <set>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

class Logger {
public:
    Logger(const Game &game);
    ~Logger();

    /**
     * Start websocket server
     */
    void start();

    /**
     * Stop websocket server
     */
    void stop();

    /**
     * Truck odometer callback
     *
     * @param km
     */
    SCSAPI_VOID odometer(const scs_value_t *const km);

    /**
     * Truck fuel callback
     *
     * @param liters
     */
    SCSAPI_VOID fuel(const scs_value_t *const liters);

    /**
     * Truck speed callback
     *
     * @param speed
     */
    SCSAPI_VOID speed(const scs_value_t *const speed);

    /**
     * Truck placement callback
     *
     * @param placement
     */
    SCSAPI_VOID truckPlacement(const scs_value_t *const placement);

    /**
     * Cargo damage callback
     *
     * Damage is not actually updated in job_t structure as it will be received
     * anyway when job has been delivered or cancelled. Damage is only sent to
     * the GUI for display purposes.
     *
     * @param damage
     */
    SCSAPI_VOID cargoDamage(const scs_value_t *const damage);

    /**
     * Frame end callback
     *
     * @param event_info
     */
    SCSAPI_VOID frameEnd(const void *const event_info);

    /**
     * Telemetry paused callback
     *
     * @param event_info
     */
    SCSAPI_VOID eventPaused(const void *const event_info);

    /**
     * Telemetry started callback
     *
     * @param event_info
     */
    SCSAPI_VOID eventStarted(const void *const event_info);

    /**
     * Configuration callback
     *
     * @param event_info
     */
    SCSAPI_VOID configuration(const scs_telemetry_configuration_t *event_info);

    /**
     * Gameplay callback
     *
     * @param event_info
     */
    SCSAPI_VOID gameplay(const scs_telemetry_gameplay_event_t *event_info);

private:
    /**
     * Websocket server mainloop
     */
    void run();

    /**
     * Send plugin version
     */
    void send_version();

    /**
     * Send job information
     */
    void send_job();

    /**
     * Send truck information
     */
    void send_truck_info();

    /**
     * Send data via websocket
     * @param data
     */
    void send(const std::string &data);

    /**
     * Callback for websocket client connected
     *
     * @param hdl
     */
    void on_open(const websocketpp::connection_hdl &hdl);

    /**
     * Callback for websocket client disconnected
     *
     * @param hdl
     */
    void on_close(const websocketpp::connection_hdl &hdl);

    typedef std::lock_guard<std::mutex> LockGuard;
    std::mutex m_lock;
    std::thread m_thread;

    websocketpp::server<websocketpp::config::asio> m_server;
    std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>> m_connections;

    Game m_game;
    bool m_paused;
    uint8_t m_shouldSend;
    job_t m_job;

    std::chrono::steady_clock::time_point m_truckLastSent;
};


#endif //ETS2_JOB_LOGGER_PLUGIN_LOGGER_H
