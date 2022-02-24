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
	void OnFindLambda1500(wxCommandEvent& event); //!< Command function for performing the 1500 nm wavelength analysis
	void OnFindLambda1900(wxCommandEvent& event); //!< Command function for performing the 1900 nm wavelength analysis
	void Close(wxCloseEvent& event); //!< Close event that is executed upon closing the window

	wxButton* m_blambda1300; //!< Button for the 1300 nm wavelength analysis
	wxButton* m_blambda1500; //!< Button for the 1500 nm wavelength analysis
	wxButton* m_blambda1900; //!< Button for the 1900 nm wavelength analysis

	wxFileName pathlambda1300; //!< Path holder for finding 1300 nm text files
	wxFileName pathlambda1500; //!< Path holder for finding 1500 nm text files
	wxFileName pathlambda1900; //!< Path holder for finding 1900 nm text files
	wxString pathlambda1300summaryCircle; //!< Path holder for the text file into which the summary is written for the instensity in the defined circle of the 1300 nm image
	wxString pathlambda1500summaryCircle; //!< Path holder for the text file into which the summary is written for the instensity in the defined circle of the 1500 nm image
	wxString pathlambda1900summaryCircle; //!< Path holder for the text file into which the summary is written for the instensity in the defined circle of the 1900 nm image
	wxString pathlambda1300summaryTotal; //!< Path holder for the text file into which the summary is written for the instensity in the whole 1300 nm image image
	wxString pathlambda1500summaryTotal; //!< Path holder for the text file into which the summary is written for the instensity in the whole 1500 nm image image
	wxString pathlambda1900summaryTotal; //!< Path holder for the text file into which the summary is written for the instensity in the whole 1900 nm image image
	wxTextFile* lambda1300SummaryCircleFile; //!< Text file into which the summary is written for the intensity in the defined circle of the 1300 nm image
	wxTextFile* lambda1500SummaryCircleFile; //!< Text file into which the summary is written for the intensity in the defined circle of the 1500 nm image
	wxTextFile* lambda1900SummaryCircleFile; //!< Text file into which the summary is written for the intensity in the defined circle of the 1900 nm image
	wxTextFile* lambda1300SummaryTotalFile; //!< Text file into which the summary is written for the instensity in the whole 1300 nm image
	wxTextFile* lambda1500SummaryTotalFile; //!< Text file into which the summary is written for the instensity in the whole 1500 nm image
	wxTextFile* lambda1900SummaryTotalFile; //!< Text file into which the summary is written for the instensity in the whole 1900 nm image
	wxString filenamelambda1300; //!< Filename of the file that is being read (1300 nm)
	wxString filenamelambda1500; //!< Filename of the file that is being read (1500 nm)
	wxString filenamelambda1900; //!< Filename of the file that is being read (1900 nm)

	wxTextFile* lambdaFile; //!< Text file which is read for the chosen wavelength
	wxString fileID; //!< ID of the tube of which the file is being read, e.g. 2001_00079
	wxArrayString arrstr; //!< String array which holds different parts of the filename after splitting it

	int totalval; //!< Intensity value for the whole image
	int circleval; //!< Intensity value for the area within the defined circle
	
	wxDECLARE_EVENT_TABLE();
};