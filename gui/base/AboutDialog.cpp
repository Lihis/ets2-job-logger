///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 18 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "AboutDialog.h"

///////////////////////////////////////////////////////////////////////////
using namespace base;

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	m_bitmapIcon = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_bitmapIcon->SetMinSize( wxSize( 128,128 ) );

	bSizer1->Add( m_bitmapIcon, 0, wxALL, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	bSizer3->SetMinSize( wxSize( -1,64 ) );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextName = new wxStaticText( this, wxID_ANY, wxT("ETS2 Job Logger"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	m_staticTextName->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer4->Add( m_staticTextName, 0, wxALL, 5 );


	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	m_staticText1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false, wxEmptyString ) );

	bSizer5->Add( m_staticText1, 0, wxALL, 5 );

	m_staticTextVersion = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	m_staticTextVersion->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false, wxEmptyString ) );

	bSizer5->Add( m_staticTextVersion, 1, wxALL, 5 );


	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );


	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Copyright © 2019-2020 Tomi Lähteenmäki"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false, wxEmptyString ) );

	bSizer6->Add( m_staticText2, 0, wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Website:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer7->Add( m_staticText3, 0, wxALIGN_CENTER|wxALL, 5 );

	wxHyperlinkCtrl* m_hyperlink1;
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("https://github.com/Lihis/ets2-job-logger"), wxT("https://github.com/Lihis/ets2-job-logger"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer7->Add( m_hyperlink1, 0, wxALL, 5 );


	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );


	bSizer2->Add( bSizer6, 0, wxEXPAND, 5 );


	bSizer2->Add( 0, 20, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxStaticText* m_staticText4;
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("License:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer8->Add( m_staticText4, 0, wxALL, 5 );

	wxTextCtrl* m_textCtrl1;
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxT("This program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA"), wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
	m_textCtrl1->SetMinSize( wxSize( 480,150 ) );

	bSizer8->Add( m_textCtrl1, 1, wxALL|wxEXPAND, 5 );


	bSizer2->Add( bSizer8, 1, wxEXPAND, 5 );


	bSizer2->Add( 0, 20, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Application"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer10->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxHyperlinkCtrl* m_hyperlink2;
	m_hyperlink2 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("icon"), wxT("https://www.iconfinder.com/icons/4072100/box_delivery_package_shipping_transport_truck_icon"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer10->Add( m_hyperlink2, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );

	wxStaticText* m_staticText6;
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("by"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer10->Add( m_staticText6, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );

	wxHyperlinkCtrl* m_hyperlink3;
	m_hyperlink3 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("icon lauk"), wxT("https://www.iconfinder.com/andhikairfani"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer10->Add( m_hyperlink3, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );

	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("is licensed under"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer10->Add( m_staticText7, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );

	wxHyperlinkCtrl* m_hyperlink4;
	m_hyperlink4 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("CC BY 3.0"), wxT("https://creativecommons.org/licenses/by/3.0/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer10->Add( m_hyperlink4, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );


	bSizer9->Add( bSizer10, 0, wxEXPAND, 5 );


	bSizer2->Add( bSizer9, 0, wxEXPAND, 5 );


	bSizer2->Add( 0, 20, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_OK, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_buttonOK, 1, wxALL, 5 );


	bSizer2->Add( bSizer11, 0, wxALIGN_RIGHT, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}
