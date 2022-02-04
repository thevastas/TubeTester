///////////////////////////////////////////////////////////////////////////////
// Name:        bmpfromocvpanel.h
// Purpose:     Displays a wxBitmap originated from OpenCV
// Author:      PB
// Created:     2020-09-16
// Copyright:   (c) 2020 PB
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
/** @file bmpfromocvpanel.h*/
#ifndef BMPFROMOCVPANEL_H
#define BMPFROMOCVPANEL_H

#include <wx/wx.h>
#include <wx/print.h>
#include <wx/scrolwin.h>

/*!
*  This class displays a wxBitmap originated from OpenCV
*/ 
class wxBitmapFromOpenCVPanel : public wxScrolledCanvas
{
public:
    /*! Class initializer for wxBitmapFromOpenCVPanel. Sets the background colour, style, scrolling style   */
    wxBitmapFromOpenCVPanel(wxFrame* parent);

    wxFrame* m_parent;

    /*!
    @brief Determines the correct size for the image to be displayed on the bitmap panel
    @param[in] bitmap   wxBitmap which should be displayed
    @param[in] timeGet  bitmap retrieval time, if one needs to display it
    @param[in] timeConvert bitmap conversion time, if one neeeds to display it
    @return True if the size was determined sucessfully
    */
    bool SetBitmap(const wxBitmap& bitmap, const long timeGet, const long timeConvert);

    const wxBitmap& GetBitmap() { return m_bitmap; }

private:
    wxBitmap m_bitmap; //!< Bitmap to be displayed on the imageframe
    long     m_timeGetCVBitmap{ 0 }; //!< Time to obtain bitmap from OpenCV in ms
    long     m_timeConvertBitmap{ 0 }; //!< Time to convert Mat to wxBitmap in ms

    /*! Determines the correct size for the bitmap panel */
    wxSize DoGetBestClientSize() const override;

    /*! Paint event function, draws the image onto the bitmap panel */
    void OnPaint(wxPaintEvent&);
};

#endif // #ifndef BMPFROMOCVPANEL_H