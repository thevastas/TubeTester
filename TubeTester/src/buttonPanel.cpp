#pragma once
#include "MainWindow.h"
#include "buttonPanel.h"

buttonPanel::buttonPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint(10, 10), wxSize(560, 980))
{

	
	int box = 10;
	int boy = 10;
	int bsp = 30;
	int bszx = 160;
	int bszy = 80;
	m_parent = parent;
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	// First Row
	
	m_idmantext = new wxTextCtrl (this, controls::id::IDMANTEXT, "", wxPoint(box + 2*(bszx+bsp), boy), wxSize(bszx, bszy));
	m_bscanid = new wxButton(this, controls::id::BSCANID, "Scan ID", wxPoint(box, boy), wxSize(bszx, bszy));
	m_bidman = new wxButton(this, controls::id::BIDMAN, "Manual ID", wxPoint((box + bszx+bsp),boy),wxSize(bszx,bszy));

	// Second Row
	m_batchmantext = new wxTextCtrl(this, controls::id::BATCHMANTEXT, "", wxPoint(box + 2 * (bszx + bsp), boy + 1 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bbatchman = new wxButton(this, controls::id::BBATCHMAN, "Change batch", wxPoint((box + bszx + bsp), boy + 1* (bszy + bsp)), wxSize(bszx, bszy));
	m_bconfig = new wxButton(this, controls::id::BCONFIG, "Configuration", wxPoint((box), boy + 1 * (bszy + bsp)), wxSize(bszx, bszy));

	// Third Row
	m_measuretext = new wxStaticText(this, controls::id::MEASURETEXT, "Measuring:", wxPoint(box, boy + 2 * (bszy + bsp) + bsp), wxSize(bszx, bszy), wxALIGN_CENTRE_HORIZONTAL);
	m_batchtext = new wxStaticText(this, controls::id::BATCHTEXT, wxString::Format(wxT("%i"), myParent->batchnumber), wxPoint(box + 1 * (bszx + bsp), boy + 2 * (bszy + bsp) + bsp), wxSize(bszx, bszy), wxALIGN_CENTRE_HORIZONTAL);
	m_idtext = new wxStaticText(this, controls::id::IDTEXT, "Empty ID", wxPoint(box + 2 * (bszx + bsp), boy + 2 * (bszy + bsp) + bsp), wxSize(bszx, bszy), wxALIGN_CENTRE_HORIZONTAL);


	// Fourth Row
	m_bmeasres = new wxButton(this, controls::id::BMEASRES,"Resolution", wxPoint(box,					boy + 3 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretres = new wxButton(this, controls::id::BRETRES, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 3 * (bszy + bsp)), wxSize(bszx, bszy));

	// Fifth Row
	m_bmeasdef = new wxButton(this, controls::id::BMEASDEF, "Defects", wxPoint(box,						boy + 4 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretdef = new wxButton(this, controls::id::BRETDEF, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 4 * (bszy + bsp)), wxSize(bszx, bszy));

	// Sixth Row
	m_bmeaslum = new wxButton(this, controls::id::BMEASLUM, "Luminance", wxPoint(box,					boy + 5 * (bszy + bsp)), wxSize(bszx, bszy));
	m_valuelum = new wxTextCtrl(this, controls::id::VALUELUM, "", wxPoint((box + bszx + bsp),			boy + 5 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretlum = new wxButton(this, controls::id::BRETLUM, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 5 * (bszy + bsp)), wxSize(bszx, bszy));

	// Seventh Row
	m_bmeaslam1300 = new wxButton(this, controls::id::BMEASLAM1300, "1300 nm", wxPoint(box,						boy + 6 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretlam1300 = new wxButton(this, controls::id::BRETLAM1300, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 6 * (bszy + bsp)), wxSize(bszx, bszy));

	// Eighth Row
	m_bmeaslam1500 = new wxButton(this, controls::id::BMEASLAM1500, "1500 nm", wxPoint(box,						boy + 7 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretlam1500 = new wxButton(this, controls::id::BRETLAM1500, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 7 * (bszy + bsp)), wxSize(bszx, bszy));

	// Ninth Row
	m_bmeaslam1900 = new wxButton(this, controls::id::BMEASLAM1900, "1900 nm", wxPoint(box,						boy + 8 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretlam1900 = new wxButton(this, controls::id::BRETLAM1900, "Retrieve", wxPoint(box + 2 * (bszx + bsp),	boy + 8 * (bszy + bsp)), wxSize(bszx, bszy));
}
