/****************************************************************************
 * Copyright (c) 2020 Tomi Lähteenmäki <lihis@lihis.net>                    *
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

#include "AboutDialog.h"
#include "Version.h"

AboutDialog::AboutDialog(wxWindow *parent) : base::AboutDialog(parent) {
#ifdef __linux__
    auto icon = wxArtProvider::GetIcon("ets2-job-logger", wxART_OTHER, wxSize(128, 128));
    if (icon.IsOk()) {
        m_bitmapIcon->SetIcon(icon);
    }
#elif _WIN32
    m_bitmapIcon->SetIcon(wxIcon("APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
#else
    // TODO: Set AboutDialog logo in MacOS
#warning "About dialog icon is not being set"
#endif

    auto titleFont = m_staticTextName->GetFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 4);
    m_staticTextName->SetFont(titleFont);
    m_staticTextVersion->SetLabel("v" APP_VERSION_FULL " (" GIT_SHA1 ")");
}
