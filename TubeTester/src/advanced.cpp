#include "advanced.h"
#include "MainWindow.h"

BEGIN_EVENT_TABLE(advanced, wxFrame)
EVT_CLOSE(advanced::Close)
EVT_BUTTON(controls::id::BFINDLAMBDA1300, advanced::OnFindLambda1300)
EVT_BUTTON(controls::id::BFINDLAMBDA1500, advanced::OnFindLambda1500)
EVT_BUTTON(controls::id::BFINDLAMBDA1900, advanced::OnFindLambda1900)
EVT_BUTTON(controls::id::BFINDRESOLUTION, advanced::OnFindResolution)
EVT_BUTTON(controls::id::BFINDUMINANCE, advanced::OnFindLuminance)
//EVT_BUTTON(controls::id::BFINDDEFECTS, advanced::OnFindDefects)
END_EVENT_TABLE()

advanced::advanced(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900, 100), wxSize(250, 200))
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	m_blambda1300 = new wxButton(this, controls::id::BFINDLAMBDA1300, "Lambda 1300 summary", wxDefaultPosition, wxSize(200,60));
	m_blambda1500 = new wxButton(this, controls::id::BFINDLAMBDA1500, "Lambda 1500 summary", wxDefaultPosition, wxSize(200, 60));
	m_blambda1900 = new wxButton(this, controls::id::BFINDLAMBDA1900, "Lambda 1900 summary", wxDefaultPosition, wxSize(200, 60));
	m_bresolution = new wxButton(this, controls::id::BFINDRESOLUTION, "Resolution summary", wxDefaultPosition, wxSize(200, 60));
	m_bluminance  = new wxButton(this, controls::id::BFINDUMINANCE, "Luminance summary", wxDefaultPosition, wxSize(200, 60));
	//m_bluminance = new wxButton(this, controls::id::BFINDDEFECTS, "Defects summary", wxDefaultPosition, wxSize(200, 60));
	mainsizer->Add(m_blambda1300);
	mainsizer->Add(m_blambda1500);
	mainsizer->Add(m_blambda1900);
	mainsizer->Add(m_bresolution);
	mainsizer->Add(m_bluminance);
	this->SetSizer(mainsizer);
}

void advanced::OnFindLambda1300(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathlambda1300 = wxFindFirstFile(myParent->directory1300 + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathlambda1300summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_1300sumcir.txt"), myParent->batchnumber);
	pathlambda1300summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_1300sumtot.txt"), myParent->batchnumber);
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
		inputFile = new wxTextFile(pathlambda1300.GetFullPath());
		filenamelambda1300 = pathlambda1300.GetFullName();

		if (wxFileExists(pathlambda1300.GetFullPath())) {
			inputFile->Open();
			arrstr = wxSplit(inputFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(inputFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			inputFile->Close();
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
	pathlambda1500summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_1500sumcir.txt"), myParent->batchnumber);
	pathlambda1500summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_1500sumtot.txt"), myParent->batchnumber);
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
		inputFile = new wxTextFile(pathlambda1500.GetFullPath());
		filenamelambda1500 = pathlambda1500.GetFullName();

		if (wxFileExists(pathlambda1500.GetFullPath())) {
			inputFile->Open();
			arrstr = wxSplit(inputFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(inputFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			inputFile->Close();
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
	pathlambda1900summaryCircle = myParent->directoryAnalysis + wxString::Format(wxT("%i_1900sumcir.txt"), myParent->batchnumber);
	pathlambda1900summaryTotal = myParent->directoryAnalysis + wxString::Format(wxT("%i_1900sumtot.txt"), myParent->batchnumber);
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
		inputFile = new wxTextFile(pathlambda1900.GetFullPath());
		filenamelambda1900 = pathlambda1900.GetFullName();

		if (wxFileExists(pathlambda1900.GetFullPath())) {
			inputFile->Open();
			arrstr = wxSplit(inputFile->GetLastLine(), '=');
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(inputFile->GetPrevLine(), '=');
			circleval = wxAtoi(arrstr[1]);
			inputFile->Close();
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

void advanced::OnFindResolution(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathresolution = wxFindFirstFile(myParent->directoryres + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathresolutionsummaryfirst = myParent->directoryAnalysis + wxString::Format(wxT("%i_ressumfirst.txt"), myParent->batchnumber);
	pathresolutionsummarysecond = myParent->directoryAnalysis + wxString::Format(wxT("%i_ressumsecond.txt"), myParent->batchnumber);
	resolutionSummaryFirstFile = new wxTextFile(pathresolutionsummaryfirst);
	resolutionSummarySecondFile = new wxTextFile(pathresolutionsummarysecond);

	if (wxFileExists(pathresolutionsummaryfirst)) {
		wxRemoveFile(pathresolutionsummaryfirst);
	}
	resolutionSummaryFirstFile->Create();
	resolutionSummaryFirstFile->Open();

	if (wxFileExists(pathresolutionsummarysecond)) {
		wxRemoveFile(pathresolutionsummarysecond);
	}
	resolutionSummarySecondFile->Create();
	resolutionSummarySecondFile->Open();

	while (!pathresolution.GetFullPath().empty())
	{
		inputFile = new wxTextFile(pathresolution.GetFullPath());
		filenameresolution = pathresolution.GetFullName();

		if (wxFileExists(pathresolution.GetFullPath())) {
			inputFile->Open();
			arrstr = wxSplit(inputFile->GetLastLine(), '=');
			firstval = arrstr[1];
			totalval = wxAtoi(arrstr[1]);
			arrstr = wxSplit(inputFile->GetPrevLine(), '=');
			secondval = arrstr[1];
			circleval = wxAtoi(arrstr[1]);
			inputFile->Close();



			arrstr = wxSplit(filenameresolution, '.');
			fileID = arrstr[0];

			resolutionSummaryFirstFile->AddLine(fileID + " " + firstval);
			resolutionSummarySecondFile->AddLine(fileID + " " + secondval);




			resolutionSummaryFirstFile->Write();
			resolutionSummarySecondFile->Write();

		}
		else {
			LOG(ERROR) << "Resolution file not found.";
			wxMessageBox(wxT("Files do not exist in the resolution measurement directory"), wxT("Warning"), wxICON_WARNING);

			break;
		}
		pathresolution = wxFindNextFile();
	}

	wxMessageBox(wxT("Summary saved successfully to:\n") + pathresolutionsummaryfirst + "\n" + pathresolutionsummarysecond, wxT("Warning"), wxICON_WARNING);
	resolutionSummaryFirstFile->Close();
	resolutionSummarySecondFile->Close();
}



void advanced::OnFindLuminance(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	pathluminance = wxFindFirstFile(myParent->directoryLuminance + wxString::Format(wxT("%i/*.txt"), myParent->batchnumber));
	pathluminancesummary = myParent->directoryAnalysis + wxString::Format(wxT("%i_lumsum.txt"), myParent->batchnumber);
	luminanceSummaryFile = new wxTextFile(pathluminancesummary);

	if (wxFileExists(pathluminancesummary)) {
		wxRemoveFile(pathluminancesummary);
	}
	luminanceSummaryFile->Create();
	luminanceSummaryFile->Open();

	while (!pathluminance.GetFullPath().empty())
	{
		inputFile = new wxTextFile(pathluminance.GetFullPath());
		filenameluminance = pathluminance.GetFullName();

		if (wxFileExists(pathluminance.GetFullPath())) {
			inputFile->Open();
			arrstr = wxSplit(inputFile->GetLastLine(), '=');
			firstval = arrstr[1];
			inputFile->Close();



			arrstr = wxSplit(filenameluminance, '.');
			fileID = arrstr[0];

			luminanceSummaryFile->AddLine(fileID + " " + firstval);
			luminanceSummaryFile->Write();

		}
		else {
			LOG(ERROR) << "Luminance file not found.";
			wxMessageBox(wxT("Files do not exist in the luminance measurement directory"), wxT("Warning"), wxICON_WARNING);

			break;
		}
		pathluminance = wxFindNextFile();
	}

	wxMessageBox(wxT("Summary saved successfully to:\n") + pathluminancesummary, wxT("Warning"), wxICON_WARNING);
	luminanceSummaryFile->Close();
}


void advanced::Close(wxCloseEvent& event) {
	Destroy();
}