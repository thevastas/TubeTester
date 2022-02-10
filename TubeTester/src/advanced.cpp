#include "advanced.h"
#include "MainWindow.h"

BEGIN_EVENT_TABLE(advanced, wxFrame)
EVT_CLOSE(advanced::Close)
END_EVENT_TABLE()

advanced::advanced(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(550, 600))
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
}

void advanced::Close(wxCloseEvent& event) {
	Destroy();
}