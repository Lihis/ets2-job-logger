///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 18 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainWindow.h"

///////////////////////////////////////////////////////////////////////////
using namespace base;

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxMenuBar* m_menubar1;
	m_menubar1 = new wxMenuBar( 0 );
	wxMenu* m_menuFile;
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemSettings;
	m_menuItemSettings = new wxMenuItem( m_menuFile, wxID_SETUP, wxString( wxT("Settings") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSettings );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemExit );

	m_menubar1->Append( m_menuFile, wxT("File") );

	wxMenu* m_menuHelp;
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );

	m_menubar1->Append( m_menuHelp, wxT("Help") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Job") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* m_staticTextCargo;
	m_staticTextCargo = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Cargo:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCargo->Wrap( -1 );
	fgSizer3->Add( m_staticTextCargo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_lblCargo = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCargo->Wrap( -1 );
	fgSizer3->Add( m_lblCargo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxStaticText* m_staticTextOrigin;
	m_staticTextOrigin = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Origin:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOrigin->Wrap( -1 );
	fgSizer3->Add( m_staticTextOrigin, 0, wxALL, 5 );

	m_lblOrigin = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblOrigin->Wrap( -1 );
	fgSizer3->Add( m_lblOrigin, 0, wxALL, 5 );

	wxStaticText* m_staticTextDestination;
	m_staticTextDestination = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Destination:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDestination->Wrap( -1 );
	fgSizer3->Add( m_staticTextDestination, 0, wxALL, 5 );

	m_lblDestination = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDestination->Wrap( -1 );
	fgSizer3->Add( m_lblDestination, 0, wxALL, 5 );

	wxStaticText* m_staticTextMass;
	m_staticTextMass = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Mass:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMass->Wrap( -1 );
	fgSizer3->Add( m_staticTextMass, 0, wxALL, 5 );

	m_lblMass = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMass->Wrap( -1 );
	fgSizer3->Add( m_lblMass, 0, wxALL, 5 );

	wxStaticText* m_staticTextIncome;
	m_staticTextIncome = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Income:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIncome->Wrap( -1 );
	fgSizer3->Add( m_staticTextIncome, 0, wxALL, 5 );

	m_lblIncome = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblIncome->Wrap( -1 );
	fgSizer3->Add( m_lblIncome, 0, wxALL, 5 );

	wxStaticText* m_staticTextCargoDamage;
	m_staticTextCargoDamage = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Damage:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCargoDamage->Wrap( -1 );
	fgSizer3->Add( m_staticTextCargoDamage, 0, wxALL, 5 );

	m_lblCargoDamage = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCargoDamage->Wrap( -1 );
	fgSizer3->Add( m_lblCargoDamage, 0, wxALL, 5 );

	wxStaticText* m_staticTextDriven;
	m_staticTextDriven = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Distance driven:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDriven->Wrap( -1 );
	fgSizer3->Add( m_staticTextDriven, 0, wxALL, 5 );

	m_lblDistanceDriven = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDistanceDriven->Wrap( -1 );
	fgSizer3->Add( m_lblDistanceDriven, 0, wxALL, 5 );

	wxStaticText* m_staticTextFuel;
	m_staticTextFuel = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Fuel consumed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFuel->Wrap( -1 );
	fgSizer3->Add( m_staticTextFuel, 0, wxALL, 5 );

	m_lblFuelConsumed = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFuelConsumed->Wrap( -1 );
	fgSizer3->Add( m_lblFuelConsumed, 0, wxALL, 5 );


	sbSizer3->Add( fgSizer3, 1, wxEXPAND, 5 );


	bSizer1->Add( sbSizer3, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::on_close ) );
}

MainWindow::~MainWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::on_close ) );

}
