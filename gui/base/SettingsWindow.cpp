///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 18 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SettingsWindow.h"

///////////////////////////////////////////////////////////////////////////
using namespace base;

SettingsWindow::SettingsWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 320,-1 ), wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerGame;
	sbSizerGame = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Game") ), wxVERTICAL );

	m_checkBoxETS2 = new wxCheckBox( sbSizerGame->GetStaticBox(), wxID_ANY, wxT("Install ETS2 plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxETS2->SetValue(true);
	sbSizerGame->Add( m_checkBoxETS2, 0, wxALL, 5 );

	wxStaticText* m_staticTextETS2Path;
	m_staticTextETS2Path = new wxStaticText( sbSizerGame->GetStaticBox(), wxID_ANY, wxT("ETS2 Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextETS2Path->Wrap( -1 );
	sbSizerGame->Add( m_staticTextETS2Path, 0, wxALL, 5 );

	m_dirCtrlETS2Path = new wxDirPickerCtrl( sbSizerGame->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a ETS2 folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	sbSizerGame->Add( m_dirCtrlETS2Path, 0, wxALL|wxEXPAND, 5 );

	m_checkBoxATS = new wxCheckBox( sbSizerGame->GetStaticBox(), wxID_ANY, wxT("Install ATS plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerGame->Add( m_checkBoxATS, 0, wxALL, 5 );

	wxStaticText* m_staticTextATSPath;
	m_staticTextATSPath = new wxStaticText( sbSizerGame->GetStaticBox(), wxID_ANY, wxT("ATS Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextATSPath->Wrap( -1 );
	sbSizerGame->Add( m_staticTextATSPath, 0, wxALL, 5 );

	m_dirCtrlATSPath = new wxDirPickerCtrl( sbSizerGame->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a ATS folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	m_dirCtrlATSPath->Enable( false );

	sbSizerGame->Add( m_dirCtrlATSPath, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( sbSizerGame, 1, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerAPI;
	sbSizerAPI = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("API") ), wxVERTICAL );

	wxStaticText* m_staticTextURL;
	m_staticTextURL = new wxStaticText( sbSizerAPI->GetStaticBox(), wxID_ANY, wxT("URL:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextURL->Wrap( -1 );
	sbSizerAPI->Add( m_staticTextURL, 0, wxALL, 5 );

	m_textCtrlURL = new wxTextCtrl( sbSizerAPI->GetStaticBox(), wxID_ANY, wxT("https://"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerAPI->Add( m_textCtrlURL, 0, wxALL|wxEXPAND, 5 );

	wxStaticText* m_staticTextToken;
	m_staticTextToken = new wxStaticText( sbSizerAPI->GetStaticBox(), wxID_ANY, wxT("API Token:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToken->Wrap( -1 );
	sbSizerAPI->Add( m_staticTextToken, 0, wxALL, 5 );

	m_textCtrlToken = new wxTextCtrl( sbSizerAPI->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerAPI->Add( m_textCtrlToken, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( sbSizerAPI, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonOK, 0, wxALL, 5 );


	bSizer3->Add( bSizerButtons, 0, wxALIGN_RIGHT, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_checkBoxETS2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsWindow::on_install_ets2 ), NULL, this );
	m_checkBoxATS->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsWindow::on_install_ats ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsWindow::on_click_ok ), NULL, this );
}

SettingsWindow::~SettingsWindow()
{
	// Disconnect Events
	m_checkBoxETS2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsWindow::on_install_ets2 ), NULL, this );
	m_checkBoxATS->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsWindow::on_install_ats ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsWindow::on_click_ok ), NULL, this );

}
