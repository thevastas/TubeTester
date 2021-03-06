#include "imageFrame.h"
#include "MainWindow.h"
BEGIN_EVENT_TABLE(imageFrame, wxFrame)
EVT_BUTTON(controls::id::BIMAGECLOSE,			imageFrame::CloseFrame)
EVT_BUTTON(controls::id::BIMAGESAVE,			imageFrame::QuickSaveSnapshot)
EVT_BUTTON(controls::id::BMEASUREBLURINESS,		imageFrame::OnCalculateBlurinessFirstZone)
EVT_BUTTON(controls::id::BMEASUREBLURINESS2,	imageFrame::OnCalculateBlurinessSecondZone)
EVT_BUTTON(controls::id::BFINDOUTLINE,			imageFrame::OnFindOutline)
EVT_BUTTON(controls::id::BCALCULATEINTENSITY,	imageFrame::OnCalculateSumIntensity)
END_EVENT_TABLE()

// A frame was retrieved from Camera.
wxDEFINE_EVENT(wxEVT_IPCAMERA_FRAME, wxThreadEvent);
// Could not retrieve a frame, consider connection to the camera lost.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EMPTY, wxThreadEvent);
// An exception was thrown in the camera thread.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EXCEPTION, wxThreadEvent);

class CameraThread : public wxThread
{
public:
	struct CameraFrame
	{
		cv::UMat matBitmap;
		long    timeGet{ 0 };
	};

	CameraThread(wxEvtHandler* eventSink, cv::VideoCapture* camera);

protected:
	wxEvtHandler* m_eventSink{ nullptr };
	cv::VideoCapture* m_camera{ nullptr };

	ExitCode Entry() override;
};

CameraThread::CameraThread(wxEvtHandler* eventSink, cv::VideoCapture* camera)
	: wxThread(wxTHREAD_JOINABLE),
	m_eventSink(eventSink), m_camera(camera)
{
	wxASSERT(m_eventSink);
	wxASSERT(m_camera);
}

/*! Thread function to set an image payload */
wxThread::ExitCode CameraThread::Entry()
{
	wxStopWatch  stopWatch;

	while (!TestDestroy())
	{
		CameraFrame* frame = nullptr;

		try
		{
			frame = new CameraFrame;
			stopWatch.Start();
			(*m_camera) >> frame->matBitmap; // retrieve the frame from capture
			frame->timeGet = stopWatch.Time(); // measure retrieval time

			if (!frame->matBitmap.empty()) //if capturing the frame was successful, set the payload
			{
				wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_FRAME);

				evt->SetPayload(frame);
				m_eventSink->QueueEvent(evt);
			}
			else // when the connection to camera is lost
			{
				m_eventSink->QueueEvent(new wxThreadEvent(wxEVT_IPCAMERA_EMPTY));
				wxDELETE(frame);
				break;
			}
		}
		catch (const std::exception& e)
		{
			wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_EXCEPTION);

			wxDELETE(frame);
			evt->SetString(e.what());
			m_eventSink->QueueEvent(evt);
			break;
		}
		catch (...)
		{
			wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_EXCEPTION);

			wxDELETE(frame);
			evt->SetString("Unknown exception");
			m_eventSink->QueueEvent(evt);
			break;

		}


	}

	return static_cast<wxThread::ExitCode>(nullptr);
}

imageFrame::imageFrame(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900,100), wxSize(700, 600))
{
	m_parent = parent;
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxImage::AddHandler(new wxJPEGHandler);

	m_bitmapPanel = new wxBitmapFromOpenCVPanel(this);
	m_bimageclose = new wxButton(this, controls::id::BIMAGECLOSE, "Close", wxPoint(0, 0), wxSize(300, 60));

	wxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);


	if (myParent->m_imagemode != myParent->qr) {
		m_bimagesave = new wxButton(this, controls::id::BIMAGESAVE, "Save", wxPoint(300, 0), wxSize(300, 60));
		sizerButtons->Add(m_bimagesave, 0, wxEXPAND | wxALL, 5);
	}
	else {
		m_breadqr = new wxButton(this, controls::id::BREADQR, "Read QR", wxPoint(300, 0), wxSize(300, 60));
		sizerButtons->Add(m_breadqr, 0, wxEXPAND | wxALL, 5);
	}

	sizerButtons->Add(m_bimageclose, 0, wxEXPAND | wxALL, 5);
	

	if (myParent->m_imagemode == myParent->l1300Video || myParent->m_imagemode == myParent->l1500Video || myParent->m_imagemode == myParent->l1900Video) {
		m_bcalculateintensity = new wxButton(this, controls::id::BCALCULATEINTENSITY, "Calculate intensity", wxPoint(600, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateintensity, 0, wxEXPAND | wxALL, 5);
	}

	if (myParent->m_imagemode == myParent->DefectsVideo) {
		m_bfindcircle = new wxButton(this, controls::id::BFINDOUTLINE, "Find Circle", wxPoint(300, 0), wxSize(300, 60));
		sizerButtons->Add(m_bfindcircle, 0, wxEXPAND | wxALL, 5);
	}

	if (myParent->m_imagemode == myParent->ResolutionVideo) {
		m_bcalculateBluriness = new wxButton(this, controls::id::BMEASUREBLURINESS, "Bluriness 1 Zone", wxPoint(300, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateBluriness, 0, wxEXPAND | wxALL, 5);
		m_bcalculateBluriness2 = new wxButton(this, controls::id::BMEASUREBLURINESS2, "Bluriness 2 Zone", wxPoint(600, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateBluriness2, 0, wxEXPAND | wxALL, 5);
	}

	sizer->Add(sizerButtons);
	sizer->Add(m_bitmapPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	this->SetSizer(sizer);
	this->Layout();
	this->Center(wxBOTH);

	switch (myParent->m_imagemode) {
	case myParent->ResolutionImage:
		m_directory = myParent->directoryres;
		SetImage(myParent -> resfile); 
		m_bimagesave->Disable();
		break;
	case myParent->DefectsImage:
		m_directory = myParent->directorydef;
		SetImage(myParent->deffile);
		m_bimagesave->Disable();
		break;
	case myParent->l1300Image:
		m_directory = myParent->directory1300;
		SetImage(myParent->l1300file);
		m_bimagesave->Disable();
		break;
	case myParent->l1500Image:
		m_directory = myParent->directory1500;
		SetImage(myParent->l1500file);
		m_bimagesave->Disable();
		break;
	case myParent->l1900Image:
		m_directory = myParent->directory1900;
		SetImage(myParent->l1900file);
		m_bimagesave->Disable();
		break;
	case myParent->qr:
		break;
	case myParent->ResolutionVideo:
		m_directory = myParent->directoryres;
		m_bimagesave->Enable();
		break;
	case myParent->DefectsVideo:
		m_directory = myParent->directorydef;
		m_bimagesave->Enable();
		break;
	case myParent->l1300Video:
		m_directory = myParent->directory1300;
		m_bimagesave->Enable();
		break;
	case myParent->l1500Video:
		m_directory = myParent->directory1500;
		m_bimagesave->Enable();
		break;
	case myParent->l1900Video:
		m_directory = myParent->directory1900;
		m_bimagesave->Enable();
		break;
	}

	m_imagesaved = false;

	Bind(wxEVT_IPCAMERA_FRAME, &imageFrame::OnCameraFrame, this);
	Bind(wxEVT_IPCAMERA_EMPTY, &imageFrame::OnCameraEmpty, this);
	Bind(wxEVT_IPCAMERA_EXCEPTION, &imageFrame::OnCameraException, this);


}


wxBitmap imageFrame::ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert)
{
	wxCHECK(!matBitmap.empty(), wxBitmap());

	wxBitmap    bitmap(matBitmap.cols, matBitmap.rows, 24);
	bool        converted = false;
	wxStopWatch stopWatch;
	long        time = 0;

	stopWatch.Start();
	converted = ConvertMatBitmapTowxBitmap(matBitmap, bitmap);
	time = stopWatch.Time();

	if (!converted)
	{
		return wxBitmap();
	}

	timeConvert = time;
	return bitmap;
}

void imageFrame::SetImage(wxString id)
{
	cv::UMat capumat;
	cv::UMat capumat2;
	cv::Mat cap_gray;
	cv::UMat cap_color; 
	// warning add batch number
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	long timeConvert = 0;
	long timeGet = 0;
	cap = cv::imread(id.ToStdString());

	if (cap.empty()) {
		wxMessageBox(wxT("The file does not exist"), wxT("Warning"), wxICON_WARNING);
		return;
	}


	cv::cvtColor(cap, cap_gray, cv::COLOR_BGR2GRAY);
	
	cap_gray.copyTo(capumat);
	// Add text and area of interest
	if (myParent->m_imagemode == myParent->ResolutionImage) {

		cv::rectangle(m_ocvmat, cv::Point(646, 422), cv::Point(1946, 1522), cv::Scalar(255, 255, 0), 5);
		cv::rectangle(m_ocvmat, cv::Point(1196, 872), cv::Point(1396, 1072), cv::Scalar(0, 0, 255), 5);

		m_bluriness = calcBlurriness(capumat, true);
		m_bluriness2 = calcBlurriness(capumat, false);

		cv::putText(cap, cv::format("Bluriness Zone 1: %E", m_bluriness), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(255, 255, 0), 5);
		cv::putText(cap, cv::format("Bluriness Zone 2: %E", m_bluriness2), cv::Point(100, 300), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(0, 0, 255), 5);

	}
	if (myParent->m_imagemode == myParent->l1300Image || myParent->m_imagemode == myParent->l1500Image || myParent->m_imagemode == myParent->l1900Image) {
		m_tsumintensity = calcSumIntensity(capumat, true);
		m_csumintensity = calcSumIntensity(capumat, false);
		cv::circle(cap, cv::Point(1296, 972), sumcircleradius, cv::Scalar(0, 0, 255), 3);
		cv::putText(cap, cv::format("Total Intensity: %E", m_tsumintensity), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(255, 255, 0), 5);
		cv::putText(cap, cv::format("Circle Intensity: %E", m_csumintensity), cv::Point(100, 300), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(0, 0, 255), 5);
	}
	
	if (myParent->m_imagemode == myParent->DefectsImage) {
		cap.copyTo(cap_color);
		if (FindCircleCenter(cap_color)) {
			m_isCircleDrawn = true;
			cap_umat = DrawCircles(cap_color);
			cap_umat.copyTo(cap);
		}
		else wxMessageBox(wxT("The tube's outline was not found"), wxT("Warning"), wxICON_WARNING);
	}

	cap.convertTo(m_ocvmat, CV_8UC3);
	wxBitmap bitmap = ConvertMatToBitmap(m_ocvmat, timeConvert);
	if (bitmap.IsOk()) {
		LOG(INFO) << "Opened file:" << id;
		m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);
		m_bitmapPanel->SetBitmap(bitmap, timeGet, timeConvert);
	}
	else {
		LOG(ERROR) << "Failed to read the image";
		m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);
	}
}

void  imageFrame::CloseFrame(wxCommandEvent& event) {

	Clear();
	Destroy();
}

bool imageFrame::StartCameraCapture()
{
	cv::VideoCapture* cap = nullptr;
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	Clear();

	{
		wxWindowDisabler disabler;
		wxBusyCursor     busyCursor;
		if (myParent->m_imagemode == myParent->qr) {
			cap = new cv::VideoCapture(3, cv::CAP_ANY); 

		}
		else cap = new cv::VideoCapture(1, cv::CAP_ANY);

	}

	if (!cap->isOpened())
	{
		delete cap;
		LOG(ERROR) << "Could not connect to the camera.";
		return false;
	}
	if (myParent->m_imagemode == myParent->qr) {
		m_videoCapture = cap;
		m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH,640);
		m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 512);
	}
	else {
		m_videoCapture = cap;
		m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, 2592);
		m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 1944);
		m_videoCapture->set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
		m_videoCapture->set(cv::CAP_PROP_EXPOSURE, -6);
		m_videoCapture->set(cv::CAP_PROP_FPS, 30);
		m_videoCapture->set(cv::CAP_PROP_GAIN, 3);
	}


	if (!StartCameraThread())

	{
		Clear();
		return false;
	}

	return true;
}

bool imageFrame::StartCameraThread()
{
	m_cameraThread = new CameraThread(this, m_videoCapture);
	if (m_cameraThread->Run() != wxTHREAD_NO_ERROR)
	{
		delete m_cameraThread;
		m_cameraThread = nullptr;
		LOG(ERROR) << "Could not create the thread needed to retrieve the images from a camera.";
		return false;
	}

	return true;
}


void imageFrame::DeleteCameraThread()
{
	if (m_cameraThread)
	{
		LOG(ERROR) << "Camera thread deleted.";
		m_cameraThread->Delete();
		delete m_cameraThread;
		m_cameraThread = nullptr;
	}
}

void imageFrame::OnCamera(wxCommandEvent& event)
{
	if (StartCameraCapture())
	{
		LOG(INFO) << "Camera capture started.";
		m_mode = Camera;
	}
	else Clear();
	Refresh();
}

void imageFrame::OnCameraFrame(wxThreadEvent& evt)
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	CameraThread::CameraFrame* frame = evt.GetPayload<CameraThread::CameraFrame*>();
	if (m_mode != Camera)
	{
		delete frame;
		return;
	}
	frame->matBitmap.copyTo(m_ocvmat);

	if (calculate_bluriness_first_zone) {
		cv::putText(m_ocvmat, cv::format("Bluriness Zone 1: %E", m_bluriness), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(255, 255, 0), 5);
		framecounter++;
		if (framecounter > 8) {
			framecounter = 0;
			calculate_bluriness_first_zone = false;
		}
	}
	else if (calculate_bluriness_second_zone) {
		cv::putText(m_ocvmat, cv::format("Bluriness Zone 2: %E", m_bluriness2), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(0, 0, 255), 5);
		framecounter++;
		if (framecounter > 8) {
			framecounter = 0;
			calculate_bluriness_second_zone = false;
		}
	}

	if (calculate_sum_intensity) {
		cv::putText(m_ocvmat, cv::format("Total Intensity: %E", m_tsumintensity), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(255, 255, 0), 5);
		cv::putText(m_ocvmat, cv::format("Circle Intensity: %E", m_csumintensity), cv::Point(100, 300), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(0, 0, 255), 5);
		
		framecounter++;
		if (framecounter > 8) {
			framecounter = 0;
			calculate_sum_intensity = false;
		}
	}

	if (myParent->m_imagemode == myParent->DefectsVideo && m_isCircleDrawn) {
		m_ocvmat = DrawCircles(m_ocvmat);
	}

	if (myParent->m_imagemode == myParent->ResolutionVideo) {
		cv::rectangle(m_ocvmat, cv::Point(646, 422), cv::Point(1946, 1522), cv::Scalar(255, 255, 0), 5);
		cv::rectangle(m_ocvmat, cv::Point(1196, 872), cv::Point(1396, 1072), cv::Scalar(0, 0, 255), 5);
	}

	if (myParent->m_imagemode == myParent->l1300Video || myParent->m_imagemode == myParent->l1500Video) {
		
		cv::circle(m_ocvmat, cv::Point(1296, 972), sumcircleradius, cv::Scalar(0, 0, 255), 3);
	}

	if (myParent->m_imagemode == myParent->l1900Video) {

		cv::circle(m_ocvmat, cv::Point(1296, 972), sumcircleradius_1900, cv::Scalar(0, 0, 255), 3);
	}

	if (m_imagesaved) {
		cv::putText(m_ocvmat, "Image saved", cv::Point(100, 100), cv::FONT_HERSHEY_PLAIN, 10, cv::Scalar(255, 255, 0), 5); // WARNING ADD TEXTBOX
		framecounter++;
		if (framecounter > 10) {
			framecounter = 0;
			m_imagesaved = false;
		}
	};


	wxBitmap bitmap = ConvertMatToBitmap(m_ocvmat, m_timeConvert);

	if (bitmap.IsOk())
		m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, 0);
	else
		m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

	delete frame;
}

void imageFrame::OnCameraEmpty(wxThreadEvent&)
{
	LOG(ERROR) << "Connection to the camera lost.";
	Clear();
}

void imageFrame::OnCameraException(wxThreadEvent& evt)
{
	LOG(ERROR) << "Exception in the camera thread: %s" << evt.GetString();
	Clear();
}

float imageFrame::calcBlurriness(const cv::UMat& src, bool measuring_first_zone)
{
	cv::Mat Gx, Gy;
	cv::Mat resized;
	if (measuring_first_zone) {
		src(cv::Range(422, 1522), cv::Range(646, 1946)).copyTo(resized);
	}
	else {
		src(cv::Range(872, 1072), cv::Range(1196, 1396)).copyTo(resized);
	}


	cv::Sobel(resized, Gx, CV_32F, 1, 0);
	cv::Sobel(resized, Gy, CV_32F, 0, 1);
	double normGx = cv::norm(Gx);
	double normGy = cv::norm(Gy);
	double sumSq = normGx * normGx + normGy * normGy;
	return static_cast<float>(1. / (sumSq / src.size().area() + 1e-6));


}

void imageFrame::SaveLambda(wxString lambda) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();

	wxString path = m_directory + wxString::Format(wxT("%i/"), myParent->batchnumber) + myParent->m_buttonPanel->m_idtext->GetLabel() + "_l" + lambda + ".txt";

	wxTextFile* blurinessFile = new wxTextFile(path);
	if (!wxFileExists(path)) {
		blurinessFile->Create();
	}
	blurinessFile->Open();
	blurinessFile->AddLine("sens_circle=" + std::to_string(m_csumintensity));
	blurinessFile->AddLine("sens_total=" + std::to_string(m_tsumintensity));
	blurinessFile->Write();
	blurinessFile->Close();
}

void imageFrame::OnCalculateSumIntensity(wxCommandEvent& event) {
	m_tsumintensity = calcSumIntensity(m_ocvmat, true);
	m_csumintensity = calcSumIntensity(m_ocvmat, false);
	calculate_sum_intensity = true;
}

float imageFrame::calcSumIntensity(const cv::UMat& src, bool measuring_total)
{
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	float sum;
	cv::Mat resized;
	if (!measuring_total) {
		cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
		cv::Mat masked;
		if (myParent->m_imagemode == myParent->l1900Video) sumcircleradius = sumcircleradius_1900 ;
		cv::circle(mask, cv::Point(1296, 972), sumcircleradius-5, 255, -1); // subtract 5 from the sum radius to omit the red marking circle
		
		src.copyTo(resized, mask);
	}
	else {
		src.copyTo(resized);
	}


	return cv::sum(resized)[0];
}



bool imageFrame::FindCircleCenter(const cv::UMat& src) {
	bool great_success = false;
	cvtColor(src, m_grayscale_image, cv::COLOR_BGR2GRAY);
	m_grayscale_image.copyTo(m_blurred_image);
	cv::medianBlur(m_blurred_image, m_blurred_image, 5);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(m_blurred_image, circles, cv::HOUGH_GRADIENT, 1,
		m_blurred_image.rows / 4,  // change this value to detect circles with different distances to each other
		25, 40, 900, 2000 // change the last two parameters
   // (min_radius & max_radius) to detect larger circles
	);
	if (true) {
		m_circles = circles[0];
		great_success = true;
	}
	
	return great_success;
}

cv::UMat imageFrame::DrawCircles(const cv::UMat& src) { // TODO: use this function on the live view instead
	cv::Point center = cv::Point(m_circles[0], m_circles[1]);
	cv::circle(src, center, 5, cv::Scalar(0, 100, 100), 3, cv::LINE_AA); // Draw center of the circle
	int radius = m_circles[2];
	cv::circle(src, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA); // Draw outline of the circle
	//TODO: incorporate px/mm value here
	cv::circle(src, center, 569, cv::Scalar(0, 0, 255), 3); // Draw the circle of 12 mm zone
	cv::circle(src, center, 911, cv::Scalar(255, 0, 0), 3); // Draw the circle of 19.2 mm zone
	return src;
}


void imageFrame::SaveBluriness() {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();

	wxString path = m_directory + wxString::Format(wxT("%i/"), myParent->batchnumber) + myParent->m_buttonPanel->m_idtext->GetLabel() + ".txt";

	wxTextFile* blurinessFile = new wxTextFile(path);
	if (!wxFileExists(path)) {
		blurinessFile->Create();
	}
	blurinessFile->Open();
	blurinessFile->AddLine("blur1=" + std::to_string(m_bluriness));
	blurinessFile->AddLine("blur2=" + std::to_string(m_bluriness2));
	blurinessFile->Write();
	blurinessFile->Close();
}


void imageFrame::OnCalculateBlurinessFirstZone(wxCommandEvent& event)
{
	calculate_bluriness_first_zone = true;
	m_bluriness = calcBlurriness(m_ocvmat, calculate_bluriness_first_zone);
}

void imageFrame::OnCalculateBlurinessSecondZone(wxCommandEvent& event)
{
	calculate_bluriness_second_zone = true;
	m_bluriness2 = calcBlurriness(m_ocvmat, calculate_bluriness_second_zone);
}

void imageFrame::QuickSaveSnapshot(wxCommandEvent& event)
{

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	//std::stringstream ss;
	ss << std::put_time(&tm, "bkp_%Y%m%d_%H-%M-%S_");
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxBitmap bitmap;
	bitmap = m_bitmapPanel->GetBitmap();
	wxString path = m_directory + wxString::Format(wxT("%i/"), myParent->batchnumber) + myParent->m_buttonPanel->m_idtext->GetLabel() + ".jpg";
	if (wxFileExists(path)) {
		wxRenameFile(path, m_directory + ss.str() + myParent->m_buttonPanel->m_idtext->GetLabel() + ".jpg");
	}
	bitmap.SaveFile(path, wxBITMAP_TYPE_JPEG);
	LOG(INFO) << "Snapshot saved at:" << path;

	switch (myParent->m_imagemode) {
	case myParent->ResolutionVideo:
		myParent->m_buttonPanel->m_bretres->Enable();
		m_bluriness = calcBlurriness(m_ocvmat, true);
		m_bluriness2 = calcBlurriness(m_ocvmat, false);
		SaveBluriness();
		break;
	case myParent->DefectsVideo:
		myParent->m_buttonPanel->m_bretdef->Enable();
		break;
	case myParent->l1300Video:
		myParent->m_buttonPanel->m_bretlam1300->Enable();
		m_tsumintensity = calcSumIntensity(m_ocvmat, true);
		m_csumintensity = calcSumIntensity(m_ocvmat, false);
		SaveLambda("1300");
		break;
	case myParent->l1500Video:
		myParent->m_buttonPanel->m_bretlam1500->Enable();
		m_tsumintensity = calcSumIntensity(m_ocvmat, true);
		m_csumintensity = calcSumIntensity(m_ocvmat, false);
		SaveLambda("1500");
		break;
	case myParent->l1900Video:
		myParent->m_buttonPanel->m_bretlam1900->Enable();
		m_tsumintensity = calcSumIntensity(m_ocvmat, true);
		m_csumintensity = calcSumIntensity(m_ocvmat, false);
		SaveLambda("1900");
		break;
	}
	m_imagesaved = true;
}

void imageFrame::OnFindOutline(wxCommandEvent& event) {
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	if (myParent->m_imagemode == myParent->DefectsVideo) {
		m_ocvmat.copyTo(cap_umat);
	}
	if (myParent->m_imagemode == myParent->DefectsImage) {
		cap.copyTo(cap_umat);
	}
	if(FindCircleCenter(cap_umat)) m_isCircleDrawn = true;
	else wxMessageBox(wxT("The tube's outline was not found"), wxT("Warning"), wxICON_WARNING);
}

void imageFrame::Clear()
{
	DeleteCameraThread();

	if (m_videoCapture)
	{
		delete m_videoCapture;
		m_videoCapture = nullptr;
	}

	m_mode = Empty;
	m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);
	m_isCircleDrawn = false;
}