#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>

class advanced : public wxFrame
{
public:
	advanced(wxPanel* parent, wxString title);
	void Close(wxCloseEvent& event);
	wxDECLARE_EVENT_TABLE();
};