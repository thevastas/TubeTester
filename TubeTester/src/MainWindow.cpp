#include "Common.h"
#include <wx/wx.h>
#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>
#include <wx/slider.h>
#include <wx/textdlg.h>
#include <wx/thread.h>
#include <wx/utils.h>
//#include "bmpfromocvpanel.h"
//#include "convertmattowxbmp.h"
#include "buttonPanel.h"
#include "MainWindow.h"

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


    //main panel
    m_parent = new wxPanel(this, wxID_ANY);

    SetBackgroundColour(wxColor(32, 32, 32));
    //wxMenuBar* menuBar = new wxMenuBar();


    m_buttonPanel = new buttonPanel(m_parent);
    m_buttonPanel->SetBackgroundColour(wxColor(64, 64, 64));


    SetMinClientSize(FromDIP(wxSize(1000, 800)));
    SetSize(FromDIP(wxSize(1000, 800)));

}


MainWindow::~MainWindow()
{

}

