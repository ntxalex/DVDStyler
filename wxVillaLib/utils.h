/////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     Miscellaneous utilities
// Author:      Alex Thuering
// Created:	23.10.2003
// RCS-ID:      $Id: utils.h,v 1.4 2012/04/01 23:25:43 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WXVILLALIB_UTILS_H
#define WXVILLALIB_UTILS_H

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/mstream.h>

wxString wxGetAppPath();
void wxSetAppPath(wxString value);

wxString wxFindDataDirectory(wxString dir);
wxString wxFindDataFile(wxString filename);

#define wxBITMAP_FROM_MEMORY(name) wxGetBitmapFromMemory(name##_png, sizeof(name##_png))

inline wxBitmap wxGetBitmapFromMemory(const unsigned char *data, int length) {
   wxMemoryInputStream is(data, length);
   return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
}

#define wxICON_FROM_MEMORY(name) wxGetIconFromMemory(name##_png, sizeof(name##_png))

inline wxIcon wxGetIconFromMemory(const unsigned char *data, int length) {
   wxIcon icon;
   icon.CopyFromBitmap(wxGetBitmapFromMemory(data, length));
   return icon;
}

// GTK themes need native padding, but the standard button width is excessive
// for an ellipsis. EXACTFIT requests the compact native button presentation.
inline long wxGetCompactButtonStyle(const wxString& label = wxT("...")) {
#ifdef __WXGTK__
    return label == wxT("...") ? wxBU_EXACTFIT : 0;
#else
    wxUnusedVar(label);
    return 0;
#endif
}

// Preserve the original square sizing on other ports. On GTK, leave both
// dimensions to the compact button's themed best size (and allow DPI changes).
inline void wxSetCompactButtonSize(wxButton* button, int side) {
#ifndef __WXGTK__
    button->SetSizeHints(side, side, side, side);
#else
    wxUnusedVar(button);
    wxUnusedVar(side);
#endif
}

// A compact numeric field still needs native space for its entry and both
// GTK step buttons. Measure the full range with the actual font/theme instead
// of treating the legacy requested width as a hard maximum.
inline void wxFitCompactSpinCtrl(wxControl* control, int width,
        const wxString& minimum, const wxString& maximum) {
#ifdef __WXGTK__
    if (width > 0) {
        control->SetMinSize(wxDefaultSize);
        int textWidth = wxMax(control->GetTextExtent(minimum).GetWidth(),
                control->GetTextExtent(maximum).GetWidth());
        wxSize size = control->GetSizeFromTextSize(textWidth);
        if (size.GetWidth() < 0 || size.GetHeight() < 0)
            size = control->GetBestSize();
        size.SetWidth(wxMax(width, size.GetWidth()));
        control->SetInitialSize(size);
    }
#else
    wxUnusedVar(control);
    wxUnusedVar(width);
    wxUnusedVar(minimum);
    wxUnusedVar(maximum);
#endif
}

#endif // WXVILLALIB_UTILS_H
