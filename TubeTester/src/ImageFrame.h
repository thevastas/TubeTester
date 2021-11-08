#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>
#include "convertmattowxbmp.h"
#include "bmpfromocvpanel.h"

class CameraThread;

class imageFrame : public wxFrame
{
public:
	imageFrame(wxPanel* parent, wxString title);
	// Camera stuff
	cv::VideoCapture* m_videoCapture{ nullptr };
	CameraThread* m_cameraThread{ nullptr };

	bool StartIPCameraCapture(const wxSize& resolution = wxSize(),
		bool useMJPEG = false);
	bool StartIPCameraThread();
	void DeleteIPCameraThread();
	void OnIPCamera(wxCommandEvent&);
	void OnCameraFrame(wxThreadEvent& evt);
	void OnCameraEmpty(wxThreadEvent&);
	void OnCameraException(wxThreadEvent& evt);
	void Clear();
	enum Mode 
	{
		Empty,
		Image,
		IPCamera
	};

	Mode                     m_mode{ Empty };

	bool m_imagesaved;

	wxBitmapFromOpenCVPanel* m_bitmapPanel;
	wxBitmap ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert);
	void SetImage(wxString path);
	void QuickSaveSnapshot(wxCommandEvent& event);
	cv::Mat cap;
	cv::UMat m_ocvmat;
	wxPanel* m_parent;
	wxButton* m_bimageclose;
	wxButton* m_bimagesave;
	void CloseFrame(wxCommandEvent& event);
	int framecounter;
	wxString m_directory;
	wxString directoryres = "D:/ADOS-Tech/metrology - Documents/img/resolution/";
	wxString directorydef = "D:/ADOS-Tech/metrology - Documents/img/defects/";
	wxString directory1300 = "D:/ADOS-Tech/metrology - Documents/img/1300/";
	wxString directory1500 = "D:/ADOS-Tech/metrology - Documents/img/1500/";
	wxString directory1900 = "D:/ADOS-Tech/metrology - Documents/img/1900/";

	wxDECLARE_EVENT_TABLE();
};