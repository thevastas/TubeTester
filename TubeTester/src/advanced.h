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
	advanced(wxPanel* parent, wxString title);
	void OnFindLambda1300(wxCommandEvent& event);
	void Close(wxCloseEvent& event);

	wxButton* m_blambda1300;
	
	wxFileName pathlambda1300;
	wxString pathlambda1300summaryCircle;
	wxString pathlambda1300summaryTotal;
	wxTextFile* lambda1300SummaryCircleFile;
	wxTextFile* lambda1300SummaryTotalFile;
	wxFileName fullnamelambda1300;
	wxTextFile* lambdaFile;
	wxString filenamelambda1300;
	wxString fileID;


	int totalval;
	int circleval;
	wxArrayString arrstr;
	wxDECLARE_EVENT_TABLE();
};