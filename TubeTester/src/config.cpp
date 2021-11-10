#include "config.h"
#include "MainWindow.h"
BEGIN_EVENT_TABLE(config, wxFrame)
EVT_BUTTON(controls::id::BCONFIGCLOSE, config::Close)
EVT_BUTTON(controls::id::BCONFIGSAVE, config::Save)
END_EVENT_TABLE()

config::config(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(550, 600))
{
	//SetBackgroundColour(wxColor(32, 32, 32));
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	wxSizer* saveclosesizer = new wxBoxSizer(wxHORIZONTAL);
	//m_pathsbox = new wxStaticBox(this, controls::id::PATHBOX, "Paths");
	//wxStaticBoxSizer* sizerpaths = new wxStaticBoxSizer(wxVERTICAL, m_pathsbox);
	wxSizer* sizerpaths =		new wxBoxSizer(wxVERTICAL);
	wxSizer* sizersettings =	new wxBoxSizer(wxVERTICAL);

	m_bconfigsave =		new wxButton(this, controls::id::BCONFIGSAVE, "Save");
	m_bconfigclose =	new wxButton(this, controls::id::BCONFIGCLOSE, "Close");

	
	m_pathresolutiontext =	new wxStaticText(this, controls::id::PATHRESOLUTIONTEXT,	"Resolution measurement directory");
	m_pathdefectstext =		new wxStaticText(this, controls::id::PATHDEFECTSTEXT,		"Defect measurement directory");
	m_pathlambda1300text =	new wxStaticText(this, controls::id::PATHLAMBDA1300TEXT,	"1300 nm directory");
	m_pathlambda1500text =	new wxStaticText(this, controls::id::PATHLAMBDA1500TEXT,	"1500 nm directory");
	m_pathlambda1900text =	new wxStaticText(this, controls::id::PATHLAMBDA1900TEXT,	"1900 nm directory");
	m_pathlumtext =			new wxStaticText(this, controls::id::PATHLUMTEXT,			"Luminance directory");

	m_pathresolution =	new wxTextCtrl(this, controls::id::PATHRESOLUTION,	myParent->directoryres, wxPoint(0,0),wxSize(500,20));
	m_pathdefects =		new wxTextCtrl(this, controls::id::PATHDEFECTS,		myParent->directorydef, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1300 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1300,	myParent->directory1300, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1500 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1500,	myParent->directory1300, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1900 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1900,	myParent->directory1300, wxPoint(0, 0), wxSize(500, 20));
	m_pathlum =			new wxTextCtrl(this, controls::id::PATHLUM,			myParent->directoryLuminance, wxPoint(0, 0), wxSize(500, 20));

	m_scalingfactortext =	new wxStaticText(this, controls::id::SCALINGFACTORTEXT, "Scaling factor, px/mm");
	m_scalingfactor =		new wxTextCtrl(this, controls::id::SCALINGFACTOR, wxString::Format(wxT("%i"), myParent->scalingFactor), wxPoint(0, 0), wxSize(50, 20));

	//sizerpaths->Add(m_pathsbox);
	sizerpaths->Add(m_pathresolutiontext);
	sizerpaths->Add(m_pathresolution, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->Add(m_pathdefectstext);
	sizerpaths->Add(m_pathdefects, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->Add(m_pathlambda1300text);
	sizerpaths->Add(m_pathlambda1300, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->Add(m_pathlambda1500text);
	sizerpaths->Add(m_pathlambda1500, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->Add(m_pathlambda1900text);
	sizerpaths->Add(m_pathlambda1900, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->Add(m_pathlumtext);
	sizerpaths->Add(m_pathlum, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->AddSpacer(10);

	sizersettings->Add(m_scalingfactortext);
	sizersettings->Add(m_scalingfactor);
	sizersettings->AddSpacer(10);

	saveclosesizer->Add(m_bconfigsave);
	saveclosesizer->Add(m_bconfigclose);


	//sizerpaths->SetItemMinSize(this, wxSize(300, 100));

	mainsizer->Add(sizerpaths);
	mainsizer->Add(sizersettings);
	mainsizer->Add(saveclosesizer);
	
	this->SetSizer(mainsizer);
	}

void config::Close(wxCommandEvent& event) {
	Destroy();
}

void config::Save(wxCommandEvent& event) {
	
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();

		myParent->m_configfile->SetPath("/paths");
		myParent->m_configfile->Write("defectspath", m_pathresolution->GetValue());
		myParent->m_configfile->Write("resolutionpath", m_pathdefects->GetValue());
		myParent->m_configfile->Write("luminancepath", m_pathlum->GetValue());
		myParent->m_configfile->Write("lambda1300path", m_pathlambda1300->GetValue());
		myParent->m_configfile->Write("lambda1500path", m_pathlambda1500->GetValue());
		myParent->m_configfile->Write("lambda1900path", m_pathlambda1900->GetValue());
		myParent->m_configfile->SetPath("/evaluation");
		myParent->m_configfile->Write("scalingfactor", m_scalingfactor->GetValue());
		myParent->m_configfile->Flush();
		wxMessageBox(wxT("Settings saved to: ")+ myParent->appPath, wxT("Warning"), wxICON_WARNING);
	

}