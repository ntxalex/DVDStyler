/////////////////////////////////////////////////////////////////////////////
// Name:        VideoFrameDlg.cpp
// Purpose:     The video frame dialog
// Author:      Alex Thuering
// Created:     23.12.2011
// RCS-ID:      $Id: VideoFrameDlg.cpp,v 1.7 2013/06/30 13:24:47 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "VideoFrameDlg.h"

//(*InternalHeaders(VideoFrameDlg)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include "Utils.h"
#include <wx/wx.h>
#include <wx/regex.h>

//(*IdInit(VideoFrameDlg)
const long VideoFrameDlg::ID_MEDIA_CTRL = wxNewId();
const long VideoFrameDlg::ID_SLIDER = wxNewId();
const long VideoFrameDlg::ID_TIME_CTRL = wxNewId();
const long VideoFrameDlg::ID_TIME_SPINB = wxNewId();
const long VideoFrameDlg::ID_FRAME_SPINBT = wxNewId();
const long VideoFrameDlg::ID_RESET_TIME_BT = wxNewId();
const long VideoFrameDlg::ID_DURATION_CTRL = wxNewId();
//*)

BEGIN_EVENT_TABLE(VideoFrameDlg,wxDialog)
	//(*EventTable(VideoFrameDlg)
	//*)
	EVT_COMMAND_SCROLL(ID_SLIDER, VideoFrameDlg::OnSliderScroll)
END_EVENT_TABLE()

VideoFrameDlg::VideoFrameDlg(wxWindow* parent, wxString fileName, bool custom, long defaultPos, long pos, int duration) {
	m_defaultPos = defaultPos;

	//(*Initialize(VideoFrameDlg)
	wxStaticText* staticText2;
	wxBoxSizer* boxSizer1;
	wxStaticText* staticText3;
	wxStdDialogButtonSizer* stdDialogButtonSizer;
	wxStaticText* staticText1;
	wxBoxSizer* mainSizer;

	Create(parent, wxID_ANY, _("Video frame"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaCtrl = new MediaCtrlFF(this, ID_MEDIA_CTRL, wxT(""), wxDefaultPosition,wxDefaultSize, 0, wxDefaultValidator, _T("ID_MEDIA_CTRL"));
	m_mediaCtrl->SetMinSize(wxSize(300, 200));
	m_mediaCtrl->SetWindowStyle(wxBORDER_NONE);
	mainSizer->Add(m_mediaCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_slider = new wxSlider(this, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER"));
	mainSizer->Add(m_slider, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	boxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	staticText1 = new wxStaticText(this, wxID_ANY, _("Time:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	boxSizer1->Add(staticText1, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_timeCtrl = new wxTextCtrl(this, ID_TIME_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIME_CTRL"));
	boxSizer1->Add(m_timeCtrl, 0, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_timeSpinBt = new wxSpinButton(this, ID_TIME_SPINB, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_TIME_SPINB"));
	m_timeSpinBt->SetRange(0, 100);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_timeSpinBt->SetMinSize(wxSize(16,12));
#endif
	boxSizer1->Add(m_timeSpinBt, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_frameSpinBt = new wxSpinButton(this, ID_FRAME_SPINBT, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_FRAME_SPINBT"));
	m_frameSpinBt->SetRange(-9999, 9999);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_frameSpinBt->SetMinSize(wxSize(16,12));
#endif
	boxSizer1->Add(m_frameSpinBt, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_resetTimeBt = new wxBitmapButton(this, ID_RESET_TIME_BT, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_UNDO")),wxART_MENU), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_RESET_TIME_BT"));
	m_resetTimeBt->SetDefault();
	m_resetTimeBt->SetToolTip(_("Reset"));
	boxSizer1->Add(m_resetTimeBt, 0, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	staticText2 = new wxStaticText(this, wxID_ANY, _("Duration:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	boxSizer1->Add(staticText2, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	m_durationCtrl = new wxSpinCtrl(this, ID_DURATION_CTRL, _T("0"), wxDefaultPosition, wxSize(60,-1), 0, 0, 999, 0, _T("ID_DURATION_CTRL"));
	m_durationCtrl->SetValue(_T("0"));
	boxSizer1->Add(m_durationCtrl, 0, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	staticText3 = new wxStaticText(this, wxID_ANY, _("sec"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	boxSizer1->Add(staticText3, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	boxSizer1->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(boxSizer1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	stdDialogButtonSizer = new wxStdDialogButtonSizer();
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	stdDialogButtonSizer->Realize();
	mainSizer->Add(stdDialogButtonSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(mainSizer);
	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);
	Center();

	Connect(ID_TIME_CTRL,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VideoFrameDlg::OnChangeTime);
	Connect(ID_TIME_SPINB,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&VideoFrameDlg::OnTimeSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoFrameDlg::OnFrameSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoFrameDlg::OnFrameSpinDown);
	Connect(ID_RESET_TIME_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VideoFrameDlg::OnResetBt);
	//*)
	stdDialogButtonSizer->GetAffirmativeButton()->SetDefault();

	int x = 0;
	m_timeCtrl->GetTextExtent(wxT("00:00:00.000"), &x, NULL);
	m_timeCtrl->SetMinSize(wxSize(x + 10, -1));

	m_mediaCtrl->Load(fileName);
	m_slider->SetMax(m_mediaCtrl->Length()/1000);
	m_timeSpinBt->SetMax(m_mediaCtrl->Length()/1000);

	SeekVideo(pos);
	m_durationCtrl->SetValue(duration);
}

VideoFrameDlg::~VideoFrameDlg() {
	//(*Destroy(VideoFrameDlg)
	//*)
}

void VideoFrameDlg::SeekVideo(long pos, bool updateTimeCtrl) {
	m_slider->SetValue(lround(pos / 1000));
	m_timeSpinBt->SetValue(lround(pos / 1000));
	m_mediaCtrl->Seek((wxFileOffset) pos);
	m_resetTimeBt->Enable(pos != m_defaultPos);
	if (updateTimeCtrl)
		m_timeCtrl->ChangeValue(Time2String(pos, true));
	m_pos = pos;
}

void VideoFrameDlg::OnSliderScroll(wxScrollEvent& event) {
	SeekVideo(((long)m_slider->GetValue())*1000);
}

void VideoFrameDlg::OnTimeSpin(wxSpinEvent& event) {
	SeekVideo(((long)m_timeSpinBt->GetValue())*1000);
}

void VideoFrameDlg::OnChangeTime(wxCommandEvent& event) {
	if (s_timeRE.Matches(m_timeCtrl->GetValue()))
		SeekVideo(String2Time(m_timeCtrl->GetValue()), false);
}

void VideoFrameDlg::OnResetBt(wxCommandEvent& event) {
	SeekVideo(m_defaultPos);
}

void VideoFrameDlg::OnFrameSpin(wxSpinEvent& event) {
	SeekVideo(m_pos + 1000 / m_mediaCtrl->GetFps());
}

void VideoFrameDlg::OnFrameSpinDown(wxSpinEvent& event) {
	long pos = m_pos - 1000 / m_mediaCtrl->GetFps();
	SeekVideo(pos >= 0 ? pos : 0);
}
