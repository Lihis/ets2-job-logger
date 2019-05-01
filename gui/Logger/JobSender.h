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

class JobSender : public wxThreadHelper {
public:
    JobSender(wxEvtHandler *handler, Settings *settings);
    ~JobSender() override;

    /**
     * Start job sender
     *
     * @return true if started, false otherwise
     */
    bool start();

    /**
     * Stop job sender
     */
    void stop();

    /**
     * Has messages in queue to be sent to API
     *
     * @return true if message in queue, false otherwise
     */
    bool hasPending();

    /**
     * Send job to API
     *
     * @param job
     */
    void send(const job_t &job);

private:
    /**
     * Main loop
     *
     * @return
     */
    wxThread::ExitCode Entry() override;

    /**
     * Generate full URL to endpoint
     *
     * @param endpoint
     * @return
     */
    std::string generate_url(const std::string &endpoint);

    /**
     * Send job information to the API
     *
     * If sending failed, message is pushed back to queue
     * @return true if job was sent successfully, false otherwise
     */
    bool send_job();

    /**
     * Wrapper to send data to the API
     *
     * @param url - Full URL
     * @param data - Data to send
     * @param error - Error message
     * @return true if sent successfully, false otherwise
     */
    bool send_data(const std::string &url, const char *data, wxString &error);

    typedef wxCriticalSectionLocker LockGuard;
    wxCriticalSection m_lock;
    wxEvtHandler *m_handler;
    Settings *m_settings;

    bool m_running;
    bool m_sending;
    std::vector<job_t> m_job_queue;
};

#endif //ETS2_JOB_LOGGER_JOBSENDER_H
