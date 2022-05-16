#pragma once
#include <wx/wx.h>
#include "Common.h"
#include "id.h"
#include <wx/scrolwin.h>
#include "convertmattowxbmp.h"
#include "bmpfromocvpanel.h"
#include <opencv2/objdetect.hpp>
/** @file ImageFrame.h*/

/*!
*  Worker thread for retrieving images from the camera
*  and sending them to the main thread for display.
*/
class CameraThread;

class imageFrame : public wxFrame
{
public:
	/*! Initializer for the image window, which displays the retrieved window or opens a camera to take the picture.
	 Depending on the m_imagemode it also sets the directory of the images */
	imageFrame(wxPanel* parent, wxString title);

	/*!
    @brief Sets the camera settings (width, height, exposure, fps, gain) and starts the camera capture by using the opencv videocapture function
	@return True if capture was opened, False otherwise
	*/
	bool StartCameraCapture();

	/*!
	@brief Function that forwards the opencv Mat for conversion into a suitable format for displaying (wxBitmap) 
	@param[in] matBitmap OpenCV UMat used for conversion
	@param[out] timeConvert Conversion time in ms
	*/
	wxBitmap ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert); 

	/*! Sets the desired image on the imageframe, draws the regions of interest and shows the calculated values for bluriness/total intensity */
	void SetImage(wxString path);

	/*! Destroys the imageframe when close event is initiated */
	void CloseFrame(wxCommandEvent& event);

	/*! Starts the camera thread */
	bool StartCameraThread();

	/*! Deletes the camera thread when the quit event is executed */
	void DeleteCameraThread();

	/*! Command event that is used for starting the camera capture */
	void OnCamera(wxCommandEvent&);

	/*! Function that is executed on every camera frame. It gets the payload from the thread, displays it and draws information and areas of interest */
	void OnCameraFrame(wxThreadEvent& evt);

	/*! Clears the image when the connection to the camera is lost */
	void OnCameraEmpty(wxThreadEvent&);

	/*! Clears the image when a thread exception is caught */
	void OnCameraException(wxThreadEvent& evt);

	/*!
	@brief Calculates the bluriness of the area of interest in an image
	@param[in] src OpenCV UMat used for calculation
	@param[in] measuring_first_zone boolean value to choose if the first or the second zone of the image should be used for calculation
	@return Float with the bluriness value of the chosen region
	*/
	float calcBlurriness(const cv::UMat& src, bool measuring_first_zone);

	/*! 
	@brief Saves the value of sensitivity at a specific wavelength for the region of interest and the whole image into a text file, e.g. sens_circle=0.0001 sens_total=0.002
	@param[in] wavelength value in nanometers, e.g. 1300, 1500, 1900 */
	void SaveLambda(wxString lambda);

	/*! Command event function for the calculation of the intensity of the image. Used to determine the sensitivity of the tubes at different wavelengths */
	void OnCalculateSumIntensity(wxCommandEvent& event);

	/*!
	@brief Calculates the intensity sum of the circle of interest in an image (if measuring_total is false), calculates the total intensity of the image otherwise
	@param[in] src OpenCV UMat used for calculation
	@param[in] measuring_first_zone boolean value to choose if the first or the whole image should be used for calculation
	@return Float with the intensity sum of the chosen region
	*/
	float calcSumIntensity(const cv::UMat& src, bool measuring_total);

	/*!
	@brief Finds the center of the tube from the edges of the glass by using the Hough Line Transform
	@param[in] src OpenCV Mat image
	@return True if a circle was found, False otherwise
	*/
	bool FindCircleCenter(const cv::UMat& src);

	/*!
	@brief Draws defect zones
	@param[in] src OpenCV UMat image
	@return OpenCV UMat image with drawn circles
	*/
	cv::UMat DrawCircles(const cv::UMat& src);

	/*! Command event for the saving of the bluriness value into a text file */
	//void OnSaveBluriness(wxCommandEvent& event);

	/*! Saves the bluriness values for both zones into a text file, e.g. blur1=0.0001 blur2=0.002 */
	void SaveBluriness();
	
	/*! Command function to calculate the bluriness of the first zone (region of interest) */
	void OnCalculateBlurinessFirstZone(wxCommandEvent& event);

	/*! Command function to calculate the bluriness of the second zone (region of interest) */
	void OnCalculateBlurinessSecondZone(wxCommandEvent& event);

	/*! Saves a JPEG snapshot of the displayed image. The filename is chosen by combining the batch number and the serial number of the tube and saved in the directory related to the measurement type, e.g. "defects", "resolution" */
	void QuickSaveSnapshot(wxCommandEvent& event);

	/*! Command function to find the outline circle of the tube */
	void OnFindOutline(wxCommandEvent& event);

	//void OnScanQR(wxCommandEvent& event);
	//cv::UMat displayBox(cv::UMat& im, cv::Mat& bbox);
	//bool m_qrfound = false;
	//std::vector<cv::Point> m_bbox;
	//cv::UMat m_qrRectified;
	//cv::QRCodeDetector qrDecoder = cv::QRCodeDetector::QRCodeDetector();


	/*! Clears the image and deletes the camera thread */
	void Clear();


	cv::VideoCapture* m_videoCapture{ nullptr };	//!< Initialization of OpenCV videocapture instance that is used for recording images
	CameraThread* m_cameraThread{ nullptr };		//!< Initialization of the camera thread for recording

	/*! enum to switch between image retrieval (image) and capture (camera) */
	enum Mode 
	{
		Empty,
		Image,
		Camera
	};

	Mode                     m_mode{ Empty };

	bool m_imagesaved;						//!< Was the image saved?, used for putting the saving information on the screen
	bool calculate_bluriness_first_zone;	//!< Should the bluriness be calculated for the first zone
	bool calculate_bluriness_second_zone;	//!< Should the bluriness be calculated for the second zone (whole image)

	wxBitmapFromOpenCVPanel* m_bitmapPanel; //!< Bitmap panel onto which the image from OpenCV is drawn
	
	wxPanel* m_parent;
	wxButton* m_bimageclose;			//!< Button for closing the imageframe window
	wxButton* m_bimagesave;				//!< Button for saving the image into a respective directory
	wxButton* m_breadqr;				//!< Button for reading the QR code of the tube
	wxButton* m_bcalculateBluriness;	//!< Button for calculating the bluriness in the first zone
	wxButton* m_bcalculateBluriness2;	//!< Button for calculating the bluriness in the second zone (whole image)
	wxButton* m_bfindcircle;			//!< Button for finding the outline of the tube in the defect measurement mode
	wxButton* m_bcalculateintensity;	//!< Button for calculating the intensity of the whole image or it's region of interest

	wxString m_directory;				//!< General holder for the path of an image, the value to which is given after determining the measurement type

	cv::Mat cap;						//!< Mat Frame that was captured by OpenCV
	//cv::VideoCapture cap2 = cv::VideoCapture("rtsp://192.168.30.159:8554/mjpeg/1", cv::CAP_FFMPEG);
	cv::UMat cap_umat;
	cv::UMat m_ocvmat;					//!< Frame converted to UMat with CV_8UC3 format (8 bit, unsigned, 3 channels)

	long m_timeConvert = 0;				//!< Time needed for the conversion from OpenCV frame to a WxBitmap

	std::stringstream ss;				//!< Stringstream used to concatenate directory paths

	float m_bluriness;					//!< Bluriness value for the first zone, used for resolution measurements
	float m_bluriness2;					//!< Bluriness value for the second zone (whole image), used for resolution measurements
	float m_tsumintensity;				//!< Total intensity sum of the image (whole image), used for sensitivity measurements
	float m_csumintensity;				//!< Intensity sum of pixels within the defined circle of the image, used for sensitivity measurements
	bool calculate_sum_intensity = false;	//!< Boolean variable for choosing if the intensity of the image should be calculated
	int framecounter;					//!< Iteration variable to count the number of frames passed
	int sumcircleradius = 700;			//!< Radius of the circle within which the pixels are supposed to be summed, used for 1300,1500 nm sensitivity measurements
	int sumcircleradius_1900 = 400;			//!< Radius of the circle within which the pixels are supposed to be summed, used for 1900 nm sensitivity measurements

	cv::Mat m_blurred_image;			//!< Blurred image that is used for the Hough Circle detection
	cv::Mat m_grayscale_image;			//!< Grayscale image that is used for the Hough Circle detection
	cv::Vec3i m_circles;				//!< Coordinates and the radius of the tube's circle
	bool m_isCircleDrawn = false;		//!< boolean variable for choosing if the outlining circle should be drawn in live view

	wxDECLARE_EVENT_TABLE();
};
