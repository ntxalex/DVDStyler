/////////////////////////////////////////////////////////////////////////////
// Name:        ChaptersDlg.cpp
// Purpose:     The chapters dialog
// Author:      Alex Thuering
// Created:     19.04.2010
// RCS-ID:      $Id: ChaptersDlg.cpp,v 1.13 2016/05/16 21:13:35 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "ChaptersDlg.h"
#include "TitlePropDlg.h"
#include "Utils.h"
#include "Config.h"
#include <wx/regex.h>
#include <wx/imaglist.h>
#include <wxVillaLib/utils.h>
#include <wxVillaLib/rc/video.png.h>
#include "rc/add.png.h"
#include "rc/remove.png.h"
#include "rc/preferences.png.h"
#include "Utils.h"

//(*InternalHeaders(ChaptersDlg)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(ChaptersDlg)
const long ChaptersDlg::ID_THUMBNAILS = wxNewId();
const long ChaptersDlg::ID_ADD_BT = wxNewId();
const long ChaptersDlg::ID_DEL_BT = wxNewId();
const long ChaptersDlg::ID_PANEL1 = wxNewId();
const long ChaptersDlg::ID_MEDIA_CTRL = wxNewId();
const long ChaptersDlg::ID_SLIDER = wxNewId();
const long ChaptersDlg::ID_TIME_CTRL = wxNewId();
const long ChaptersDlg::ID_TIME_SPINB = wxNewId();
const long ChaptersDlg::ID_FRAME_SPINBT = wxNewId();
const long ChaptersDlg::ID_RADIOBUTTON1 = wxNewId();
const long ChaptersDlg::ID_RADIOBUTTON2 = wxNewId();
const long ChaptersDlg::ID_RADIOBUTTON3 = wxNewId();
const long ChaptersDlg::ID_STATICTEXT1 = wxNewId();
const long ChaptersDlg::ID_CHOICE1 = wxNewId();
const long ChaptersDlg::ID_END_CTRL = wxNewId();
const long ChaptersDlg::ID_END_SPIN = wxNewId();
const long ChaptersDlg::ID_END_FRAME_SPINBT = wxNewId();
const long ChaptersDlg::ID_DURATION_CTRL = wxNewId();
const long ChaptersDlg::ID_COMMANDS_CTRL = wxNewId();
const long ChaptersDlg::ID_PANEL2 = wxNewId();
const long ChaptersDlg::ID_SPLITTERWINDOW = wxNewId();
const long ChaptersDlg::ID_ADD_CELL_CHECK = wxNewId();
//*)

BEGIN_EVENT_TABLE(ChaptersDlg, wxDialog)
	//(*EventTable(ChaptersDlg)
	//*)
	EVT_COMMAND_SCROLL(ID_SLIDER, ChaptersDlg::OnSliderScroll)
END_EVENT_TABLE()

ChaptersDlg::ChaptersDlg(wxWindow* parent, DVD* dvd, int tsi, int pgci, int vobi, Vob* vob) {
	m_dvd = dvd;
	m_tsi = tsi;
	m_pgci = pgci;
	m_vobi = vobi;
	m_vob = vob;
	VECTOR_COPY(m_vob->GetCells(), m_cells, Cell);
	m_selectedCell = NULL;

	//(*Initialize(ChaptersDlg)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxPanel* panel1;
	wxStaticText* staticText2;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* boxSizer1;
	wxStaticText* staticText5;
	wxStaticText* staticText4;
	wxSplitterWindow* splitterWindow;
	wxBoxSizer* boxSizer2;
	wxStaticText* staticText3;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* flexGridSizer1;
	wxStdDialogButtonSizer* stdDialogButtonSizer;
	wxStaticText* staticText1;
	wxBoxSizer* BoxSizer1;
	wxPanel* panel2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* mainSizer;

	Create(parent, wxID_ANY, _("Chapters"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(800,450));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	splitterWindow = new wxSplitterWindow(this, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_3DSASH, _T("ID_SPLITTERWINDOW"));
	splitterWindow->SetMinSize(wxSize(50,50));
	splitterWindow->SetMinimumPaneSize(50);
	splitterWindow->SetSashGravity(0.5);
	panel1 = new wxPanel(splitterWindow, ID_PANEL1, wxPoint(47,173), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	staticText1 = new wxStaticText(panel1, wxID_ANY, _("Chapters:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer2->Add(staticText1, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	m_thumbnails = new wxThumbnails(panel1,ID_THUMBNAILS);
	BoxSizer3->Add(m_thumbnails, 1, wxEXPAND, 5);
	boxSizer2 = new wxBoxSizer(wxVERTICAL);
	m_addBt = new wxBitmapButton(panel1, ID_ADD_BT, wxBITMAP_FROM_MEMORY(add), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ADD_BT"));
	boxSizer2->Add(m_addBt, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_delBt = new wxBitmapButton(panel1, ID_DEL_BT, wxBITMAP_FROM_MEMORY(remove), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_DEL_BT"));
	boxSizer2->Add(m_delBt, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer3->Add(boxSizer2, 0, wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer3, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);
	panel1->SetSizer(BoxSizer2);
	BoxSizer2->Fit(panel1);
	BoxSizer2->SetSizeHints(panel1);
	panel2 = new wxPanel(splitterWindow, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	m_mediaCtrl = new MediaCtrlFF(panel2, ID_MEDIA_CTRL, wxT(""), wxDefaultPosition,wxDefaultSize, 0, wxDefaultValidator, _T("ID_MEDIA_CTRL"));
	m_mediaCtrl->SetMinSize(wxSize(300, 200));
	m_mediaCtrl->SetWindowStyle(wxBORDER_NONE);
	BoxSizer4->Add(m_mediaCtrl, 1, wxALL|wxEXPAND, 4);
	m_slider = new wxSlider(panel2, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER"));
	BoxSizer4->Add(m_slider, 0, wxEXPAND, 2);
	flexGridSizer1 = new wxFlexGridSizer(0, 2, 2, 4);
	flexGridSizer1->AddGrowableCol(1);
	StaticText1 = new wxStaticText(panel2, wxID_ANY, _("Time:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	flexGridSizer1->Add(StaticText1, 0, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	m_timeCtrl = new wxTextCtrl(panel2, ID_TIME_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIME_CTRL"));
	BoxSizer5->Add(m_timeCtrl, 0, wxEXPAND, 5);
	m_timeSpinBt = new wxSpinButton(panel2, ID_TIME_SPINB, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_TIME_SPINB"));
	m_timeSpinBt->SetRange(0, 100);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_timeSpinBt->SetMinSize(wxSize(16,12));
#endif
	BoxSizer5->Add(m_timeSpinBt, 0, wxEXPAND, 5);
	m_frameSpinBt = new wxSpinButton(panel2, ID_FRAME_SPINBT, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_FRAME_SPINBT"));
	m_frameSpinBt->SetRange(-9999, 9999);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_frameSpinBt->SetMinSize(wxSize(16,12));
#endif
	BoxSizer5->Add(m_frameSpinBt, 0, wxEXPAND, 5);
	flexGridSizer1->Add(BoxSizer5, 0, wxALL|wxEXPAND, 0);
	staticText5 = new wxStaticText(panel2, wxID_ANY, _("Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	flexGridSizer1->Add(staticText5, 0, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 4);
	boxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_chapterBt = new wxRadioButton(panel2, ID_RADIOBUTTON1, _("Chapter"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	boxSizer1->Add(m_chapterBt, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_programBt = new wxRadioButton(panel2, ID_RADIOBUTTON2, _("Program"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	boxSizer1->Add(m_programBt, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	m_regularBt = new wxRadioButton(panel2, ID_RADIOBUTTON3, _("Regular cell"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	boxSizer1->Add(m_regularBt, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	flexGridSizer1->Add(boxSizer1, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(panel2, ID_STATICTEXT1, _("End:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	flexGridSizer1->Add(StaticText2, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	m_endChoice = new wxChoice(panel2, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	m_endChoice->SetSelection( m_endChoice->Append(_("Auto")) );
	m_endChoice->Append(_("Custom"));
	BoxSizer6->Add(m_endChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	m_endCtrl = new wxTextCtrl(panel2, ID_END_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_END_CTRL"));
	BoxSizer6->Add(m_endCtrl, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_endSpinBt = new wxSpinButton(panel2, ID_END_SPIN, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_END_SPIN"));
	m_endSpinBt->SetRange(0, 100);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_endSpinBt->SetMinSize(wxSize(16,12));
#endif
	BoxSizer6->Add(m_endSpinBt, 0, wxEXPAND, 0);
	m_endFrameSpinBt = new wxSpinButton(panel2, ID_END_FRAME_SPINBT, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_END_FRAME_SPINBT"));
	m_endFrameSpinBt->SetRange(-9999, 9999);
#ifndef __WXGTK__ // GTK needs its native width for side-by-side spin arrows.
	m_endFrameSpinBt->SetMinSize(wxSize(16,12));
#endif
	BoxSizer6->Add(m_endFrameSpinBt, 0, wxEXPAND, 5);
	staticText2 = new wxStaticText(panel2, wxID_ANY, _("Pause:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(staticText2, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	m_pauseSpin = new wxSpinCtrl(panel2, ID_DURATION_CTRL, _T("0"), wxDefaultPosition, wxSize(60,-1), 0, -1, 999, 0, _T("ID_DURATION_CTRL"));
	m_pauseSpin->SetValue(_T("0"));
	BoxSizer6->Add(m_pauseSpin, 0, wxLEFT|wxEXPAND, 4);
	staticText3 = new wxStaticText(panel2, wxID_ANY, _("sec"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(staticText3, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 2);
	flexGridSizer1->Add(BoxSizer6, 1, wxALL|wxEXPAND, 0);
	staticText4 = new wxStaticText(panel2, wxID_ANY, _("Commands:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	flexGridSizer1->Add(staticText4, 0, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 4);
	m_commandsCtrl = new wxComboBox(panel2, ID_COMMANDS_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMMANDS_CTRL"));
	flexGridSizer1->Add(m_commandsCtrl, 1, wxEXPAND, 4);
	BoxSizer4->Add(flexGridSizer1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	panel2->SetSizer(BoxSizer4);
	BoxSizer4->Fit(panel2);
	BoxSizer4->SetSizeHints(panel2);
	splitterWindow->SplitVertically(panel1, panel2);
	mainSizer->Add(splitterWindow, 1, wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_addCellAtBegin = new wxCheckBox(this, ID_ADD_CELL_CHECK, _("Add cell at begin if it doesn\'t exist"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADD_CELL_CHECK"));
	m_addCellAtBegin->SetValue(false);
	BoxSizer1->Add(m_addCellAtBegin, 0, wxALIGN_CENTER_VERTICAL, 5);
	stdDialogButtonSizer = new wxStdDialogButtonSizer();
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	stdDialogButtonSizer->Realize();
	BoxSizer1->Add(stdDialogButtonSizer, 1, wxTOP|wxEXPAND, 5);
	mainSizer->Add(BoxSizer1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	SetSizer(mainSizer);
	SetSizer(mainSizer);
	Layout();
	Center();

	Connect(ID_ADD_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChaptersDlg::OnAddBt);
	Connect(ID_DEL_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChaptersDlg::OnDelBt);
	Connect(ID_TIME_CTRL,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ChaptersDlg::OnChangeTime);
	Connect(ID_TIME_SPINB,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ChaptersDlg::OnTimeSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&ChaptersDlg::OnFrameSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&ChaptersDlg::OnFrameSpinDown);
	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&ChaptersDlg::OnEndChoice);
	Connect(ID_END_CTRL,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ChaptersDlg::OnChangeEnd);
	Connect(ID_END_SPIN,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ChaptersDlg::OnEndSpin);
	Connect(ID_END_FRAME_SPINBT,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&ChaptersDlg::OnEndFrameSpin);
	Connect(ID_END_FRAME_SPINBT,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&ChaptersDlg::OnEndFrameSpinDown);
	//*)
	Connect(ID_THUMBNAILS, EVT_COMMAND_THUMBNAILS_SEL_CHANGED, (wxObjectEventFunction)&ChaptersDlg::OnSelectionChanged);
	m_thumbnails->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(ChaptersDlg::OnKeyDown), (wxObject*) NULL, this);
	Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChaptersDlg::OnOkBt);
	stdDialogButtonSizer->GetAffirmativeButton()->SetDefault();

    SetIcon(((wxTopLevelWindow*)wxGetTopLevelParent(parent))->GetIcon());

    int x = 0;
	m_timeCtrl->GetTextExtent(wxT("00:00:00.000"), &x, NULL);
	m_timeCtrl->SetMinSize(wxSize(x + 10, -1));
	m_endCtrl->SetMinSize(wxSize(x + 10, -1));
	m_pauseSpin->GetTextExtent(wxT("999"), &x, NULL);
	m_pauseSpin->SetMinSize(wxSize(x + 32, -1));
	m_mediaCtrl->Load(m_vob->GetFilename());
	m_slider->SetMax(m_vob->GetResultDuration());
	m_timeSpinBt->SetMax(m_vob->GetResultDuration());
	m_endSpinBt->SetMax(m_vob->GetResultDuration());
	m_commandsCtrl->Append(TitlePropDlg::GetCommandList(m_dvd, m_tsi));

	m_thumbnails->Clear();
	for (vector<Cell*>::const_iterator it = m_cells.begin(); it != m_cells.end(); it++) {
		AddThumbnail((*it));
	}

	m_addCellAtBegin->SetValue(m_cells.size() == 0 || m_cells.front()->GetStart() == 0);

	if (m_thumbnails->GetItemCount() > 0)
		m_thumbnails->SetSelected(0);
	wxCommandEvent evt;
	OnSelectionChanged(evt);
    m_thumbnails->SetFocus();
}

ChaptersDlg::~ChaptersDlg() {
	//(*Destroy(ChaptersDlg)
	//*)
}

wxImage ChaptersDlg::LoadFrame(long pos) {
	double dpos = ((double) pos)/1000;
	wxImage image;
	wxFfmpegMediaDecoder& decoder = m_mediaCtrl->GetDecoder();
	if (decoder.SetPosition(dpos >= 1 ? dpos - 1 : 0)) {
		for (int i = 0; i < 60; i++) {
			image = decoder.GetNextFrame();
			if (decoder.GetPosition() >= dpos)
				break;
		}
	}
	return image.Ok() ? image : wxBITMAP_FROM_MEMORY(video).ConvertToImage();
}

void ChaptersDlg::AddThumbnail(Cell* cell, int pos) {
	wxThumb* thumb = new wxThumb(wxT(""), cell->GetStartStr());
	thumb->SetImage(LoadFrame(cell->GetStart() + (m_vob->GetStartTime()*1000)));
	m_thumbnails->InsertItem(thumb, pos);
}

void ChaptersDlg::OnAddBt(wxCommandEvent& event) {
	long start = m_cells.size() > 0 ? m_cells.back()->GetStart() + s_config.GetDefChapterLength() * 60000 : 0;
	long duration = m_vob->GetResultDuration()*1000;
	Cell* cell = new Cell(start < duration ? start : (duration > 1000 ? duration - 1000 - (duration % 1000): 0));
	m_cells.push_back(cell);
	AddThumbnail(cell);
	m_thumbnails->Refresh();
}

void ChaptersDlg::OnDelBt(wxCommandEvent& event) {
	if (m_thumbnails->GetSelected() < 0)
		return;
	m_selectedCell = NULL;
	m_thumbnails->RemoveItemAt(m_thumbnails->GetSelected());
	m_cells.erase(m_cells.begin() + m_thumbnails->GetSelected());
	if (m_thumbnails->GetSelected() >= m_thumbnails->GetItemCount()) {
		m_thumbnails->SetSelected(m_thumbnails->GetItemCount() - 1);
	}
	wxCommandEvent evt;
	OnSelectionChanged(evt);
	m_thumbnails->Refresh();
}

void ChaptersDlg::OnSelectionChanged(wxCommandEvent& event) {
	if (m_selectedCell != NULL && !CheckCellValues()) {
		for (unsigned int idx = 0; idx < m_cells.size(); idx++) {
			if (m_cells[idx] == m_selectedCell) {
				m_thumbnails->SetSelected(idx);
				break;
			}
		}
		return;
	}

	int idx = m_thumbnails->GetSelected();
	m_selectedCell = idx >= 0 ? m_cells[idx] : NULL;

	bool enable = idx >= 0;
	m_delBt->Enable(enable);
	m_slider->Enable(enable);
	m_timeCtrl->Enable(enable);
	m_timeSpinBt->Enable(enable);
	m_frameSpinBt->Enable(enable);
	m_endChoice->Enable(enable);
	m_endSpinBt->Enable(enable);
	m_endCtrl->Enable(enable);
	m_pauseSpin->Enable(enable);
	m_regularBt->Enable(enable);
	m_chapterBt->Enable(enable);
	m_programBt->Enable(enable);
	m_commandsCtrl->Enable(enable);
	if (enable) {
		m_endChoice->SetSelection(m_selectedCell->GetEnd() == -1 ? 0 : 1);
		m_endSpinBt->SetValue(m_selectedCell->GetEnd() >= 0 ? m_selectedCell->GetEnd() / 1000 : 0);
		m_endCtrl->ChangeValue(m_selectedCell->GetEnd() >= 0 ? Time2String(m_selectedCell->GetEnd(), true) : wxString(wxT("")));
		m_pauseSpin->SetValue(m_selectedCell->GetPause());
		m_regularBt->SetValue(true);
		m_chapterBt->SetValue(m_selectedCell->IsChapter());
		m_programBt->SetValue(m_selectedCell->IsProgram());
		m_commandsCtrl->SetValue(m_selectedCell->GetCommands());
		SeekVideo(m_selectedCell->GetStart());
	} else {
		SeekVideo(0);
	}
}

void ChaptersDlg::OnKeyDown(wxKeyEvent& event) {
	switch (event.GetKeyCode()) {
	case WXK_DELETE: {
		wxCommandEvent evt;
		OnDelBt(evt);
		break;
	}
	default:
		event.Skip();
		break;
	}
}

void ChaptersDlg::SeekVideo(long pos, bool updateTimeCtrl) {
	m_slider->SetValue(lround(pos / 1000));
	m_timeSpinBt->SetValue(lround(pos / 1000));
	m_mediaCtrl->Seek((wxFileOffset) (pos + (m_vob->GetStartTime()*1000)));
	if (updateTimeCtrl)
		m_timeCtrl->ChangeValue(Time2String(pos, true));

	int idx = m_thumbnails->GetSelected();
	if (idx < 0)
		return;
	Cell* cell = m_cells[idx];
	cell->SetStart(pos);
	int idx2 = idx;
	while (idx2 > 0 && cell->GetStart() < m_cells[idx2-1]->GetStart())
		idx2--;
	while (idx2 < (int) m_cells.size() - 1 && cell->GetStart() > m_cells[idx2+1]->GetStart())
		idx2++;
	if (idx2 != idx) {
		m_cells.erase(m_cells.begin() + idx);
		m_cells.insert(m_cells.begin() + idx2, cell);
		m_thumbnails->RemoveItemAt(idx);
		AddThumbnail(cell, idx2);
		m_thumbnails->SetSelected(idx2);
	}
	m_thumbnails->GetSelectedItem()->SetCaption(cell->GetStartStr());
	wxImage image = m_mediaCtrl->GetImage();
	m_thumbnails->GetSelectedItem()->SetImage(image.Ok() ? image : wxBITMAP_FROM_MEMORY(video).ConvertToImage());
	m_thumbnails->GetSelectedItem()->Update();
	m_thumbnails->Refresh();
	wxCommandEvent evt;
	OnEndChoice(evt); // update end time if selected "auto"
}

void ChaptersDlg::OnChangeTime(wxCommandEvent& event) {
	if (s_timeRE.Matches(m_timeCtrl->GetValue())) {
		SeekVideo(String2Time(m_timeCtrl->GetValue(), m_mediaCtrl->GetFps()), false);
	}
}

void ChaptersDlg::OnTimeSpin(wxSpinEvent& event) {
	SeekVideo(((long)m_timeSpinBt->GetValue())*1000);
}

void ChaptersDlg::OnSliderScroll(wxScrollEvent& event) {
	SeekVideo(((long)m_slider->GetValue())*1000);
}

void ChaptersDlg::OnEndChoice(wxCommandEvent& event) {
	if (m_endChoice->GetSelection() == 0) {
		int idx = m_thumbnails->GetSelected();
		Cell* nextCell = idx >= 0 && idx + 1 < (int) m_cells.size() ? m_cells[idx + 1] : NULL;
		long endTime = nextCell ? nextCell->GetStart() : m_vob->GetResultDuration()*1000;
		m_endSpinBt->SetValue(endTime / 1000);
		m_endCtrl->ChangeValue(Time2String(endTime, true));
	}
	m_endCtrl->Enable(m_endChoice->GetSelection() > 0);
	m_endSpinBt->Enable(m_endChoice->GetSelection() > 0);
	m_endFrameSpinBt->Enable(m_endChoice->GetSelection() > 0);
}

void ChaptersDlg::OnChangeEnd(wxCommandEvent& event) {
	if (s_timeRE.Matches(m_endCtrl->GetValue())) {
		m_endSpinBt->SetValue(String2Time(m_endCtrl->GetValue(), m_mediaCtrl->GetFps()) / 1000);
	}
}

void ChaptersDlg::OnFrameSpin(wxSpinEvent& event) {
	SeekVideo(String2Time(m_timeCtrl->GetValue(), m_mediaCtrl->GetFps()) + 1000 / m_mediaCtrl->GetFps());
}

void ChaptersDlg::OnFrameSpinDown(wxSpinEvent& event) {
	long pos = String2Time(m_timeCtrl->GetValue(), m_mediaCtrl->GetFps()) - 1000 / m_mediaCtrl->GetFps();
	SeekVideo(pos >= 0 ? pos : 0);
}

void ChaptersDlg::OnEndSpin(wxSpinEvent& event) {
	m_endCtrl->ChangeValue(Time2String(m_endSpinBt->GetValue() * 1000, true));
}

void ChaptersDlg::OnEndFrameSpin(wxSpinEvent& event) {
	long pos = String2Time(m_endCtrl->GetValue(), m_mediaCtrl->GetFps()) + 1000 / m_mediaCtrl->GetFps();
	m_endCtrl->ChangeValue(Time2String(pos, true));
}

void ChaptersDlg::OnEndFrameSpinDown(wxSpinEvent& event) {
	long pos = String2Time(m_endCtrl->GetValue(), m_mediaCtrl->GetFps()) - 1000 / m_mediaCtrl->GetFps();
	m_endCtrl->ChangeValue(Time2String(pos >= 0 ? pos : 0, true));
}

bool ChaptersDlg::CheckCellValues() {
	if (m_selectedCell == NULL)
		return true;

	if (!s_timeRE.Matches(m_timeCtrl->GetValue())) {
		wxLogError(_("'%s' is not valid time"), m_timeCtrl->GetValue().c_str());
		return false;
	}
	if (m_endChoice->GetSelection() > 0 && m_endCtrl->GetValue().length() > 0
			&& !s_timeRE.Matches(m_endCtrl->GetValue())) {
		wxLogError(_("'%s' is not valid time"), m_endCtrl->GetValue().c_str());
		return false;
	}
	if (m_endChoice->GetSelection() > 0 && m_endCtrl->GetValue().length() > 0
			&& String2Time(m_timeCtrl->GetValue()) > String2Time(m_endCtrl->GetValue())) {
		wxLogError(_("Start time cannot be after end time"));
		return false;
	}
	DVDAction action;
	action.SetCustom(m_commandsCtrl->GetValue());
	if (m_commandsCtrl->GetValue().length() && !action.IsValid(m_dvd, m_tsi, m_pgci, false, wxT(""), true, false)) {
		return false;
	}

	// update
	m_selectedCell->SetStart(String2Time(m_timeCtrl->GetValue(), m_mediaCtrl->GetFps()));
	m_selectedCell->SetEnd(m_endChoice->GetSelection() > 0 && m_endCtrl->GetValue().length()
			? String2Time(m_endCtrl->GetValue(), m_mediaCtrl->GetFps()) : -1);
	m_selectedCell->SetPause(m_pauseSpin->GetValue());
	m_selectedCell->SetCommands(m_commandsCtrl->GetValue());
	m_selectedCell->SetChapter(m_chapterBt->GetValue());
	m_selectedCell->SetProgram(m_programBt->GetValue());
	return true;
}

void ChaptersDlg::OnOkBt(wxCommandEvent& event) {
	if (!CheckCellValues())
		return;
	// update VOB
	if (m_cells.size() == 0 && m_vobi == 0)
		m_cells.push_back(new Cell(0));
	else if (m_cells.size() && m_cells.front()->GetStart() != 0 && m_addCellAtBegin->GetValue())
		m_cells.insert(m_cells.begin(), new Cell(0, m_vobi == 0));
	m_vob->GetCells().swap(m_cells);
	EndModal(wxID_OK);
}
