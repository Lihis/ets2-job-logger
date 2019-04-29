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

#include "SettingsWindow.h"
#include <wx/msgdlg.h>

SettingsWindow::SettingsWindow(Settings *settings, wxWindow *parent) :
base::SettingsWindow(parent),
m_settings(settings)
{
    SetReturnCode(wxID_CANCEL);

    bool install_ets2 = !m_settings->GetETS2Path().empty();
    bool install_ats = !m_settings->GetATSPath().empty();

    if (!install_ets2 && !install_ats) {
        m_dirCtrlETS2Path->Enable(true);
    } else {
        m_dirCtrlETS2Path->Enable(install_ets2);
    }

    m_dirCtrlETS2Path->SetPath(m_settings->GetETS2Path());
    m_dirCtrlATSPath->Enable(install_ats);
    m_dirCtrlATSPath->SetPath(m_settings->GetATSPath());

    if (!m_settings->GetURL().empty()) {
        m_textCtrlURL->SetValue(m_settings->GetURL());
    }

    m_textCtrlToken->SetValue(m_settings->GetToken());
}

void SettingsWindow::on_install_ets2(wxCommandEvent &event) {
    m_dirCtrlETS2Path->Enable(m_checkBoxETS2->IsChecked());
}

void SettingsWindow::on_install_ats(wxCommandEvent &event) {
    m_dirCtrlATSPath->Enable(m_checkBoxATS->IsChecked());
}

void SettingsWindow::on_click_ok(wxCommandEvent &/*event*/) {
    if (!m_checkBoxETS2->IsChecked() && !m_checkBoxATS->IsChecked()) {
        wxMessageBox("Plugin must be installed either for ETS2 or ATS.", "Error", wxOK, this);
        return;
    }

    if (m_checkBoxETS2->IsChecked()) {
        if (m_dirCtrlETS2Path->GetPath().empty()) {
            wxMessageBox("Path to ETS2 must be selected.", "Error", wxOK, this);
            return;
        } else {
            m_settings->SetETS2Path(m_dirCtrlETS2Path->GetPath());
        }
    } else {
        m_settings->SetETS2Path("");
    }

    if (m_checkBoxATS->IsChecked()) {
        if (m_dirCtrlATSPath->GetPath().empty()) {
            wxMessageBox("Path to ATS must be selected.", "Error", wxOK, this);
            return;
        } else {
            m_settings->SetATSPath(m_dirCtrlATSPath->GetPath());
        }
    } else {
        m_settings->SetATSPath("");
    }

    if (m_textCtrlURL->GetValue().empty()) {
        wxMessageBox("URL can't be empty.", "Error", wxOK, this);
        return;
    } else {
        m_settings->SetURL(m_textCtrlURL->GetValue());
    }

    if (m_textCtrlToken->GetValue().empty()) {
        wxMessageBox("Token can't be empty.", "Error", wxOK, this);
        return;
    } else {
        m_settings->SetToken(m_textCtrlToken->GetValue());
    }

    EndDialog(wxID_OK);
}