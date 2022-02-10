#pragma once
#include "Common.h"
#include "MainWindow.h"
#include <wx/wx.h>
#include <wx/panel.h>
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/radiobut.h"
#include "wx/listbox.h"
#include "wx/bitmap.h"
class buttonPanel : public wxPanel
{
public:
	buttonPanel(wxPanel* parent);
	wxPanel* m_parent;
	
	// First row
	wxButton* m_bscanid; //!< Button for scanning the ID from the QR code
	wxButton* m_bidman; //!< Button for setting the manual ID
	wxTextCtrl* m_idmantext; //!< Textbox for the manual ID
	
	// Second row
	wxButton* m_bconfig; //!< Button for opening up the configuration window
	wxButton* m_bbatchman; //!< Button for manually changing the batch number
	wxTextCtrl* m_batchmantext; //!< Textbox for the batch number of the measurement
	
	// Third row
	wxStaticText* m_measuretext; //!< Text displaying the "Measuring"guider
	wxStaticText* m_batchtext; //!< Text displaying the batch number of the measurement
	wxStaticText* m_idtext; //!< Text displaying the ID of the tube

	// Fourth row
	wxButton* m_bmeasres; //!< Button for opening the resolution measurement window
	wxButton* m_bretres; //!< Button for retrieving the recorded resolution image in a window

	// Fifth row
	wxButton* m_bmeasdef; //!< Button for opening the defects measurement window
	wxButton* m_bretdef; //!< Button for retrieving the recorded defects image in a window

	// Sixth row
	wxButton* m_bmeaslum; //!< Button for saving the entered luminance value into a text file
	wxTextCtrl* m_valuelum; //!< Textbox for entering or displaying the luminance value
	wxButton* m_bretlum; //!< Button for retrieving the recorded luminance value in a textbox
	
	// Seventh row
	wxButton* m_bmeaslam1300; //!< Button for opening the measurement window for the sensitivity of 1300 nm wavelenght
	wxButton* m_bretlam1300; //!< Button for retrieving the recorded image for the sensitivity of 1300 nm wavelenght

	// Eighth row
	wxButton* m_bmeaslam1500; //!< Button for opening the measurement window for the sensitivity of 1500 nm wavelenght
	wxButton* m_bretlam1500; //!< Button for retrieving the recorded image for the sensitivity of 1500 nm wavelenght

	// Ninth row
	wxButton* m_bmeaslam1900; //!< Button for opening the measurement window for the sensitivity of 1900 nm wavelenght
	wxButton* m_bretlam1900; //!< Button for retrieving the recorded image for the sensitivity of 1900 nm wavelenght

	// Tenth row
	wxButton* m_badvanced; //!< Button for opening the advanced functionality window
};