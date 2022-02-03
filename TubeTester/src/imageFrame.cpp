#include "imageFrame.h"
#include "MainWindow.h"
BEGIN_EVENT_TABLE(imageFrame, wxFrame)
EVT_BUTTON(controls::id::BIMAGECLOSE,			imageFrame::CloseFrame)
EVT_BUTTON(controls::id::BIMAGESAVE,			imageFrame::QuickSaveSnapshot)
EVT_BUTTON(controls::id::BMEASUREBLURINESS,		imageFrame::OnCalculateBlurinessFirstZone)
EVT_BUTTON(controls::id::BMEASUREBLURINESS2,	imageFrame::OnCalculateBlurinessSecondZone)
EVT_BUTTON(controls::id::BSAVEBLURINESS,		imageFrame::OnSaveBluriness)
END_EVENT_TABLE()

// A frame was retrieved from Camera.
wxDEFINE_EVENT(wxEVT_IPCAMERA_FRAME, wxThreadEvent);
// Could not retrieve a frame, consider connection to the camera lost.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EMPTY, wxThreadEvent);
// An exception was thrown in the camera thread.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EXCEPTION, wxThreadEvent);

/*!
*  Worker thread for retrieving images from the camera
*  and sending them to the main thread for display.
*/
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

/*!
*  Thread function to set an image payload 
*/
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

/*!
* Initializer for the image window, which displays the retrieved window or opens a camera to take the picture.
* Depending on the m_imagemode it also sets the directory of the images
*/
imageFrame::imageFrame(wxPanel* parent, wxString title)
	:wxFrame(parent, wxID_ANY, title, wxPoint(900,100), wxSize(700, 600))
{
	m_parent = parent;
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxImage::AddHandler(new wxJPEGHandler);

	m_bitmapPanel = new wxBitmapFromOpenCVPanel(this);
	m_bimageclose = new wxButton(this, controls::id::BIMAGECLOSE, "Close", wxPoint(0, 0), wxSize(300, 60));
	m_bimagesave = new wxButton(this, controls::id::BIMAGESAVE, "Save", wxPoint(300, 0), wxSize(300, 60));

	wxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizerButtons->Add(m_bimageclose, 0, wxEXPAND | wxALL, 5);
	sizerButtons->Add(m_bimagesave, 0, wxEXPAND | wxALL, 5);

	if (myParent->m_imagemode == myParent->ResolutionVideo) {
		m_bcalculateBluriness = new wxButton(this, controls::id::BMEASUREBLURINESS, "Bluriness 1 Zone", wxPoint(300, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateBluriness, 0, wxEXPAND | wxALL, 5);
		m_bcalculateBluriness2 = new wxButton(this, controls::id::BMEASUREBLURINESS2, "Bluriness 2 Zone", wxPoint(600, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateBluriness2, 0, wxEXPAND | wxALL, 5);
	}

	if (myParent->m_imagemode == myParent->ResolutionImage) {
		m_bcalculateBluriness = new wxButton(this, controls::id::BSAVEBLURINESS, "Save bluriness data", wxPoint(600, 0), wxSize(300, 60));
		sizerButtons->Add(m_bcalculateBluriness, 0, wxEXPAND | wxALL, 5);
		//SaveBluriness();
	}
	sizer->Add(sizerButtons);
	sizer->Add(m_bitmapPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	this->SetSizer(sizer);
	this->Layout();
	this->Center(wxBOTH);

	switch (myParent->m_imagemode) {
	case myParent->ResolutionImage:
		m_directory = directoryres;
		SetImage(myParent -> resfile); 
		m_bimagesave->Disable();
		break;
	case myParent->DefectsImage:
		m_directory = directorydef;
		SetImage(myParent->deffile);
		m_bimagesave->Disable();
		break;
	case myParent->l1300Image:
		m_directory = directory1300;
		SetImage(myParent->l1300file);
		m_bimagesave->Disable();
		break;
	case myParent->l1500Image:
		m_directory = directory1500;
		SetImage(myParent->l1500file);
		m_bimagesave->Disable();
		break;
	case myParent->l1900Image:
		m_directory = directory1900;
		SetImage(myParent->l1300file);
		m_bimagesave->Disable();
		break;
	case myParent->ResolutionVideo:
		m_directory = directoryres;
		m_bimagesave->Enable();
		break;
	case myParent->DefectsVideo:
		m_directory = directorydef;
		m_bimagesave->Enable();
		break;
	case myParent->l1300Video:
		m_directory = directory1300;
		m_bimagesave->Enable();
		break;
	case myParent->l1500Video:
		m_directory = directory1500;
		m_bimagesave->Enable();
		break;
	case myParent->l1900Video:
		m_directory = directory1900;
		m_bimagesave->Enable();
		break;
	}

	m_imagesaved = false;

	Bind(wxEVT_IPCAMERA_FRAME, &imageFrame::OnCameraFrame, this);
	Bind(wxEVT_IPCAMERA_EMPTY, &imageFrame::OnCameraEmpty, this);
	Bind(wxEVT_IPCAMERA_EXCEPTION, &imageFrame::OnCameraException, this);


}

/*!
* Function that forwards the opencv Mat for conversion into a suitable format for displaying (wxBitmap)
*/
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

/*!
* Sets the desired image on the imageframe, draws the regions of interest and shows the calculated values for bluriness/total intensity
*/
void imageFrame::SetImage(wxString id)
{
	// warning add batch number
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	//wxString path = m_directory+"/"+ wxString::Format(wxT("%i"), myParent->batchnumber)+"/" + id + ".jpg";
	long timeConvert = 0;
	long timeGet = 0;
	cap = cv::imread(id.ToStdString());

	cv::UMat capumat;
	cv::UMat capumat2;
	cv::Mat cap2;
	cv::Mat cap3;
	cv::cvtColor(cap, cap2, cv::COLOR_BGR2GRAY);
	
	cap2.copyTo(capumat);
	// Add text and area of interest
	if (myParent->m_imagemode == myParent->ResolutionImage) {
		cv::rectangle(cap, cv::Point(446, 222), cv::Point(2146, 1722), cv::Scalar(255, 255, 0), 5);
		cv::rectangle(cap, cv::Point(1146, 822), cv::Point(1446, 1122), cv::Scalar(0, 0, 255), 5);

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

		

	if (cap.empty()) {
		wxMessageBox(wxT("The file does not exist"), wxT("Warning"), wxICON_WARNING);
		return;
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

/*!
* Destroys the imageframe when close event is initiated
*/
void  imageFrame::CloseFrame(wxCommandEvent& event) {

	Clear();
	Destroy();
}


/*!
* Sets the camera settings (width, height, exposure, fps, gain) and starts the camera capture by using the opencv videocapture function
*/
bool imageFrame::StartCameraCapture(const wxSize& resolution,
	bool useMJPEG)
{
	cv::VideoCapture* cap = nullptr;

	Clear();

	{
		wxWindowDisabler disabler;
		wxBusyCursor     busyCursor;
		cap = new cv::VideoCapture(1, cv::CAP_ANY);

	}

	if (!cap->isOpened())
	{
		delete cap;
		LOG(ERROR) << "Could not connect to the camera.";
		return false;
	}

	// settings for the camera (resolution, framerate)
	m_videoCapture = cap;
	m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, 2592);
	m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 1944);
	m_videoCapture->set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
	m_videoCapture->set(cv::CAP_PROP_EXPOSURE, -5);
	m_videoCapture->set(cv::CAP_PROP_FPS, 30);
	m_videoCapture->set(cv::CAP_PROP_GAIN, 3);

	if (!StartCameraThread())

	{
		Clear();
		return false;
	}

	return true;
}


/*!
* Starts the camera thread
*/
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


/*!
* Command event that is used for starting the camera capture
*/
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

/*
* Calculates the bluriness of the area of interest in an image (if measuring_first_zone is true),
* calculates the bluriness of the whole image otherwise
*/
float imageFrame::calcBlurriness(const cv::UMat& src, bool measuring_first_zone)
{
	cv::Mat Gx, Gy;
	cv::Mat resized;
	if (measuring_first_zone) {
		src(cv::Range(222, 1722), cv::Range(446, 2146)).copyTo(resized);
	}
	else {
		src(cv::Range(822, 1122), cv::Range(1146, 1446)).copyTo(resized);
	}
	cv::Sobel(resized, Gx, CV_32F, 1, 0);
	cv::Sobel(resized, Gy, CV_32F, 0, 1);
	double normGx = cv::norm(Gx);
	double normGy = cv::norm(Gy);
	double sumSq = normGx * normGx + normGy * normGy;
	return static_cast<float>(1. / (sumSq / src.size().area() + 1e-6));
}

/*!
* Calculates the intensity sum of the area of interest in an image (if measuring_total is false),
* calculates the total intensity of the image otherwise
*/
float imageFrame::calcSumIntensity(const cv::UMat& src, bool measuring_total)
{
	float sum;
	cv::Mat resized;
	if (!measuring_total) {
		cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
		cv::Mat masked;
		cv::circle(mask, cv::Point(1296, 972), sumcircleradius, 255, -1);
		
		src.copyTo(resized, mask);
		//cv::bitwise_and(src, src, resized, mask=mask);
		//src(cv::Range(222, 1722), cv::Range(446, 2146)).copyTo(resized);
	}
	else {
		src.copyTo(resized);
	}


	return cv::sum(resized)[0];
}


/*!
* Function that is executed on every camera frame. It gets the payload from the thread, displays it and draws information and areas of interest
*/
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

	if (m_calculateBluriness) {
		cv::putText(m_ocvmat, cv::format("Bluriness Zone 1: %E", m_bluriness), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(255, 255, 0), 5);
		framecounter++;
		if (framecounter > 8) {
			framecounter = 0;
			m_calculateBluriness = false;
		}
	}
	else if (m_calculateBluriness2) {
		cv::putText(m_ocvmat, cv::format("Bluriness Zone 2: %E", m_bluriness2), cv::Point(100, 200), cv::FONT_HERSHEY_PLAIN, 8, cv::Scalar(0, 0, 255), 5);
		framecounter++;
		if (framecounter > 8) {
			framecounter = 0;
			m_calculateBluriness2 = false;
		}
	}

	if (myParent->m_imagemode == myParent->ResolutionVideo) {
		cv::rectangle(m_ocvmat, cv::Point(446, 222), cv::Point(2146, 1722), cv::Scalar(255, 255, 0), 5);
		cv::rectangle(m_ocvmat, cv::Point(1146, 822), cv::Point(1446, 1122), cv::Scalar(0, 0, 255), 5);
	}

	if (myParent->m_imagemode == myParent->l1300Video || myParent->m_imagemode == myParent->l1500Video || myParent->m_imagemode == myParent->l1900Video) {
		cv::circle(m_ocvmat, cv::Point(1296, 972), sumcircleradius, cv::Scalar(0, 0, 255), 3);
	}

	if (m_imagesaved) {
			cv::putText(m_ocvmat, "Image saved",cv::Point(100,100), cv::FONT_HERSHEY_PLAIN, 10, cv::Scalar(255,255,0),5); // WARNING ADD TEXTBOX
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

/*!
* Deletes the camera thread when the quit event is executed
*/
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

/*!
* Clears the image when the connection to the camera is lost
*/
void imageFrame::OnCameraEmpty(wxThreadEvent&)
{
	LOG(ERROR) << "Connection to the camera lost.";
	Clear();
}


/*!
* Clears the image when a thread exception is caught
*/
void imageFrame::OnCameraException(wxThreadEvent& evt)
{
	LOG(ERROR) << "Exception in the camera thread: %s" << evt.GetString();
	Clear();
}

/*!
* Clears the image and deletes the camera thread
*/
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

}

/*!
* Saves a JPEG snapshot of the displayed image. The filename is chosen by combining the batch number and the serial number of the tube
* and saved in the directory related to the measurement type, e.g. "defects", "resolution"
*/
void imageFrame::QuickSaveSnapshot(wxCommandEvent& event)
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "bkp_%Y%m%d_%H-%M-%S_");
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	wxBitmap bitmap;
	bitmap = m_bitmapPanel->GetBitmap();
	wxString path = m_directory + myParent->m_buttonPanel->m_idtext->GetLabel() + ".jpg";
	if (wxFileExists(path)) {
		wxRenameFile(path, m_directory + ss.str() + myParent->m_buttonPanel->m_idtext->GetLabel() + ".jpg");
	}
	bitmap.SaveFile(path, wxBITMAP_TYPE_JPEG);
	LOG(INFO) << "Snapshot saved at:" << path;

	switch (myParent->m_imagemode) {
	case myParent->ResolutionVideo:
		myParent->m_buttonPanel->m_bretres->Enable();
		SaveBluriness();
		break;
	case myParent->DefectsVideo:
		myParent->m_buttonPanel->m_bretdef->Enable();
		break;
	case myParent->l1300Video:
		myParent->m_buttonPanel->m_bretlam1300->Enable();
		break;
	case myParent->l1500Video:
		myParent->m_buttonPanel->m_bretlam1500->Enable();
		break;
	case myParent->l1900Video:
		myParent->m_buttonPanel->m_bretlam1900->Enable();
		break;
	}
	m_imagesaved = true;
}

/*!
* Command function to calculate the bluriness of the first zone (region of interest)
*/
void imageFrame::OnCalculateBlurinessFirstZone(wxCommandEvent& event)
{
	bool measure_first_zone = true;
	m_bluriness = calcBlurriness(m_ocvmat, measure_first_zone);
	m_calculateBluriness = true;
}


/*!
* Command function to calculate the bluriness of the second zone (whole image)
*/
void imageFrame::OnCalculateBlurinessSecondZone(wxCommandEvent& event)
{
	bool measure_first_zone = false;
	m_bluriness2 = calcBlurriness(m_ocvmat, measure_first_zone);
	m_calculateBluriness2 = true;
}

/*!
* Command event for the saving of the bluriness value into a text file
*/
void imageFrame::OnSaveBluriness(wxCommandEvent& event) {
	SaveBluriness();
}

/*!
* Saves the bluriness values for both zones into a text file, e.g. blur1=0.0001 blur2=0.002
*/
void imageFrame::SaveBluriness() {
	//m_imagemode = luminance;
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	//wxString path = m_directory + myParent->m_buttonPanel->m_idtext->GetLabel() + ".txt";

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