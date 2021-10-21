#pragma once
#include "MainWindow.h"
#include "buttonPanel.h"

buttonPanel::buttonPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint(10, 10), wxSize(700, 600))
{
	int box = 10;
	int boy = 10;
	int bsp = 20;
	int bszx = 160;
	int bszy = 40;
	m_parent = parent;

	// First Row
	m_idtext = new wxStaticText(this, controls::id::IDTEXT,"Empty ID", wxPoint(box + 3*(bszx+bsp), boy+bsp/2),wxSize(bszx,bszy), wxALIGN_CENTRE_HORIZONTAL);
	m_idmantext = new wxTextCtrl (this, controls::id::IDMANTEXT, "", wxPoint(box + 2*(bszx+bsp), boy), wxSize(bszx, bszy));
	m_bscanid = new wxButton(this, controls::id::BSCANID, "Scan ID", wxPoint(box, boy), wxSize(bszx, bszy));
	m_bidman = new wxButton(this, controls::id::BIDMAN, "Manual ID", wxPoint((box + bszx+bsp),boy),wxSize(bszx,bszy));

	// Second Row
	m_measuretext = new wxStaticText(this, controls::id::MEASURETEXT, "Measure", wxPoint(box, boy + bszy + bsp + bsp), wxSize(bszx, bszy), wxALIGN_CENTRE_HORIZONTAL);
	
	// Third Row
	m_bmeasres = new wxButton(this, controls::id::BMEASRES,"Resolution", wxPoint(box, boy + 2 * (bszy + bsp)), wxSize(bszx,bszy));
	m_bretres = new wxButton(this, controls::id::BRETRES, "Retrieve", wxPoint(box + 2 * (bszx + bsp), boy + 2 * (bszy + bsp)), wxSize(bszx, bszy));

	// Fourth Row
	m_bmeasdef = new wxButton(this, controls::id::BMEASDEF, "Defects", wxPoint(box, boy + 3*(bszy + bsp)), wxSize(bszx, bszy));
	m_bretdef = new wxButton(this, controls::id::BRETDEF, "Retrieve", wxPoint(box + 2 * (bszx + bsp), boy + 3 * (bszy + bsp)), wxSize(bszx, bszy));

	// Fifth Row
	m_bmeaslum = new wxButton(this, controls::id::BMEASLUM, "Luminance", wxPoint(box, boy + 4 * (bszy + bsp)), wxSize(bszx, bszy));
	m_valuelum = new wxTextCtrl(this, controls::id::VALUELUM, "", wxPoint((box + bszx + bsp), boy + 4 * (bszy + bsp)), wxSize(bszx, bszy));
	m_bretlum = new wxButton(this, controls::id::BRETLUM, "Retrieve", wxPoint(box + 2 * (bszx + bsp), boy + 4 * (bszy + bsp)), wxSize(bszx, bszy));

	// Sixth Row


}
