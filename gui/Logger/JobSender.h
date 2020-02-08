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
 * @file JobSender.h
 * @brief Send data to the API
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_JOBSENDER_H
#define ETS2_JOB_LOGGER_JOBSENDER_H

#include "Logger/Settings.h"
#include <jobplugin/PluginDefs.h>
#include <wx/event.h>
#include <wx/thread.h>
#ifdef _WIN32
#include <openssl/x509.h>
#endif

class JobSender : public wxThreadHelper {
public:
    JobSender(wxEvtHandler *handler, Settings *settings);
    ~JobSender() override;

    /**
     * Start job sender
     *
     * @return bool - true if started, false otherwise
     */
    bool start();

    /**
     * Stop job sender
     */
    void stop();

    /**
     * Server URL has been changed, query server capabilities again
     */
    void serverChanged();

    /**
     * Has messages in queue to be sent to API
     *
     * @return bool - true if message in queue, false otherwise
     */
    bool hasPending();

    /**
     * Add job information to send queue
     *
     * @param job
     */
    void send(const job_t &job);

    /**
     * Add truck information to send queue
     *
     * @param truck
     */
    void send(const truck_t &truck);

    /**
     * Add fine to send queue
     *
     * @param fine
     */
    void send(const fine_t &fine);

private:
    /**
     * Main loop
     *
     * @return wxThread::ExitCode
     */
    wxThread::ExitCode Entry() override;

    /**
     * Generate full URL to endpoint
     *
     * @param endpoint
     * @return std::string
     */
    std::string generate_url(const std::string &endpoint);

    /**
     * Query server of it's capabilities
     *
     * @return bool - true if query successful, false otherwise
     */
    bool query_capabilities();

    /**
     * Send job information to the API
     *
     * If sending failed, message is pushed back to queue
     *
     * @return bool - true if job was sent successfully, false otherwise
     */
    bool send_job();

    /**
     * Send truck information to the API
     *
     * Response from server is ignored and message
     * will not be sent again.
     */
    void send_truck();

    /**
     * Send received fine to the API
     */
    void send_fine();

#ifdef _WIN32
    /**
     * Add certificates from system store
     *
     * @param curl
     * @param sslctx
     * @param userdata
     * @return int - CURLE_OK
     */
    static int add_certificates(void *curl, void *sslctx, void *userdata);
#endif

    /**
     * cURL callback to receive response data
     *
     * @param contents
     * @param size
     * @param nmemb
     * @param userp
     * @return size_t - bytes written
     */
    static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userdata);

    /**
     * Wrapper to send data to the API
     *
     * @param url - Full URL
     * @param data - Data to send
     * @param response - Received response content
     * @param error - Error message
     * @return long - HTTP response code
     */
    long send_data(const std::string &url, const char *data, wxString &response, wxString &error);

    typedef wxCriticalSectionLocker LockGuard;
    wxCriticalSection m_lock;
    wxEvtHandler *m_handler;
    Settings *m_settings;

    bool m_running;
    bool m_canSend;
    bool m_sending;
    long m_job_sent_time;
    long m_fine_failed_time;
    std::vector<job_t> m_job_queue;
    std::deque<truck_t> m_truck_queue;
    std::deque<fine_t> m_fine_queue;

    struct Capabilities {
        Capabilities() : truck(false), fine(false) {
        }

        bool truck;
        bool fine;
    } m_caps;

#ifdef _WIN32
    std::vector<X509*> m_certificates;
#endif
};

#endif //ETS2_JOB_LOGGER_JOBSENDER_H
