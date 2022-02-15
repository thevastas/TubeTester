#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>
#include <wx/filefn.h> 
#include <wx/filename.h>

class advanced : public wxFrame
{
public:
	advanced(wxPanel* parent, wxString title); //!< Initializer for advanced functionality window, which contains buttons for executing various analysis procedures
	void OnFindLambda1300(wxCommandEvent& event); //!< Command function for performing the 1300 nm wavelength analysis
	void Close(wxCloseEvent& event); //!< Close event that is executed upon closing the window

	wxButton* m_blambda1300; //!< Button for the 1300 nm wavelength analysis
	wxFileName pathlambda1300; //!< Path holder for finding 1300 nm text files
	wxString pathlambda1300summaryCircle; //!< Path holder for the text file into which the summary is written for the instensity in the defined circle
	wxString pathlambda1300summaryTotal; //!< Path holder for the text file into which the summary is written for the instensity in the whole image
	wxTextFile* lambda1300SummaryCircleFile; //!< Text file into which the summary is written for the instensity in the defined circle
	wxTextFile* lambda1300SummaryTotalFile; //!< Text file into which the summary is written for the instensity in the whole image
	wxString filenamelambda1300; //!< Filename of the file that is being read (1300 nm)

	wxTextFile* lambdaFile; //!< Text file which is read for the chosen wavelength
	wxString fileID; //!< ID of the tube of which the file is being read, e.g. 2001_00079
	wxArrayString arrstr; //!< String array which holds different parts of the filename after splitting it

	int totalval; //!< Intensity value for the whole image
	int circleval; //!< Intensity value for the area within the defined circle
	
	wxDECLARE_EVENT_TABLE();
};