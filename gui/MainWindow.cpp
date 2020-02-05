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

#include "MainWindow.h"
#include "Version.h"
#include <msgpack.hpp>
#include <wx/msgdlg.h>
#include <wx/notifmsg.h>
#include <wx/aboutdlg.h>
#ifdef _WIN32
#include <synchapi.h>
#endif

MainWindow::MainWindow(Settings *settings) :
base::MainWindow(nullptr),
m_sender(nullptr),
m_settings(settings),
m_job(Game::Unknown),
m_odometerOnStart(-1.f),
m_fuelOnStart(-1.f),
m_socket_running(false)
{
    m_client.clear_access_channels(websocketpp::log::alevel::all);
    m_client.clear_error_channels(websocketpp::log::alevel::all);
    m_client.init_asio();
    m_client.start_perpetual();

    m_client.set_fail_handler(websocketpp::lib::bind(&MainWindow::socket_on_fail, this));
    m_client.set_message_handler(websocketpp::lib::bind(&MainWindow::socket_on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    m_client.set_close_handler(websocketpp::lib::bind(&MainWindow::socket_on_close, this));

    m_sender = new JobSender(this, m_settings);

    Connect(wxID_ABOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::on_about));
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindow::on_close));
    Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MainWindow::on_job_update));
    Bind(wxEVT_THREAD, &MainWindow::on_sender_update, this);
}

MainWindow::~MainWindow() {
    if (m_socket_running) {
        stop();
    }

    if (m_sender) {
        m_sender->stop();
    }
}

void MainWindow::on_job_update(wxCommandEvent &event) {
    const auto type = static_cast<PacketType>(event.GetInt());

    if (type == PacketType::Job) {
        m_lblCargo->SetLabel(wxString::FromUTF8(m_job.cargo.name.c_str()));
        if (m_job.status != JobStatus::FreeAsWind) {
            wxString origin(wxString::FromUTF8(m_job.source.city.name.c_str()));
            wxString sender(wxString::FromUTF8(m_job.source.company.name.c_str()));
            m_lblOrigin->SetLabel(origin + (sender.empty() ? "" : " (" + sender + ")"));

            wxString destination(wxString::FromUTF8(m_job.destination.city.name.c_str()));
            wxString receiver(wxString::FromUTF8(m_job.destination.company.name.c_str()));
            m_lblDestination->SetLabel(destination + (receiver.empty() ? "" : " (" + receiver + ")"));
        } else {
            m_lblOrigin->SetLabel("-");
            m_lblDestination->SetLabel("-");
        }
        m_lblMass->SetLabel(std::to_string(static_cast<uint32_t>(m_job.cargo.mass / 1000)) + " T");
        m_lblIncome->SetLabel(wxString(std::to_string(m_job.income)).append(wxString::FromUTF8(" €")));
    }

    if (type == PacketType::Job || type == PacketType::CargoDamage) {
        // TODO: Game seems to "forgive" damage if it's under 1%?
        std::stringstream damage;
        damage.precision(1);
        damage << std::fixed << (m_job.cargo.damage * 100.f);
        m_lblCargoDamage->SetLabel(damage.str() + " %");
    }

    if (type == PacketType::Job || type == PacketType::Truck) {
        m_lblDistanceDriven->SetLabel(std::to_string(static_cast<uint32_t>(m_job.distance.driven)) + " KM");
        m_lblFuelConsumed->SetLabel(std::to_string(static_cast<uint32_t>(m_job.fuelConsumed)) + " L");
    }
}

void MainWindow::on_sender_update(wxThreadEvent &event) {
    wxNotificationMessage notificationMessage("ETS2 Job Logger", "", this);

    if (event.GetId() == wxID_ABORT) {
        notificationMessage.SetMessage("Failed to send the job. (" + event.GetString() + ")");
    } else {
        notificationMessage.SetMessage("Job sent successfully!");
    }

    notificationMessage.Show(wxNotificationMessage::Timeout_Auto);
}

bool MainWindow::can_close() {
    bool ret;

    if (m_sender->hasPending()) {
        wxString message("There is still pending jobs to send. Are you sure you want to quit?");
        ret = (wxMessageBox(message, "Warning", wxYES_NO, nullptr) == wxYES);
    } else {
        ret = true;
    }

    return ret;
}

void MainWindow::server_changed() {
    m_sender->serverChanged();
}

void MainWindow::on_about(wxCommandEvent &event) {
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("ETS2 Job Logger");
    aboutInfo.SetVersion(APP_VERSION_FULL);
    aboutInfo.SetDescription("Euro Truck Simulator 2 Job Logger");
    aboutInfo.SetCopyright(wxString::FromUTF8("(c) Tomi Lähteenmäki 2019"));
    aboutInfo.SetLicence("This program is free software; you can redistribute it and/or modify\n"
                         "it under the terms of the GNU General Public License as published by\n"
                         "the Free Software Foundation; either version 2 of the License, or\n"
                         "(at your option) any later version.\n"
                         "\n"
                         "This program is distributed in the hope that it will be useful,\n"
                         "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                         "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                         "GNU General Public License for more details.\n"
                         "\n"
                         "You should have received a copy of the GNU General Public License\n"
                         "along with this program; if not, write to the Free Software\n"
                         "Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,\n"
                         "MA 02110-1301, USA.");
    aboutInfo.SetWebSite("https://github.com/Lihis/ets2-job-logger", "Website");
    aboutInfo.AddDeveloper(wxString::FromUTF8("Tomi Lähteenmäki"));

    wxAboutBox(aboutInfo, this);
}

void MainWindow::on_close(wxCloseEvent &event) {
    if (m_settings->GetRunInBackground()) {
        Hide();
        event.Veto();
        return;
    }

    if (can_close()) {
        stop();
        Destroy();
    } else {
        event.Veto();
    }
}

bool MainWindow::start() {
    LockGuard lock(m_socket_lock);

    if (!m_sender->start()) {
        wxMessageBox("Failed to create job sender.", "Error", wxOK, this);
        return false;
    }

    if (!m_socket_running) {
        m_socket_thread = std::thread(&MainWindow::socket_run, this);
    }

    socket_connect();

    return true;
}

void MainWindow::stop() {
    LockGuard lock(m_socket_lock);

    if (m_socket_running) {
        m_socket_running = false;
        m_client.stop();
        m_socket_thread.join();
    }

    if (m_sender) {
        m_sender->stop();
        delete m_sender;
        m_sender = nullptr;
    }
}

void MainWindow::socket_run() {
    m_socket_running = true;

    while (m_socket_running) {
        m_client.run();
    }

    m_socket_running = false;
}

void MainWindow::socket_connect() {
    wxSleep(1);
    websocketpp::lib::error_code ec;
    m_connection = m_client.get_connection("ws://127.0.0.1:" + std::to_string(WEBSOCK_PORT), ec);
    if (!ec) {
        m_client.connect(m_connection);
    }
}

void MainWindow::socket_on_fail() {
    m_job = job_t();
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED);
    event.SetInt(static_cast<int>(PacketType::Job));
    AddPendingEvent(event);
    socket_connect();
}

void MainWindow::socket_on_message(const websocketpp::connection_hdl &hdl,
                                   const WebsocketMessage &msg) {
    PacketType packetType;
    std::size_t offset = 0;
    {
        auto oh = msgpack::unpack(msg->get_payload().c_str(), msg->get_payload().size(), offset);
        packetType = oh.get().convert();
    }
    auto oh = msgpack::unpack(msg->get_payload().c_str(), msg->get_payload().size(), offset);
    const auto &obj = oh.get();

    if (packetType == PacketType::Job) {
        float maxSpeed = m_job.maxSpeed;
        float fuelConsumed = m_job.fuelConsumed;

        m_job = obj.as<job_t>();

        if (m_job.status != JobStatus::FreeAsWind) {
            m_job.maxSpeed = maxSpeed;
            m_job.fuelConsumed = fuelConsumed;
            m_sender->send(m_job);
        }

        if (m_job.status == JobStatus::Cancelled || m_job.status == JobStatus::Delivered) {
            m_job = job_t(m_job.game);
            m_odometerOnStart = -1.f;
            m_fuelOnStart = -1.f;
        }
    } else if (packetType == PacketType::Truck) {
        truck_t truck = obj.as<truck_t>();

        if (m_job.status == JobStatus::OnJob) {
            if (truck.odometer >= 0.f && m_odometerOnStart > -0.9f) {
                m_job.distance.driven = (truck.odometer - m_odometerOnStart);
            } else if (truck.odometer >= 0.f && m_odometerOnStart < 0.f) {
                m_odometerOnStart = truck.odometer;
            }

            if (truck.fuel >= 0.f && m_fuelOnStart > -0.9f) {
                m_job.fuelConsumed = (m_fuelOnStart - truck.fuel);
            } else if (truck.fuel >= 0.f && m_fuelOnStart < 0.f) {
                m_fuelOnStart = truck.fuel;
            }

            if (std::abs(truck.speed) > std::abs(m_job.maxSpeed)) {
                m_job.maxSpeed = truck.speed;
            }
        }
        m_sender->send(truck);
    } else if (packetType == PacketType::Version) {
        version_t version = obj.as<version_t>();
        // FIXME: Show error of mismatching plugin version
        if (version.major != PLUGIN_VERSION_MAJOR && version.minor != PLUGIN_VERSION_MINOR) {
            m_client.close(hdl, websocketpp::close::status::unsupported_data, "");
        }
    } else if (packetType == PacketType::CargoDamage) {
        m_job.cargo.damage = obj.as<float>();
    } else if (packetType == PacketType::Unknown) {
        return;
    }

    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED);
    event.SetInt(static_cast<int>(packetType));
    AddPendingEvent(event);
}

void MainWindow::socket_on_close() {
    m_job = job_t();
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED);
    event.SetInt(static_cast<int>(PacketType::Job));
    AddPendingEvent(event);
    socket_connect();
}
