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
	void SetImage(wxString path);
	void QuickSaveSnapshot(wxCommandEvent& event);
	void OnCalculateBlurinessFirstZone(wxCommandEvent& event);
	void OnCalculateBlurinessSecondZone(wxCommandEvent& event);
	void CloseFrame(wxCommandEvent& event);
	void Clear();


	cv::VideoCapture* m_videoCapture{ nullptr }; //!< Initialization of OpenCV videocapture instance that is used for recording images
	CameraThread* m_cameraThread{ nullptr }; //!< Initialization of the camera thread for recording

	enum Mode 
	{
		Empty,
		Image,
		Camera
	};

	Mode                     m_mode{ Empty };

	bool m_imagesaved; //!< Was the image saved?, used for putting the saving information on the screen
	bool calculate_bluriness_first_zone; //!< Should the bluriness be calculated for the first zone
	bool calculate_bluriness_second_zone; //!< Should the bluriness be calculated for the second zone (whole image)

	wxBitmapFromOpenCVPanel* m_bitmapPanel; //!< Bitmap panel onto which the image from OpenCV is drawn
	wxBitmap ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert);  //!< Bitmap converted from an OpenCV image
	wxPanel* m_parent;
	wxButton* m_bimageclose; //!< Button for closing the imageframe window
	wxButton* m_bimagesave; //!< Button for saving the image into a respective directory
	wxButton* m_bcalculateBluriness; //!< Button for calculating the bluriness in the first zone
	wxButton* m_bcalculateBluriness2; //!< Button for calculating the bluriness in the second zone (whole image)

	wxString m_directory; //!< General holder for the path of an image, the value to which is given after determining the measurement type

	cv::Mat cap; //!< Mat Frame that was captured by OpenCV
	cv::UMat m_ocvmat; //!< Frame converted to UMat with CV_8UC3 format (8 bit, unsigned, 3 channels)

	long m_timeConvert = 0; //!< Time needed for the conversion from OpenCV frame to a WxBitmap

	std::stringstream ss; //!< Stringstream used to concatenate directory paths

	float m_bluriness; //!< Bluriness value for the first zone, used for resolution measurements
	float m_bluriness2; //!< Bluriness value for the second zone (whole image), used for resolution measurements
	float m_tsumintensity; //!< Total intensity sum of the image (whole image), used for sensitivity measurements
	float m_csumintensity; //!< Intensity sum of pixels within the defined circle of the image, used for sensitivity measurements
	int framecounter; //!< Iteration variable to count the number of frames passed
	int sumcircleradius = 800; //!< Radius of the circle within which the pixels are supposed to be summed, used for sensitivity measurements

	wxDECLARE_EVENT_TABLE();
};
