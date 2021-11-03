///////////////////////////////////////////////////////////////////////////////
// Name:        bmpfromocvpanel.cpp
// Purpose:     Displays a wxBitmap originated from OpenCV
// Author:      PB
// Created:     2020-09-16
// Copyright:   (c) 2020 PB
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/fontdlg.h>

#include "bmpfromocvpanel.h"

//wxBitmapFromOpenCVPanel::wxBitmapFromOpenCVPanel(wxWindow* parent)
wxBitmapFromOpenCVPanel::wxBitmapFromOpenCVPanel(wxFrame* parent)
    : wxScrolledCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_parent = parent;
    m_overlayTextColour = *wxGREEN;
    m_overlayFont = GetFont();

    SetBackgroundColour(*wxBLACK);
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    SetScrollRate(FromDIP(8), FromDIP(8));
    // We need to do this to prevent drawing artefacts
    // due to the info "overlay" which does not scroll with the bitmap.
    EnableScrolling(false, false);

    Bind(wxEVT_PAINT, &wxBitmapFromOpenCVPanel::OnPaint, this);

    //Bind(wxEVT_LEFT_DCLICK, &wxBitmapFromOpenCVPanel::OnChangeOverlayTextColour, this);
    //Bind(wxEVT_RIGHT_DCLICK, &wxBitmapFromOpenCVPanel::OnChangeOverlayFont, this);
}

bool wxBitmapFromOpenCVPanel::SetBitmap(const wxBitmap& bitmap, const long timeGet, const long timeConvert)
{
    m_bitmap = bitmap;

    if (m_bitmap.IsOk())
    {
        if (m_bitmap.GetSize() != GetVirtualSize())
        {
            InvalidateBestSize();
            SetVirtualSize(m_bitmap.GetSize());
        }
    }
    else
    {
        InvalidateBestSize();
        SetVirtualSize(1, 1);
    }

    m_timeGetCVBitmap = timeGet;
    m_timeConvertBitmap = timeConvert;

    Refresh(); Update();
    //if (timeConvert < 30) Sleep(30 - timeConvert);
    return true;
}

wxSize wxBitmapFromOpenCVPanel::DoGetBestClientSize() const
{
    if (!m_bitmap.IsOk())
        return FromDIP(wxSize(64, 48));

    const int maxWidth = FromDIP(800);
    const int maxHeight = FromDIP(600);

    wxSize size;

    size.SetWidth(wxMin(maxWidth, m_bitmap.GetWidth()));
    size.SetHeight(wxMin(maxHeight, m_bitmap.GetHeight()));

    return size;
}

void wxBitmapFromOpenCVPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);

    if (!m_bitmap.IsOk())
    {
        dc.Clear();
        return;
    }

    const wxSize clientSize = GetClientSize();
    wxPoint      offset = GetViewStart();
    int          pixelsPerUnitX = 0, pixelsPerUnitY = 0;
    wxStopWatch  stopWatch;

    stopWatch.Start();

    DoPrepareDC(dc);



        float fWScale = 1.0f;   // horizontal scaling factor
        float fHScale = 1.0f;   // vertical scaling factor
        int iImageH = -1;       // the bitmap's height
        int iImageW = -1;       // the bitmap's width
        int iThisH = -1;        // the panel's height
        int iThisW = -1;        // the panel's width

        // how is the bitmap's actual size?
        iImageH = m_bitmap.GetHeight();
        iImageW = m_bitmap.GetWidth();

        //Panel size
        GetSize(&iThisW, &iThisH);

        // no division by zero !
        if ((iImageH > 0) && (iImageW > 0))
        {
            // calculate the scaling factor for the 2 dimensions
            fHScale = (float)iThisH / (float)iImageH;
            fWScale = (float)iThisW / (float)iImageW;

            // always take the smaller scaling factor,
            // so that the bitmap will always fit into the panel's paintable area
            if (fHScale < fWScale)
            {
                fWScale = fHScale;
            }
            else
            {
                fHScale = fWScale;
            }

        }

        dc.SetUserScale(fHScale, fWScale);







    dc.DrawBitmap(m_bitmap, 0, 0, false);

    GetScrollPixelsPerUnit(&pixelsPerUnitX, &pixelsPerUnitY);
    offset.x *= pixelsPerUnitX; offset.y *= pixelsPerUnitY;

    // fill the right part of the client area possibly not covered by the bitmap
    if (m_bitmap.GetWidth() - offset.x < clientSize.GetWidth())
    {
        wxDCPenChanger   penChanger(dc, *wxBLACK_PEN);
        wxDCBrushChanger brushChanger(dc, *wxBLACK_BRUSH);
        wxRect           r;

        r.SetX(m_bitmap.GetWidth());
        r.SetY(offset.y);
        r.SetWidth(clientSize.GetWidth() - r.GetX() + offset.x);
        r.SetHeight(clientSize.GetHeight());

        dc.DrawRectangle(r);
    }

    // fill the bottom part of the client area possibly not covered by the bitmap
    if (m_bitmap.GetHeight() - offset.y < clientSize.GetHeight())
    {
        wxDCPenChanger   penChanger(dc, *wxBLACK_PEN);
        wxDCBrushChanger brushChanger(dc, *wxBLACK_BRUSH);
        wxRect           r;

        r.SetX(offset.x);
        r.SetY(m_bitmap.GetHeight());
        r.SetWidth(m_bitmap.GetWidth());
        r.SetHeight(clientSize.GetHeight() - r.GetY() + offset.y);

        dc.DrawRectangle(r);
    }

    // Draw info "overlay", always at the top left corner of the window
    // regardless of how the bitmap is scrolled.
    const long            drawTime = stopWatch.Time();
    wxDCTextColourChanger textColourChanger(dc, m_overlayTextColour);
    wxDCFontChanger       fontChanger(dc, m_overlayFont);

    //dc.DrawText(wxString::Format("GetCVBitmap: %ld ms\nConvertCVtoWXBitmap: %ld ms\nDrawWXBitmap: %ld ms\n",
    //    m_timeGetCVBitmap, m_timeConvertBitmap, drawTime),
    //    offset);
}


void wxBitmapFromOpenCVPanel::OnChangeOverlayTextColour(wxMouseEvent&)
{
    const wxColour colour = wxGetColourFromUser(this, m_overlayTextColour,
        "Color for text overlay");

    if (!colour.IsOk())
        return;

    m_overlayTextColour = colour;
    Refresh(); Update();
}

void wxBitmapFromOpenCVPanel::OnChangeOverlayFont(wxMouseEvent&)
{
    const wxFont font = wxGetFontFromUser(this, m_overlayFont,
        "Font for text overlay");

    if (!font.IsOk())
        return;

    m_overlayFont = font;
    Refresh(); Update();
}