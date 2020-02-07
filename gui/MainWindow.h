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
 * @file MainWindow.h
 * @brief Applications main window.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_MAINWINDOW_H
#define ETS2_JOB_LOGGER_MAINWINDOW_H

#include "base/MainWindow.h"
#include "Logger/Settings.h"
#include "Logger/JobSender.h"
#include <jobplugin/PluginDefs.h>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

class MainWindow : public base::MainWindow {
public:
    explicit MainWindow(Settings *settings);
    ~MainWindow() override;

    /**
     * Start websocket client and job sender
     *
     * @return
     */
    bool start();

    /**
     * Stop websocket client and job sender
     */
    void stop();

    /**
     * Can application close, i.e., is there still jobs to be sent to API
     */
    bool can_close();

    /**
     * Inform JobSender of API URL change
     */
    void server_changed();

private:
    typedef std::lock_guard<std::mutex> LockGuard;
    typedef websocketpp::client<websocketpp::config::asio_client> WebsocketClient;
    typedef websocketpp::config::asio_client::message_type::ptr WebsocketMessage;

    /**
     * Callback from job sender (eg. API call failed)
     *
     * @param event
     */
    void on_sender_update(wxThreadEvent &event);

    /**
     * Update job labels
     *
     * @param event
     */
    void on_job_update(wxCommandEvent &event);

    /**
     * Show about dialog
     *
     * @param event
     */
    void on_about(wxCommandEvent &event);

    /**
     * Close window event
     *
     * @param event
     */
    void on_close(wxCloseEvent &event) final;

    /**
     * Websocket client main loop
     */
    void socket_run();

    /**
     * Try to connect plugin
     */
    void socket_connect();

    /**
     * Websocket connection to plugin failed
     */
    void socket_on_fail();

    /**
     * Websocket message received
     *
     * @param hdl
     * @param msg
     */
    void socket_on_message(const websocketpp::connection_hdl &hdl,
                           const WebsocketMessage &msg);

    /**
     * Websocket connection closed
     */
    void socket_on_close();

    JobSender *m_sender;
    Settings *m_settings;
    job_t m_job;
    float m_odometerOnStart;
    float m_fuelOnStart;

    bool m_socket_running;
    std::mutex m_socket_lock;
    std::thread m_socket_thread;
    WebsocketClient m_client;
};

#endif //ETS2_JOB_LOGGER_MAINWINDOW_H
