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
	wxStaticText* m_idtext;
	wxTextCtrl* m_idmantext;
	wxButton* m_bidman;
	wxButton* m_bscanid;

	wxStaticText* m_measuretext;

	wxButton* m_bbatchman;
	wxTextCtrl* m_batchmantext;
	wxButton* m_bconfig;
	wxStaticText* m_batchtext;

	wxButton* m_bmeasres;
	wxButton* m_bretres;
	wxButton* m_bretdef;
	wxButton* m_bmeaslum;
	wxTextCtrl* m_valuelum;
	wxButton* m_bretlum;
	wxButton* m_bmeasdef;
	wxButton* m_bmeaslam1300;
	wxButton* m_bmeaslam1500;
	wxButton* m_bmeaslam1900;
	wxButton* m_bretlam1300;
	wxButton* m_bretlam1500;
	wxButton* m_bretlam1900;
};