#include "advanced.h"
#include "MainWindow.h"

BEGIN_EVENT_TABLE(advanced, wxFrame)
EVT_CLOSE(advanced::Close)
EVT_BUTTON(controls::id::BFINDLAMBDA1300, advanced::OnFindLambda1300)
EVT_BUTTON(controls::id::BFINDLAMBDA1500, advanced::OnFindLambda1500)
EVT_BUTTON(controls::id::BFINDLAMBDA1900, advanced::OnFindLambda1900)
END_EVENT_TABLE()

advanced::advanced(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(550, 600))
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	m_blambda1300 = new wxButton(this, controls::id::BFINDLAMBDA1300, "Lambda 1300 summary");
	m_blambda1500 = new wxButton(this, controls::id::BFINDLAMBDA1500, "Lambda 1500 summary");
	m_blambda1900 = new wxButton(this, controls::id::BFINDLAMBDA1900, "Lambda 1900 summary");
	mainsizer->Add(m_blambda1300);
	mainsizer->Add(m_blambda1500);
	mainsizer->Add(m_blambda1900);
	this->SetSizer(mainsizer);
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

	wxMessageBox(wxT("Summary saved successfully to:\n") + pathlambda1300summaryCircle + "\n" + pathlambda1300summaryTotal, wxT("Warning"), wxICON_WARNING);
	lambda1300SummaryCircleFile->Close();
	lambda1300SummaryTotalFile->Close();
}

void advanced::OnFindLambda1500(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathlambda1500 = wxFindFirstFile(myParent->directory1500 + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathlambda1500summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumcir.txt"), myParent->batchnumber);
	pathlambda1500summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumtot.txt"), myParent->batchnumber);
	lambda1500SummaryCircleFile = new wxTextFile(pathlambda1500summaryCircle);
	lambda1500SummaryTotalFile = new wxTextFile(pathlambda1500summaryTotal);

	if (wxFileExists(pathlambda1500summaryCircle)) {
		wxRemoveFile(pathlambda1500summaryCircle);
	}
	lambda1500SummaryCircleFile->Create();
	lambda1500SummaryCircleFile->Open();

	if (wxFileExists(pathlambda1500summaryTotal)) {
		wxRemoveFile(pathlambda1500summaryTotal);
	}
	lambda1500SummaryTotalFile->Create();
	lambda1500SummaryTotalFile->Open();

	while (!pathlambda1500.GetFullPath().empty())
	{
		lambdaFile = new wxTextFile(pathlambda1500.GetFullPath());
		filenamelambda1500 = pathlambda1500.GetFullName();

		if (wxFileExists(pathlambda1500.GetFullPath())) {
			lambdaFile->Open();
			arrstr = wxSplit(lambdaFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(lambdaFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			lambdaFile->Close();
			arrstr = wxSplit(filenamelambda1500, '_');
			fileID = arrstr[0] + "_" + arrstr[1];

			lambda1500SummaryCircleFile->AddLine(fileID + " " + std::to_string(totalval));
			lambda1500SummaryTotalFile->AddLine(fileID + " " + std::to_string(circleval));
			lambda1500SummaryCircleFile->Write();
			lambda1500SummaryTotalFile->Write();

		}
		else {
			LOG(ERROR) << "Lambda=1500 file not found.";
			wxMessageBox(wxT("Files do not exist in the lambda measurement directory"), wxT("Warning"), wxICON_WARNING);
			break;
		}
		pathlambda1500 = wxFindNextFile();
	}

	wxMessageBox(wxT("Summary saved successfully to:\n") + pathlambda1500summaryCircle + "\n" + pathlambda1500summaryTotal, wxT("Warning"), wxICON_WARNING);
	lambda1500SummaryCircleFile->Close();
	lambda1500SummaryTotalFile->Close();
}

void advanced::OnFindLambda1900(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathlambda1900 = wxFindFirstFile(myParent->directory1900 + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathlambda1900summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumcir.txt"), myParent->batchnumber);
	pathlambda1900summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_sumtot.txt"), myParent->batchnumber);
	lambda1900SummaryCircleFile = new wxTextFile(pathlambda1900summaryCircle);
	lambda1900SummaryTotalFile = new wxTextFile(pathlambda1900summaryTotal);

	if (wxFileExists(pathlambda1900summaryCircle)) {
		wxRemoveFile(pathlambda1900summaryCircle);
	}
	lambda1900SummaryCircleFile->Create();
	lambda1900SummaryCircleFile->Open();

	if (wxFileExists(pathlambda1900summaryTotal)) {
		wxRemoveFile(pathlambda1900summaryTotal);
	}
	lambda1900SummaryTotalFile->Create();
	lambda1900SummaryTotalFile->Open();

	while (!pathlambda1900.GetFullPath().empty())
	{
		lambdaFile = new wxTextFile(pathlambda1900.GetFullPath());
		filenamelambda1900 = pathlambda1900.GetFullName();

		if (wxFileExists(pathlambda1900.GetFullPath())) {
			lambdaFile->Open();
			arrstr = wxSplit(lambdaFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(lambdaFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			lambdaFile->Close();
			arrstr = wxSplit(filenamelambda1900, '_');
			fileID = arrstr[0] + "_" + arrstr[1];

			lambda1900SummaryCircleFile->AddLine(fileID + " " + std::to_string(totalval));
			lambda1900SummaryTotalFile->AddLine(fileID + " " + std::to_string(circleval));
			lambda1900SummaryCircleFile->Write();
			lambda1900SummaryTotalFile->Write();

		}
		else {
			LOG(ERROR) << "Lambda=1900 file not found.";
			wxMessageBox(wxT("Files do not exist in the lambda measurement directory"), wxT("Warning"), wxICON_WARNING);
			break;
		}
		pathlambda1900 = wxFindNextFile();
	}

	wxMessageBox(wxT("Summary saved successfully to:\n") + pathlambda1900summaryCircle + "\n" + pathlambda1900summaryTotal, wxT("Warning"), wxICON_WARNING);
	lambda1900SummaryCircleFile->Close();
	lambda1900SummaryTotalFile->Close();
}

void advanced::Close(wxCloseEvent& event) {
	Destroy();
}