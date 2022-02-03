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
    MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    ~MainWindow();
    
    void OpenResolutionImage(wxCommandEvent& event);
    void OpenDefectsImage(wxCommandEvent& event);
    void Open1300Image(wxCommandEvent& event);
    void Open1500Image(wxCommandEvent& event);
    void Open1900Image(wxCommandEvent& event);
    void CaptureResolutionImage(wxCommandEvent& event);
    void CaptureDefectsImage(wxCommandEvent& event);
    void Capture1300Image(wxCommandEvent& event);
    void Capture1500Image(wxCommandEvent& event);
    void Capture1900Image(wxCommandEvent& event);
    void SetManualID(wxCommandEvent& event);
    void ScanID(wxCommandEvent& event);
    void SaveLuminance(wxCommandEvent& event);
    void RetrieveLuminance(wxCommandEvent& event);
    void OpenConfiguration(wxCommandEvent& event);
    void SetManualBatch(wxCommandEvent& event);
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
    config* m_configwindow;

    wxFileConfig* m_configfile = new wxFileConfig(wxEmptyString, wxEmptyString, appPath, wxEmptyString, wxCONFIG_USE_LOCAL_FILE); //!< wxFileConfig instance for the local text config file
    wxString appPath; //!< String into which the application path is saved

    wxString directoryres = "D:/ADOS-Tech/metrology - Documents/img/resolution/"; //!< Default directory for the resolution images (possible duplicate, also is retrieved from config file)
    wxString directorydef = "D:/ADOS-Tech/metrology - Documents/img/defects/"; //!< Default directory for the defects images (possible duplicate, also is retrieved from config file)
    wxString directory1300 = "D:/ADOS-Tech/metrology - Documents/img/1300/"; //!< Default directory for the 1300 nm sensitivity images (possible duplicate, also is retrieved from config file)
    wxString directory1500 = "D:/ADOS-Tech/metrology - Documents/img/1500/"; //!< Default directory for the 1500 nm sensitivity images (possible duplicate, also is retrieved from config file)
    wxString directory1900 = "D:/ADOS-Tech/metrology - Documents/img/1900/"; //!< Default directory for the 1900 nm sensitivity images (possible duplicate, also is retrieved from config file)
    wxString directoryLuminance = "D:/ADOS-Tech/metrology - Documents/img/luminance/"; //!< Default directory for the luminance values (possible duplicate, also is retrieved from config file)

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

