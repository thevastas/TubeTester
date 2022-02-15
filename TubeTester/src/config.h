#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>
class config : public wxFrame
{
public:
	config(wxPanel* parent, wxString title);//!< Initializer for configuration window, which is used to set paths for the directories and other settings used for evaluation

	wxStaticText* m_pathresolutiontext; //!< Text displaying the "resolution path" guider
	wxTextCtrl* m_pathresolution; //!< Textbox for displaying or changing the path of the directory for resolution measurements

	wxStaticText* m_pathdefectstext; //!< Text displaying the "defects path" guider
	wxTextCtrl* m_pathdefects; //!< Textbox for displaying or changing the path of the directory for defects measurements

	wxStaticText* m_pathlambda1300text; //!< Text displaying the "lambda 1300 path" guider
	wxTextCtrl* m_pathlambda1300; //!< Textbox for displaying or changing the path of the directory for the 1300 nm sensitivity measurements

	wxStaticText* m_pathlambda1500text; //!< Text displaying the "lambda 1500 path" guider
	wxTextCtrl* m_pathlambda1500; //!< Textbox for displaying or changing the path of the directory for the 1500 nm sensitivity measurements

	wxStaticText* m_pathlambda1900text; //!< Text displaying the "lambda 1900 path" guider
	wxTextCtrl* m_pathlambda1900; //!< Textbox for displaying or changing the path of the directory for the 1900 nm sensitivity measurements

	wxStaticText* m_pathlumtext; //!< Text displaying the "luminance path" guider
	wxTextCtrl* m_pathlum; //!< Textbox for displaying or changing the path of the directory for luminance measurements

	wxStaticText* m_pathanatext; //!< Text displaying the "analysis path" guider
	wxTextCtrl* m_pathana; //!< Textbox for displaying or changing the path of the directory for analysis files

	wxStaticText* m_scalingfactortext; //!< Text displaying the "scaling factor" guider
	wxTextCtrl* m_scalingfactor; //!< Textbox for displaying or chaning the scaling factor in px/mm

	wxButton* m_bconfigsave; //!< Button for saving the configuration to a text file

	void Close(wxCloseEvent& event); //!< Close event that is executed upon closing the window
	void Save(wxCommandEvent& event); //!< Command event for saving the configuration to a text file
	wxDECLARE_EVENT_TABLE();
};
