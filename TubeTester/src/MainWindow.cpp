#include "Common.h"
#include <wx/wx.h>
#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>
#include <wx/slider.h>
#include <wx/textdlg.h>
#include <wx/thread.h>
#include <wx/utils.h>
#include "bmpfromocvpanel.h"
#include "convertmattowxbmp.h"
#include "buttonPanel.h"
#include "MainWindow.h"
#include <wx/popupwin.h>
#include "ImageFrame.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_BUTTON(controls::id::BRETRES,       MainWindow::OpenResolutionImage)
    EVT_BUTTON(controls::id::BRETDEF,       MainWindow::OpenDefectsImage)
    EVT_BUTTON(controls::id::BRETLAM1300,   MainWindow::Open1300Image)
    EVT_BUTTON(controls::id::BRETLAM1500,   MainWindow::Open1500Image)
    EVT_BUTTON(controls::id::BRETLAM1900,   MainWindow::Open1900Image)
    EVT_BUTTON(controls::id::BMEASRES,      MainWindow::CaptureResolutionImage)
    EVT_BUTTON(controls::id::BMEASDEF,      MainWindow::CaptureDefectsImage)
    EVT_BUTTON(controls::id::BMEASLAM1300,  MainWindow::Capture1300Image)
    EVT_BUTTON(controls::id::BMEASLAM1500,  MainWindow::Capture1500Image)
    EVT_BUTTON(controls::id::BMEASLAM1900,  MainWindow::Capture1900Image)
    EVT_BUTTON(controls::id::BSCANID,       MainWindow::ScanID)
    EVT_BUTTON(controls::id::BIDMAN,        MainWindow::SetManualID)
    EVT_BUTTON(controls::id::BBATCHMAN,     MainWindow::SetManualBatch)
    EVT_BUTTON(controls::id::BMEASLUM,      MainWindow::SaveLuminance)
    EVT_BUTTON(controls::id::BRETLUM,       MainWindow::RetrieveLuminance)
    EVT_BUTTON(controls::id::BCONFIG,       MainWindow::OpenConfiguration)
END_EVENT_TABLE()

MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :

    wxFrame(parent, id, title, pos, size, style, name)
{

    wxWindow::SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    cv::setBreakOnError(true);


    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    appPath=(f.GetPath() + _T("\\config.ini"));

    m_configfile = new wxFileConfig(wxEmptyString, wxEmptyString, appPath, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    //main panel
    m_parent = new wxPanel(this, wxID_ANY);
    SetBackgroundColour(wxColor(32, 32, 32));



    if (wxFileExists(appPath)) {
        m_configfile->SetPath("/paths");

            m_configfile->Read("defectspath", &directorydef);
            m_configfile->Read("resolutionpath", &directoryres);
            m_configfile->Read("luminancepath", &directoryLuminance);
            m_configfile->Read("lamba1300path", &directory1300);
            m_configfile->Read("lamba1500path", &directory1500);
            m_configfile->Read("lamba1900path", &directory1900);

            m_configfile->SetPath("/evaluation");
            m_configfile->Read("scalingfactor", &scalingFactor);

            m_configfile->SetPath("/misc");
            m_configfile->Read("batch",&batchnumber);

    }
    else {
        LOG(ERROR) << "File does not exist for writing:" << appPath;
        wxMessageBox(wxT("The configuration file does not exist:") + appPath, wxT("Warning"), wxICON_WARNING);
    }

    m_buttonPanel = new buttonPanel(m_parent);
    m_buttonPanel->SetBackgroundColour(wxColor(64, 64, 64));



    SetMinClientSize(FromDIP(wxSize(580, 1000)));
    SetSize(FromDIP(wxSize(580, 1000)));
       
    
}


MainWindow::~MainWindow()
{

}

void MainWindow::OpenResolutionImage(wxCommandEvent& event) {
    m_imagemode = ResolutionImage;
    m_resolutionWindow = new imageFrame(m_parent, "Saved Resolution image: "+ m_buttonPanel->m_idtext->GetLabel());
    m_resolutionWindow->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_resolutionWindow->Show();
}
void MainWindow::OpenDefectsImage(wxCommandEvent& event) {
    m_imagemode = DefectsImage;
    m_defectsWindow = new imageFrame(m_parent, "Saved Defects image: " + m_buttonPanel->m_idtext->GetLabel());
    m_defectsWindow->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_defectsWindow->Show();
}
void MainWindow::Open1300Image(wxCommandEvent& event) {
    m_imagemode = l1300Image;
    m_1300Window = new imageFrame(m_parent, "Saved 1300 nm image: " + m_buttonPanel->m_idtext->GetLabel());
    m_1300Window->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_1300Window->Show();
}
void MainWindow::Open1500Image(wxCommandEvent& event) {
    m_imagemode = l1500Image;
    m_1500Window = new imageFrame(m_parent, "Saved 1500 nm Image: " + m_buttonPanel->m_idtext->GetLabel());
    m_1500Window->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_1500Window->Show();
}
void MainWindow::Open1900Image(wxCommandEvent& event) {
    m_imagemode = l1900Image;
    m_1900Window = new imageFrame(m_parent, "Saved 1900 nm Image: " + m_buttonPanel->m_idtext->GetLabel());
    m_1900Window->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_1900Window->Show();
}

void MainWindow::CaptureResolutionImage(wxCommandEvent& event) {
    m_imagemode = ResolutionVideo;
    m_capres = new imageFrame(m_parent, "Capturing resolution image: " + m_buttonPanel->m_idtext->GetLabel());
    m_capres->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_capres->OnCamera(event);
    m_capres->Show();
}

void MainWindow::CaptureDefectsImage(wxCommandEvent& event) {
    m_imagemode = DefectsVideo;
    m_capdef = new imageFrame(m_parent, "Capturing defects image: " + m_buttonPanel->m_idtext->GetLabel());
    m_capdef->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_capdef->OnCamera(event);
    m_capdef->Show();
}

void MainWindow::Capture1300Image(wxCommandEvent& event) {
    m_imagemode = l1300Video;
    m_cap1300 = new imageFrame(m_parent, "Capturing 1300 nm image: " + m_buttonPanel->m_idtext->GetLabel());
    m_cap1300->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_cap1300->OnCamera(event);
    m_cap1300->Show();
}

void MainWindow::Capture1500Image(wxCommandEvent& event) {
    m_imagemode = l1500Video;
    m_cap1500 = new imageFrame(m_parent, "Capturing 1500 nm image: " + m_buttonPanel->m_idtext->GetLabel());
    m_cap1500->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_cap1500->OnCamera(event);
    m_cap1500->Show();
}

void MainWindow::Capture1900Image(wxCommandEvent& event) {
    m_imagemode = l1900Video;
    m_cap1900 = new imageFrame(m_parent, "Capturing 1900 nm image: " + m_buttonPanel->m_idtext->GetLabel());
    m_cap1900->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_cap1900->OnCamera(event);
    m_cap1900->Show();
}
  
void MainWindow::SaveLuminance(wxCommandEvent& event) {
    m_imagemode = luminance;
    wxString pathLuminance = directoryLuminance + m_buttonPanel->m_idtext->GetLabel()+".txt";
    wxTextFile* luminanceFile = new wxTextFile(pathLuminance);
    if (!wxFileExists(pathLuminance)) {
        luminanceFile->Create();
    }
    luminanceFile->Open();
    luminanceFile->AddLine("lum=" + m_buttonPanel->m_valuelum->GetValue());
    luminanceFile->Write();
    luminanceFile->Close();
}

void MainWindow::RetrieveLuminance(wxCommandEvent& event) {
    m_imagemode = luminance;
    MainWindow* myParent = (MainWindow*)m_parent->GetParent();
    wxString pathLuminance = directoryLuminance + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel()+".txt";
    wxTextFile* luminanceFile = new wxTextFile(pathLuminance);
    if (wxFileExists(pathLuminance)) {
        luminanceFile->Open();
        wxArrayString arrstr = wxSplit(luminanceFile->GetLastLine(), '=');
        int cval = wxAtoi(arrstr[1]);
        m_buttonPanel->m_valuelum->SetValue(wxString::Format(wxT("%i"), cval));
        luminanceFile->Close();
    }
    else {
        LOG(ERROR) << "Luminance file not found.";
        wxMessageBox(wxT("The file does not exist"), wxT("Warning"), wxICON_WARNING);
    }
}

void MainWindow::ScanID(wxCommandEvent& event) {
    wxMessageBox(wxT("The scanning feature is not yet implemented"), wxT("Warning"), wxICON_WARNING);
}


void  MainWindow::SetManualID(wxCommandEvent& event) {

    m_buttonPanel->m_idtext->SetLabel(m_buttonPanel->m_idmantext->GetLineText(0));
    UpdateButtons();
}

void MainWindow::OpenConfiguration(wxCommandEvent& event) {
    m_configwindow = new config(m_parent, "Configuration");
    m_configwindow->SetSize(frameoriginx, frameoriginy, framesizex, framesizey);
    m_configwindow->Show();
}

void MainWindow::SetManualBatch(wxCommandEvent& event) {
    m_buttonPanel->m_batchtext->SetLabel(m_buttonPanel->m_batchmantext->GetValue());
    batchnumber = wxAtoi(m_buttonPanel->m_batchmantext->GetValue());
    UpdateButtons();
}

void MainWindow::UpdateButtons() {
    MainWindow* myParent = (MainWindow*)m_parent->GetParent();
    lumfile = directoryLuminance + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".txt";
    resfile = directoryres + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".jpg";
    deffile = directorydef + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".jpg";
    l1300file = directory1300 + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".jpg";
    l1500file = directory1500 + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".jpg";
    l1900file = directory1900 + wxString::Format(wxT("%i/"), myParent->batchnumber) + m_buttonPanel->m_idtext->GetLabel() + ".jpg";
    if (!wxFileExists(lumfile)) {
        m_buttonPanel->m_bretlum->Disable();
    }
    else m_buttonPanel->m_bretlum->Enable();

    if (!wxFileExists(resfile)) {
        m_buttonPanel->m_bretres->Disable();
    }
    else m_buttonPanel->m_bretres->Enable();

    if (!wxFileExists(deffile)) {
        m_buttonPanel->m_bretdef->Disable();
    }
    else m_buttonPanel->m_bretdef->Enable();

    if (!wxFileExists(l1300file)) {
        m_buttonPanel->m_bretlam1300->Disable();
    }
    else m_buttonPanel->m_bretlam1300->Enable();

    if (!wxFileExists(l1500file)) {
        m_buttonPanel->m_bretlam1500->Disable();
    }
    else m_buttonPanel->m_bretlam1500->Enable();

    if (!wxFileExists(l1900file)) {
        m_buttonPanel->m_bretlam1900->Disable();
    }
    else m_buttonPanel->m_bretlam1900->Enable();
}