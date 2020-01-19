///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 18 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace base
{

	///////////////////////////////////////////////////////////////////////////////
	/// Class MainWindow
	///////////////////////////////////////////////////////////////////////////////
	class MainWindow : public wxFrame
	{
		private:

		protected:
			wxStaticText* m_lblCargo;
			wxStaticText* m_lblOrigin;
			wxStaticText* m_lblDestination;
			wxStaticText* m_lblMass;
			wxStaticText* m_lblIncome;
			wxStaticText* m_lblCargoDamage;
			wxStaticText* m_lblDistanceDriven;
			wxStaticText* m_lblFuelConsumed;

			// Virtual event handlers, overide them in your derived class
			virtual void on_close( wxCloseEvent& event ) = 0;


		public:

			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ETS2 Job Logger"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,320 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~MainWindow();

	};

} // namespace base

