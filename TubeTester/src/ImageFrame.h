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
	//warning
	

	bool StartCameraCapture(const wxSize& resolution = wxSize(),
		bool useMJPEG = false);
	bool StartCameraThread();
	void DeleteCameraThread();
	void OnCamera(wxCommandEvent&);
	void OnCameraFrame(wxThreadEvent& evt);
	void OnCameraEmpty(wxThreadEvent&);
	void OnCameraException(wxThreadEvent& evt);
	float calcBlurriness(const cv::UMat& src, bool measuring_first_zone);
	float calcSumIntensity(const cv::UMat& src, bool measuring_total);
	void OnSaveBluriness(wxCommandEvent& event);
	void SaveBluriness();

	void Clear();
	enum Mode 
	{
		Empty,
		Image,
		Camera
	};

	Mode                     m_mode{ Empty };

	bool m_imagesaved;
	bool m_calculateBluriness;
	bool m_calculateBluriness2;

	wxBitmapFromOpenCVPanel* m_bitmapPanel;
	wxBitmap ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert);
	void SetImage(wxString path);
	void QuickSaveSnapshot(wxCommandEvent& event);
	void OnCalculateBlurinessFirstZone(wxCommandEvent& event);
	void OnCalculateBlurinessSecondZone(wxCommandEvent& event);
	cv::Mat cap;
	cv::UMat m_ocvmat;
	cv::UMat dst;
	long m_timeConvert=0;
	float focusMeasure;
	cv::Scalar mu, sigma;
	std::stringstream socv;
	wxPanel* m_parent;
	wxButton* m_bimageclose;
	wxButton* m_bimagesave;
	wxButton* m_bcalculateBluriness;
	wxButton* m_bcalculateBluriness2;
	float m_bluriness;
	float m_bluriness2;
	float m_tsumintensity;
	float m_csumintensity;
	void CloseFrame(wxCommandEvent& event);
	int framecounter;
	int sumcircleradius = 800;
	wxString m_directory;
	wxString directoryres = "D:/ADOS-Tech/metrology - Documents/img/resolution/";
	wxString directorydef = "D:/ADOS-Tech/metrology - Documents/img/defects/";
	wxString directory1300 = "D:/ADOS-Tech/metrology - Documents/img/1300/";
	wxString directory1500 = "D:/ADOS-Tech/metrology - Documents/img/1500/";
	wxString directory1900 = "D:/ADOS-Tech/metrology - Documents/img/1900/";

	wxDECLARE_EVENT_TABLE();
};
