#include "advanced.h"
#include "MainWindow.h"

BEGIN_EVENT_TABLE(advanced, wxFrame)
EVT_CLOSE(advanced::Close)
EVT_BUTTON(controls::id::BFINDLAMBDA1300, advanced::OnFindLambda1300)
END_EVENT_TABLE()

advanced::advanced(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(550, 600))
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	m_blambda1300 = new wxButton(this, controls::id::BFINDLAMBDA1300, "Lambda 1300 summary");
	mainsizer->Add(m_blambda1300);
}

void advanced::OnFindLambda1300(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathlambda1300 = wxFindFirstFile(myParent->directory1300 + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathlambda1300summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumcir.txt"), myParent->batchnumber);
	pathlambda1300summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumtot.txt"), myParent->batchnumber);
	lambda1300SummaryCircleFile = new wxTextFile(pathlambda1300summaryCircle);
	lambda1300SummaryTotalFile = new wxTextFile(pathlambda1300summaryTotal);
	
	if (wxFileExists(pathlambda1300summaryCircle)) {
		wxRemoveFile(pathlambda1300summaryCircle);
	}
		lambda1300SummaryCircleFile->Create();
		lambda1300SummaryCircleFile->Open();

	if (wxFileExists(pathlambda1300summaryTotal)) {
		wxRemoveFile(pathlambda1300summaryTotal);
	}
		lambda1300SummaryTotalFile->Create();
		lambda1300SummaryTotalFile->Open();

	while (!pathlambda1300.GetFullPath().empty())
	{
		lambdaFile = new wxTextFile(pathlambda1300.GetFullPath());
		filenamelambda1300 = pathlambda1300.GetFullName();

		if (wxFileExists(pathlambda1300.GetFullPath())) {
			lambdaFile->Open();
			arrstr = wxSplit(lambdaFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(lambdaFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			lambdaFile->Close();
			arrstr = wxSplit(filenamelambda1300, '_');
			fileID = arrstr[0] + "_" + arrstr[1];

			lambda1300SummaryCircleFile->AddLine(fileID + " " + std::to_string(totalval));
			lambda1300SummaryTotalFile->AddLine(fileID + " " + std::to_string(circleval));
			lambda1300SummaryCircleFile->Write();
			lambda1300SummaryTotalFile->Write();

		}
		else {
			LOG(ERROR) << "Lambda=1300 file not found.";
			wxMessageBox(wxT("Files do not exist in the lambda measurement directory"), wxT("Warning"), wxICON_WARNING);
			break;
		}
		pathlambda1300 = wxFindNextFile();
	}

	wxMessageBox(wxT("Summary saved successfully"), wxT("Warning"), wxICON_WARNING);
	lambda1300SummaryCircleFile->Close();
	lambda1300SummaryTotalFile->Close();
}

void advanced::Close(wxCloseEvent& event) {
	Destroy();
}