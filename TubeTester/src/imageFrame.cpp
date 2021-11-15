#include "imageFrame.h"
#include "MainWindow.h"
BEGIN_EVENT_TABLE(imageFrame, wxFrame)
EVT_BUTTON(controls::id::BIMAGECLOSE, imageFrame::CloseFrame)
EVT_BUTTON(controls::id::BIMAGESAVE, imageFrame::QuickSaveSnapshot)
END_EVENT_TABLE()

// A frame was retrieved from Camera.
wxDEFINE_EVENT(wxEVT_IPCAMERA_FRAME, wxThreadEvent);
// Could not retrieve a frame, consider connection to the camera lost.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EMPTY, wxThreadEvent);
// An exception was thrown in the camera thread.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EXCEPTION, wxThreadEvent);

//
// Worker thread for retrieving images from WebCam or IP Camera
// and sending them to the main thread for display.
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
			(*m_camera) >> frame->matBitmap; //retrieve frame from capture
			frame->timeGet = stopWatch.Time(); //measure retrieval time

			if (!frame->matBitmap.empty()) //if successful, set payload
			{
				wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_FRAME);

				evt->SetPayload(frame);
				m_eventSink->QueueEvent(evt);
			}
			else // connection to camera lost
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
	
	wxImage::AddHandler(new wxJPEGHandler);
	m_parent = parent;
	//MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	//m_parent = this;
	m_bitmapPanel = new wxBitmapFromOpenCVPanel(this);
	m_bimageclose = new wxButton(this, controls::id::BIMAGECLOSE, "Close", wxPoint(0, 0), wxSize(300, 60));
	m_bimagesave = new wxButton(this, controls::id::BIMAGESAVE, "Save", wxPoint(300, 0), wxSize(300, 60));
	wxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizerButtons->Add(m_bimageclose, 0, wxEXPAND | wxALL, 5);
	sizerButtons->Add(m_bimagesave, 0, wxEXPAND | wxALL, 5);
	sizer->Add(sizerButtons);
	sizer->Add(m_bitmapPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	this->SetSizer(sizer);
	this->Layout();
	this->Center(wxBOTH);

	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
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
	// warning add batch number
	MainWindow* myParent = (MainWindow*)m_parent->GetParent();
	//wxString path = m_directory+"/"+ wxString::Format(wxT("%i"), myParent->batchnumber)+"/" + id + ".jpg";
	long timeConvert = 0;
	long timeGet = 0;
	cap = cv::imread(id.ToStdString());
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


void  imageFrame::CloseFrame(wxCommandEvent& event) {

	Clear();
	Destroy();
}

// IP CAMERA 

bool imageFrame::StartIPCameraCapture(const wxSize& resolution,
	bool useMJPEG)
{
	//const bool        isDefaultWebCam = address.empty();
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
	m_videoCapture->set(3, 2592);
	m_videoCapture->set(4, 1944);
	m_videoCapture->set(15, -5);
	//m_videoCapture->set(5, 30);
	m_videoCapture->set(14, 3);

	if (!StartIPCameraThread())

	{
		Clear();
		return false;
	}

	return true;
}
bool imageFrame::StartIPCameraThread()
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
void imageFrame::OnIPCamera(wxCommandEvent& event)
{
	//m_onlyZoom = true;

	if (StartIPCameraCapture())
	{
		LOG(INFO) << "Camera capture started.";
		m_mode = IPCamera;
	}
	else Clear();
	Refresh();
}
void imageFrame::OnCameraFrame(wxThreadEvent& evt)
{
	CameraThread::CameraFrame* frame = evt.GetPayload<CameraThread::CameraFrame*>();
	if (m_mode != IPCamera)
	{
		delete frame;
		return;
	}

	long     timeConvert = 0;
	frame->matBitmap.copyTo(m_ocvmat);

	if (m_imagesaved == true) {
			cv::putText(m_ocvmat, "Image saved",cv::Point(100,100), cv::FONT_HERSHEY_PLAIN, 10, cv::Scalar(255,255,0),5);
			framecounter++;
			if (framecounter > 10) {
				framecounter = 0;
				m_imagesaved = false;
			}
	}; 


	wxBitmap bitmap = ConvertMatToBitmap(m_ocvmat, timeConvert);




	if (bitmap.IsOk())
		m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
	else
		m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);


	delete frame;
}
void imageFrame::DeleteIPCameraThread()
{
	if (m_cameraThread)
	{
		LOG(ERROR) << "Camera thread deleted.";
		m_cameraThread->Delete();
		delete m_cameraThread;
		m_cameraThread = nullptr;
	}
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


void imageFrame::Clear()
{
	DeleteIPCameraThread();

	if (m_videoCapture)
	{
		delete m_videoCapture;
		m_videoCapture = nullptr;
	}

	m_mode = Empty;
	m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

}

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