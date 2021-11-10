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
    enum ImageMode
    {
        Empty,
        ResolutionImage,
        DefectsImage,
        l1300Image,
        l1500Image,
        l1900Image,
        ResolutionVideo,
        DefectsVideo,
        l1300Video,
        l1500Video,
        l1900Video,
        luminance
    };

    ImageMode                     m_imagemode{ Empty };

    int frameoriginx = 1000;
    int frameoriginy = 100;
    int framesizex = 1067;
    int framesizey = 900;



    wxPanel* m_parent;
    buttonPanel* m_buttonPanel;
    
    cv::VideoCapture* m_videoCapture{ nullptr };
    CameraThread* m_cameraThread{ nullptr };
    imageFrame* m_resolutionWindow;
    imageFrame* m_defectsWindow;
    imageFrame* m_1300Window;
    imageFrame* m_1500Window;
    imageFrame* m_1900Window;
    imageFrame* m_capres;
    imageFrame* m_capdef;
    imageFrame* m_cap1300;
    imageFrame* m_cap1500;
    imageFrame* m_cap1900;
    config* m_configwindow;

    wxFileConfig* m_configfile;

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
    void SaveLuminance(wxCommandEvent& event);
    void RetrieveLuminance(wxCommandEvent& event);
    void OpenConfiguration(wxCommandEvent& event);

    wxString directoryLuminance = "D:/ADOS-Tech/metrology - Documents/img/luminance/";  // TODO move away to configs
    wxString directoryres = "D:/ADOS-Tech/metrology - Documents/img/resolution/";
    wxString directorydef = "D:/ADOS-Tech/metrology - Documents/img/defects/";
    wxString directory1300 = "D:/ADOS-Tech/metrology - Documents/img/1300/";
    wxString directory1500 = "D:/ADOS-Tech/metrology - Documents/img/1500/";
    wxString directory1900 = "D:/ADOS-Tech/metrology - Documents/img/1900/";
    int scalingFactor = 10000;
    //wxPopupWindow* aa;
    wxDECLARE_EVENT_TABLE();
};

