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
#include <json/reader.h>
#ifdef _WIN32
#include <wincrypt.h>
#include <windows.h>
#include <openssl/ssl.h>
#include <openssl/x509_vfy.h>
#endif

#define API_VERSION "v1"

JobSender::JobSender(wxEvtHandler *handler, Settings *settings) :
m_handler(handler),
m_settings(settings),
m_running(false),
m_canSend(false),
m_sending(false),
m_job_sent_time(0L)
{
    curl_global_init(CURL_GLOBAL_ALL);

#ifdef _WIN32
    std::vector<std::wstring> stores = {std::wstring(L"CA"), std::wstring(L"AuthRoot"), std::wstring(L"ROOT")};

    for (const auto& store : stores) {
        HCERTSTORE storeHandle = CertOpenSystemStore(NULL, store.c_str());
        if (!storeHandle) {
            continue;
        }

        PCCERT_CONTEXT windowsCertificate = CertEnumCertificatesInStore(storeHandle, nullptr);
        while (windowsCertificate != nullptr) {
            X509 *opensslCertificate = d2i_X509(nullptr, const_cast<unsigned char const **>(&windowsCertificate->pbCertEncoded), windowsCertificate->cbCertEncoded);
            if (opensslCertificate) {
                m_certificates.push_back(opensslCertificate);
            }
            windowsCertificate = CertEnumCertificatesInStore(storeHandle, windowsCertificate);
        }
        CertCloseStore(storeHandle, 0);
    }
#endif
}

JobSender::~JobSender() {
    stop();
    curl_global_cleanup();
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
    if (m_running) {
        m_running = false;
        if (GetThread()->IsRunning()) {
            GetThread()->Wait();
        }
    }
}

void JobSender::serverChanged() {
    m_canSend = false;
    m_job_sent_time = 0L;
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

void JobSender::send(const truck_t &truck) {
    if (m_caps.truck) {
        LockGuard lock(m_lock);
        m_truck_queue.push_back(truck);
    }
}

wxThread::ExitCode JobSender::Entry() {
    Json::Value json;
    wxString error;
    bool job_send_failed = false;

    while (m_running) {
        if (!m_canSend && (wxGetUTCTime() - m_job_sent_time) > 30) {
            m_canSend = query_capabilities();
            if (!m_canSend) {
                m_job_sent_time = wxGetUTCTime();
                continue;
            }
        }

        if ((wxGetUTCTime() - m_job_sent_time) > (job_send_failed ? 30 : 5)) {
            job_send_failed = !send_job();
            m_job_sent_time = wxGetUTCTime();
        }

        send_truck();

        if (GetThread()->TestDestroy()) {
            m_running = false;
        }
        wxMicroSleep(100000);
    }

    return static_cast<wxThread::ExitCode>(nullptr);
}

std::string JobSender::generate_url(const std::string &endpoint) {
    std::string url(m_settings->GetURL().append(API_VERSION));
    url.append("/" + endpoint);

    return url;
}

bool JobSender::query_capabilities() {
    std::string url = generate_url("capabilities");
    wxString response;
    wxString error;

    if (send_data(url, "", response, error) != 200L) {
        return false;
    }

    Json::Value json;
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    if (!reader->parse(response.c_str(), response.c_str() + response.length(), &json, nullptr)) {
        return false;
    }

    m_caps.truck = json.get("truck", false).asBool();

    return true;
}

bool JobSender::send_job() {
    {
        LockGuard lock(m_lock);
        if (m_job_queue.empty()) {
            return true;
        }
    }

    job_t job;
    std::string url = generate_url("job");
    Json::Value json;
    wxString response;
    wxString error;

    {
        LockGuard lock(m_lock);
        job = m_job_queue.back();
        m_job_queue.pop_back();
    }

    job.Serialize(json);

    long code = send_data(url, json.toStyledString().c_str(), response, error);
    if (code != 200L) {
        auto event = new wxThreadEvent(wxEVT_COMMAND_THREAD, wxID_ABORT);
        event->SetString("API returned error code: " + std::to_string(code) + " " + error);
        wxQueueEvent(m_handler, event);
        LockGuard lock(m_lock);
        m_job_queue.emplace_back(job);
    } else {
        wxQueueEvent(m_handler, new wxThreadEvent(wxEVT_COMMAND_THREAD, wxID_OK));
    }

    return (code == 200L);
}

void JobSender::send_truck() {
    truck_t truck;
    std::string url = generate_url("truck");
    Json::Value json;
    wxString response;
    wxString error;

    {
        LockGuard lock(m_lock);
        if (m_truck_queue.empty()) {
            return;
        }
        truck = m_truck_queue.front();
        m_truck_queue.pop_front();
    }

    json["speed"] = truck.speed;
    truck.position.Serialize(json);

    send_data(url, json.toStyledString().c_str(), response, error);
}

#ifdef _WIN32
#pragma warning(suppress: 4138)
int JobSender::add_certificates(void */*curl*/, void *sslctx, void *userdata) {
    auto certStore = SSL_CTX_get_cert_store(reinterpret_cast<SSL_CTX *>(sslctx));
    auto obj = static_cast<JobSender *>(userdata);

    for (const auto cert : obj->m_certificates) {
        X509_STORE_add_cert(certStore, cert);
    }

    return CURLE_OK;
}
#endif

size_t JobSender::write_callback(void *contents, size_t size, size_t nmemb, void *userdata) {
    auto buffer = static_cast<wxString *>(userdata);
    size_t realSize = size * nmemb;

    buffer->append((char*)contents, realSize);

    return realSize;
}

long JobSender::send_data(const std::string &url, const char *data, wxString &response, wxString &error) {
    long ret = 0;
    CURL *curl;
    CURLcode res;

    {
        LockGuard lock(m_lock);
        m_sending = true;
    }

    response.clear();
    error.clear();

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
#ifdef _WIN32
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void *)this);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, add_certificates);
#endif
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        ret = false;
        error = curl_easy_strerror(res);
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ret);
    if (ret == 500L) {
        error = "Internal Server Error";
    }

    curl_easy_cleanup(curl);

    LockGuard lock(m_lock);
    m_sending = false;

    return ret;
}
