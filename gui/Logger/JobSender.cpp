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

#include "JobSender.h"
#include <wx/time.h>
#include <curl/curl.h>
#include <sstream>

JobSender::JobSender(wxEvtHandler *handler, Settings *settings) :
m_handler(handler),
m_settings(settings),
m_running(false),
m_sending(false)
{
}

JobSender::~JobSender() {
    stop();
}

bool JobSender::start() {
    if (m_running) {
        return true;
    }

    if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
        return false;
    }

    if (GetThread()->Run() == wxTHREAD_NO_ERROR) {
        m_running = true;
    }

    return m_running;
}

void JobSender::stop() {
    LockGuard lock(m_lock);
    m_running = false;
    if (GetThread()->IsRunning()) {
        GetThread()->Wait();
    }
}

bool JobSender::hasPending() {
    bool pending = !m_lock.TryEnter();
    if (!pending) {
        pending = (!m_job_queue.empty() || m_sending);
        m_lock.Leave();
    }

    return pending;
}

void JobSender::send(const job_t &job) {
    LockGuard lock(m_lock);
    m_job_queue.push_back(job);
}

wxThread::ExitCode JobSender::Entry() {
    Json::Value json;
    wxString error;
    wxLongLong job_sent_time = 0;
    bool job_send_failed = false;

    while (m_running) {
        if ((wxGetUTCTime() - job_sent_time) > (job_send_failed ? 30 : 5)) {
            job_send_failed = !send_job();
            job_sent_time = wxGetUTCTime();
        }

        if (GetThread()->TestDestroy()) {
            m_running = false;
        }
        wxMicroSleep(100000);
    }

    return static_cast<wxThread::ExitCode>(nullptr);
}

bool JobSender::send_job() {
    bool ret;
    {
        LockGuard lock(m_lock);
        if (m_job_queue.empty()) {
            return true;
        }
    }

    job_t job;
    std::string url(m_settings->GetURL().mbc_str() + std::string("job"));
    Json::Value json;
    wxString error;

    {
        LockGuard lock(m_lock);
        m_sending = true;
        job = m_job_queue.back();
        m_job_queue.pop_back();
    }

    job.Serialize(json);

    ret = send_data(url, json.toStyledString().c_str(), error);
    if (!ret) {
        auto event = new wxThreadEvent(wxEVT_COMMAND_THREAD, wxID_ABORT);
        event->SetString(error);
        wxQueueEvent(m_handler, event);
        LockGuard lock(m_lock);
        m_job_queue.emplace_back(job);
    } else {
        wxQueueEvent(m_handler, new wxThreadEvent(wxEVT_COMMAND_THREAD, wxID_OK));
    }

    LockGuard lock(m_lock);
    m_sending = false;

    return ret;
}

bool JobSender::send_data(const std::string &url, const char *data, wxString &error) {
    bool ret = false;
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return ret;
    }

    struct curl_slist *header = nullptr;
    wxString token = m_settings->GetToken();
    header = curl_slist_append(header, wxString("Authorization-token: " + token).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        ret = false;
        error = curl_easy_strerror(res);
    } else {
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code != 200) {
            ret = false;
            error = "API returned error code: " + std::to_string(response_code);
        } else {
            ret = true;
        }
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return ret;
}
