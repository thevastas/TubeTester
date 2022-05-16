#include "config.h"
#include "MainWindow.h"
BEGIN_EVENT_TABLE(config, wxFrame)
EVT_CLOSE(config::Close)
EVT_BUTTON(controls::id::BCONFIGSAVE, config::Save)
END_EVENT_TABLE()


/*!
* Initializer for configuration window, which is used to set paths for the directories and other settings used for evaluation
*/
config::config(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(550, 600))
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();

	myParent->m_buttonPanel->m_bconfig->Disable();

	wxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	wxSizer* saveclosesizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* sizerpaths =		new wxBoxSizer(wxVERTICAL);
	wxSizer* sizersettings =	new wxBoxSizer(wxVERTICAL);

	m_bconfigsave =		new wxButton(this, controls::id::BCONFIGSAVE, "Save");
	
	m_pathresolutiontext =	new wxStaticText(this, controls::id::PATHRESOLUTIONTEXT,	"Resolution measurement directory");
	m_pathdefectstext =		new wxStaticText(this, controls::id::PATHDEFECTSTEXT,		"Defect measurement directory");
	m_pathlambda1300text =	new wxStaticText(this, controls::id::PATHLAMBDA1300TEXT,	"1300 nm directory");
	m_pathlambda1500text =	new wxStaticText(this, controls::id::PATHLAMBDA1500TEXT,	"1500 nm directory");
	m_pathlambda1900text =	new wxStaticText(this, controls::id::PATHLAMBDA1900TEXT,	"1900 nm directory");
	m_pathlumtext =			new wxStaticText(this, controls::id::PATHLUMTEXT,			"Luminance directory");
	m_pathanatext =			new wxStaticText(this, controls::id::PATHANATEXT,			"Analysis directory");

	m_pathresolution =	new wxTextCtrl(this, controls::id::PATHRESOLUTION,	myParent->directoryres, wxPoint(0,0),wxSize(500,20));
	m_pathdefects =		new wxTextCtrl(this, controls::id::PATHDEFECTS,		myParent->directorydef, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1300 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1300,	myParent->directory1300, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1500 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1500,	myParent->directory1500, wxPoint(0, 0), wxSize(500, 20));
	m_pathlambda1900 =	new wxTextCtrl(this, controls::id::PATHLAMBDA1900,	myParent->directory1900, wxPoint(0, 0), wxSize(500, 20));
	m_pathlum =			new wxTextCtrl(this, controls::id::PATHLUM,			myParent->directoryLuminance, wxPoint(0, 0), wxSize(500, 20));
	m_pathana =			new wxTextCtrl(this, controls::id::PATHANA,			myParent->directoryAnalysis, wxPoint(0, 0), wxSize(500, 20));

	//m_scalingfactortext =	new wxStaticText(this, controls::id::SCALINGFACTORTEXT, "Scaling factor, px/mm");
	//m_scalingfactor =		new wxTextCtrl(this, controls::id::SCALINGFACTOR, wxString::Format(wxT("%i"), myParent->scalingFactor), wxPoint(0, 0), wxSize(50, 20));

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
	sizerpaths->Add(m_pathanatext);
	sizerpaths->Add(m_pathana, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizerpaths->AddSpacer(10);

	//sizersettings->Add(m_scalingfactortext);
	//sizersettings->Add(m_scalingfactor);
	//sizersettings->AddSpacer(10);

	saveclosesizer->Add(m_bconfigsave);

	mainsizer->Add(sizerpaths);
	mainsizer->Add(sizersettings);
	mainsizer->Add(saveclosesizer);
	
	this->SetSizer(mainsizer);
	}

/*!
* Configuration window destroyer
*/
void config::Close(wxCloseEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	myParent->m_buttonPanel->m_bconfig->Enable();
	Destroy();
}

/*!
* Function for saving of the configuration settings into a text file
*/
void config::Save(wxCommandEvent& event) { //WARNING: DOES NOT SEEM TO BE WORKING!!!! WTF
	
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
		myParent->ReadConfig();
		myParent->m_configfile->SetPath("/paths");
		myParent->m_configfile->Write("defectspath", m_pathdefects->GetValue());
		myParent->m_configfile->Write("resolutionpath", m_pathresolution->GetValue());
		myParent->m_configfile->Write("luminancepath", m_pathlum->GetValue());
		myParent->m_configfile->Write("lambda1300path", m_pathlambda1300->GetValue());
		myParent->m_configfile->Write("lambda1500path", m_pathlambda1500->GetValue());
		myParent->m_configfile->Write("lambda1900path", m_pathlambda1900->GetValue());
		myParent->m_configfile->Write("analysispath", m_pathana->GetValue());
		myParent->m_configfile->SetPath("/evaluation");
		myParent->m_configfile->Write("scalingfactor", m_scalingfactor->GetValue());
		myParent->m_configfile->Flush();
		wxMessageBox(wxT("Settings saved to: ")+ myParent->configPath, wxT("Warning"), wxICON_WARNING);
		myParent->ReadConfig();
		myParent->UpdateButtons();
	

}