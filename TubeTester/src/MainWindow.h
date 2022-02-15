#pragma once
#include "Common.h"
#include "wx/artprov.h"
#include "id.h"
#include "config.h"
#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include "ImageFrame.h"
#include "buttonPanel.h"
#include "advanced.h"

class wxBitmapFromOpenCVPanel;

namespace cv
{
    class Mat;
    class VideoCapture;
}


class CameraThread;
class buttonPanel;


class MainWindow : public wxFrame
{
public:
    /*! Initializer for the main window, onto which all panels are drawn */
    MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    ~MainWindow();

    /*! Command event handler to retrieve a resolution image determined by the serial number of the tube */
    void OpenResolutionImage(wxCommandEvent& event);

    /*! Command event handler to retrieve a defects image determined by the serial number of the tube */
    void OpenDefectsImage(wxCommandEvent& event);

    /*! Command event to retrieve a 1300 nm wavelength intensity image determined by the serial number of the tube */
    void Open1300Image(wxCommandEvent& event);

    /*! Command event to retrieve a 1500 nm wavelength intensity image determined by the serial number of the tube */
    void Open1500Image(wxCommandEvent& event);

    /*! Command event to retrieve a 1900 nm wavelength intensity image determined by the serial number of the tube */
    void Open1900Image(wxCommandEvent& event);

    /*! Command event to start capturing a resolution image for the tube determined by the serial number */
    void CaptureResolutionImage(wxCommandEvent& event);

    /*! Command event to start capturing a defects image for the tube determined by the serial number */
    void CaptureDefectsImage(wxCommandEvent& event);

    /*! Command event to start capturing a 1300 nm wavelength intensity image for the tube determined by the serial number */
    void Capture1300Image(wxCommandEvent& event);

    /*! Command event to start capturing a 1500 nm wavelength intensity image for the tube determined by the serial number */
    void Capture1500Image(wxCommandEvent& event);

    /*! Command event to start capturing a 1900 nm wavelength intensity image for the tube determined by the serial number */
    void Capture1900Image(wxCommandEvent& event);

    /*! Command event to save the luminance value that is entered into the user interface. The value is prepended to the top of a text file if several measurements are taken, e.g. lum=200 lum=198 */
    void SaveLuminance(wxCommandEvent& event);

    /*! Retrieves the first luminance value that is found in the text file according to the serial number of the tube and displays it onto the user interface */
    void RetrieveLuminance(wxCommandEvent& event);

    /*! Sets the serial number (ID) of the tube manually, using the value that was entered into the user interface */
    void SetManualID(wxCommandEvent& event);

    /*! TODO: automatic scanning of the QR code on the tube to retrieve the serial number */
    void ScanID(wxCommandEvent& event);

    /*! Reads the configuration file and sets the correct paths of image directories */
    void ReadConfig();

    /*! Command event to open the configuration window */
    void OpenConfiguration(wxCommandEvent& event);

    /*! Command event to open the advanced functionality window */
    void OpenAdvanced(wxCommandEvent& event);

    /*!
    * Sets the batch number of the tube manually, using the value that was entered into the user interface. Note: The batch is meant as the measurement batch, e.g.
    * the batch number is the same for all tubes that were measured on one go/day,etc. - it is not the manufacturing batch. For old measurements of the pre-tubetester era
    * batch number 0 is used.
    */
    void SetManualBatch(wxCommandEvent& event);

    /*! Enables or disables the user interface buttons depending if the files exist for retrieval */
    void UpdateButtons();
    
    enum ImageMode
    {
        Empty,              //!< Mode for when the image is not retrieved
        ResolutionImage,    //!< Mode for retrieving the resolution measurement image
        DefectsImage,       //!< Mode for retrieving the defects measurement image
        l1300Image,         //!< Mode for retrieving the 1300 nm wavelength sensitivity image
        l1500Image,         //!< Mode for retrieving the 1500 nm wavelength sensitivity image
        l1900Image,         //!< Mode for retrieving the 1900 nm wavelength sensitivity image
        ResolutionVideo,    //!< Mode for recording the resolution image
        DefectsVideo,       //!< Mode for recording the defects image
        l1300Video,         //!< Mode for recording the 1300 nm wavelength sensitivity image
        l1500Video,         //!< Mode for recording the 1500 nm wavelength sensitivity image
        l1900Video,         //!< Mode for recording the 1900 nm wavelength sensitivity image
        luminance           //!< Mode for recording or retrieving the luminance values
    };

    ImageMode                     m_imagemode{ Empty };

    int frameoriginx = 1000;    //!< Horizontal starting position of the configuration window
    int frameoriginy = 100;     //!< Vertical starting position of the configuration window
    int framesizex = 1067;      //!< Starting width of the configuration window
    int framesizey = 900;       //!< Starting heigth of the configuration window

    wxPanel* m_parent;
    buttonPanel* m_buttonPanel; //!< Panel in which all the buttons of the main window are stored
    
    imageFrame* m_resolutionWindow; //!< imageframe instance (window) for retrieving the resolution image
    imageFrame* m_defectsWindow;    //!< imageframe instance (window) for retrieving the defects image
    imageFrame* m_1300Window;       //!< imageframe instance (window) for retrieving the 1300 nm wavelength sensitivity image
    imageFrame* m_1500Window;       //!< imageframe instance (window) for retrieving the 1500 nm wavelength sensitivity image
    imageFrame* m_1900Window;       //!< imageframe instance (window) for retrieving the 1900 nm wavelength sensitivity image
    imageFrame* m_capres;           //!< imageframe instance (window) for recording of the resolution image
    imageFrame* m_capdef;           //!< imageframe instance (window) for recording of the defects image
    imageFrame* m_cap1300;          //!< imageframe instance (window) for recording of the 1300 nm wavelength sensitivity image
    imageFrame* m_cap1500;          //!< imageframe instance (window) for recording of the 1500 nm wavelength sensitivity image
    imageFrame* m_cap1900;          //!< imageframe instance (window) for recording of the 1900 nm wavelength sensitivity image
    config* m_configwindow;         //!< config instance (window) for setting the configuration of the application
    advanced* m_advancedwindow;     //!< advanced instance (window) for advanced functionality

    wxFileConfig* m_configfile = new wxFileConfig(wxEmptyString, wxEmptyString, configPath, wxEmptyString, wxCONFIG_USE_LOCAL_FILE); //!< wxFileConfig instance for the local text config file
    wxString configPath; //!< String into which the configuration file path is saved

    wxString directoryres = "D:/ADOS-Tech/metrology - Documents/img/resolution/"; //!< Default directory for the resolution images
    wxString directorydef = "D:/ADOS-Tech/metrology - Documents/img/defects/"; //!< Default directory for the defects images
    wxString directory1300 = "D:/ADOS-Tech/metrology - Documents/img/1300/"; //!< Default directory for the 1300 nm sensitivity images
    wxString directory1500 = "D:/ADOS-Tech/metrology - Documents/img/1500/"; //!< Default directory for the 1500 nm sensitivity images
    wxString directory1900 = "D:/ADOS-Tech/metrology - Documents/img/1900/"; //!< Default directory for the 1900 nm sensitivity images
    wxString directoryLuminance = "D:/ADOS-Tech/metrology - Documents/img/luminance/"; //!< Default directory for the luminance values
    wxString directoryAnalysis = "D:/ADOS-Tech/metrology - Documents/img/analysis/"; //!< Default directory for the analysis files

    wxString lumfile;   //!< Full path to the luminance file
    wxString resfile;   //!< Full path to the resolution image file
    wxString deffile;   //!< Full path to the defects image file
    wxString l1300file; //!< Full path to the 1300 nm wavelength sensitivity file
    wxString l1500file; //!< Full path to the 1500 nm wavelength sensitivity file
    wxString l1900file; //!< Full path to the 1900 nm wavelength sensitivity file

    int batchnumber = 0;    //!< Initial value for the measurement batch number is set to zero, which is default for test/old measurements
    int scalingFactor = 10000; //!< Default value for the scaling factor
    wxDECLARE_EVENT_TABLE();
};

