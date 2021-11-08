#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>
class config : public wxFrame
{
public:
	config(wxPanel* parent, wxString title);
	wxButton* m_bconfigclose;
	wxButton* m_bconfigsave;
	wxStaticText* m_pathresolutiontext;
	wxStaticText* m_pathdefectstext;
	wxStaticText* m_pathlambda1300text;
	wxStaticText* m_pathlambda1500text;
	wxStaticText* m_pathlambda1900text;
	wxStaticText* m_pathlumtext;
	wxStaticText* m_scalingfactortext;
	wxTextCtrl* m_scalingfactor;
	wxTextCtrl* m_pathdefects;
	wxTextCtrl* m_pathresolution;
	wxTextCtrl* m_pathlambda1300;
	wxTextCtrl* m_pathlambda1500;
	wxTextCtrl* m_pathlambda1900;
	wxTextCtrl* m_pathlum;
	wxStaticBox* m_pathsbox;
	void Close(wxCommandEvent& event);
	void Save(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};
