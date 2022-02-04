///////////////////////////////////////////////////////////////////////////////
// Name:        convertmattowxbmp.h
// Purpose:     Converts OpenCV bitmap (Mat) stored as BGR CVU8 to wxBitmap
// Author:      PB
// Created:     2020-09-16
// Copyright:   (c) 2020 PB
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
/** @file convertmattowxbmp.h*/

#ifndef CONVERTMATTOWXBMP_H
#define CONVERTMATTOWXBMP_H

// forward declarations
namespace cv { class Mat; }
class wxBitmap;

/*!
*   @brief Converts OpenCV frame to a wxBitmap
    @param[in] matBitmap  OpenCV UMat, which data must be encoded as BGR CV_8UC3, probably the most common format for OpenCV images.
    @param[in] bitmap   wxBitmap, which must be initialized to the same width and height as matBitmap and its depth must be 24.
    @return True if the conversion succeeded, False otherwise.

    On MS Windows, a MSW-optimized version is used if possible,
    the portable one otherwise. In my testing on MSW with
    3840x2160 image in the Release build, the optimized version
    was about 25% faster then the portable one. MSW-optimized version
    is used when bitmap is a DIB and its width modulo 4 is 0.
*/
bool ConvertMatBitmapTowxBitmap(const cv::UMat matBitmap, wxBitmap& bitmap);

#endif // #ifndef CONVERTMATTOWXBMP_H