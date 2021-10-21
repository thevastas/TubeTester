#pragma once
#include "Common.h"
#include "wx/artprov.h"
#include "id.h"

#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>

class wxBitmapFromOpenCVPanel;

namespace cv
{
    class Mat;
    class VideoCapture;
}


class CameraThread;



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


    wxPanel* m_parent;
    wxPanel* m_buttonPanel;
    
    cv::VideoCapture* m_videoCapture{ nullptr };
    CameraThread* m_cameraThread{ nullptr };

    //wxDECLARE_EVENT_TABLE();
};

