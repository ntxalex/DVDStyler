/////////////////////////////////////////////////////////////////////////////
// Name:        About.cpp
// Purpose:     About dialog
// Author:      Alex Thuering
// Created:     6.11.2003
// RCS-ID:      $Id: About.cpp,v 1.84 2016/05/08 17:06:49 ntalex Exp $
// Copyright:  (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "About.h"
#include "Authors.h"
#include <wxVillaLib/utils.h>
#include <wx/statline.h>
#include <wx/notebook.h>
#include <wx/hyperlink.h>
#include <wx/utils.h>
#include "rc/logo.png.h"

BEGIN_EVENT_TABLE(About, wxDialog)
	EVT_HTML_LINK_CLICKED(wxID_ANY, About::OnLinkClicked)
END_EVENT_TABLE()

wxString FixEmail(const wxString& str) {
	wxString result = str;
	result.Replace(wxT(" at "), wxT("@"));
	result.Replace(wxT("<"), wxT("&lt;"));
	result.Replace(wxT(">"), wxT("&gt;"));
	return result;
}

About::About(wxWindow* parent): wxDialog(parent, -1, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    // sets the application icon
    SetTitle(_("About ..."));
	
	wxNotebook* notebook = new wxNotebook(this, -1);
	wxPanel* aboutPanel = new wxPanel(notebook, -1);
	notebook->AddPage(aboutPanel, _("About"));
	wxBoxSizer* aboutSizer = new wxBoxSizer(wxVERTICAL);
	aboutPanel->SetAutoLayout(true);
	aboutPanel->SetSizer(aboutSizer);
	
    // about info
    wxGridSizer* aboutinfo = new wxFlexGridSizer(2, 3, 3);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, _("Written by: ")), 0, wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, APP_MAINT), 1, wxEXPAND | wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, _("Version: ")), 0, wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, APP_VERSION), 1, wxEXPAND | wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, _("Licence type: ")), 0, wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, APP_LICENCE), 1, wxEXPAND | wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, _("Copyright: ")), 0, wxALIGN_LEFT);
    aboutinfo->Add(new wxStaticText(aboutPanel, -1, APP_COPYRIGHT), 1, wxEXPAND | wxALIGN_LEFT);

    // about title/info
    wxBoxSizer* abouttext = new wxBoxSizer(wxVERTICAL);
    wxStaticText* appname = new wxStaticText(aboutPanel, -1, APP_NAME);
    appname->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    abouttext->Add(appname, 0, wxALIGN_LEFT);
    abouttext->Add(0, 10);
    abouttext->Add(aboutinfo, 1, wxEXPAND);

    // about icontitle//info
    wxBoxSizer* aboutpane = new wxBoxSizer(wxHORIZONTAL);
    wxBitmap bitmap = wxBITMAP_FROM_MEMORY(logo);
    aboutpane->Add(new wxStaticBitmap(aboutPanel, -1, bitmap), 0, wxALIGN_LEFT);
    aboutpane->Add(10, 0);
    aboutpane->Add(abouttext, 1, wxEXPAND);
	
    // about description
    aboutSizer->Add(aboutpane, 0, wxEXPAND | wxALL, 10);
    aboutSizer->Add(new wxStaticText(aboutPanel, -1,
	  _("DVDStyler is a crossplatform authoring system for Video DVD production.")),
	  0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    aboutSizer->Add(0, 6);
    wxHyperlinkCtrl* website = new wxHyperlinkCtrl(aboutPanel, wxID_ANY, APP_WEBSITE, APP_WEBSITE);
    wxString url = APP_WEBSITE;
    website->SetURL(url);
    aboutSizer->Add(website, 0, wxALIGN_CENTER);
    
    // support
    wxHtmlWindow* supportPanel = new wxHtmlWindow(notebook, -1,
    		wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHW_SCROLLBAR_AUTO);
    wxString page = wxT("<html><body>");
    page += wxT("<h5>") + wxString(_("Forum")) + wxT("</h5>");
    page += wxString::Format(_("Please use %sDVDStyler forum%s to get support, ask questions, or discuss this software."),
    		wxT("<a href=\"http://sourceforge.net/p/dvdstyler/discussion/318795/\" target=\"_blank\">"), wxT("</a>"));
    page += wxT("<h5>") + wxString(_("WIKI")) + wxT("</h5>");
    page += wxString::Format(_("Some documentation and %sFAQ%s you can find in %sDVDStyler WIKI%s or you can publish there your comments or small guides."),
			wxT("<a href=\"https://github.com/ntxalex/DVDStyler/wiki\" target=\"_blank\">"), wxT("</a>"),
			wxT("<a href=\"https://github.com/ntxalex/DVDStyler/wiki\" target=\"_blank\">"), wxT("</a>"));
    page += wxT("<h5>") + wxString(_("Bugs & RFE")) + wxT("</h5>");
    page += wxString::Format(_("Please use the %sGitHub issue tracker%s to report bugs or submit feature requests."),
        wxT("<a href=\"https://github.com/ntxalex/DVDStyler/issues\" target=\"_blank\">"), wxT("</a>"));
    page += wxT("</body></html>");
    supportPanel->SetPage(page);
    notebook->AddPage(supportPanel, _("Support"));
	
	// authors
	wxHtmlWindow* authorsPanel = new wxHtmlWindow(notebook, -1,
	  wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHW_SCROLLBAR_AUTO);
	page = _T("<html><body>");
	page += _T("<table width='100%' cellspacing='2' cellpadding='2' border='0' valign='top'>");
	
	page += _T("<tr><td colspan='3'><b>") + wxString(_("Author and Maintainer")) + _T("</b></td></tr>");
	page += _T("<tr><td>&nbsp;</td><td colspan='2'>");
	page += FixEmail(s_author);
	page += _T("</td></tr>");
	
	page += _T("<tr><td colspan='3'><b>") + wxString(_("Doc Writer")) + _T("</b></td></tr>");
	page += _T("<tr><td>&nbsp;</td><td colspan='2'>");
	page += FixEmail(s_docWriter);
	page += _T("</td></tr>");
	
	page += _T("<tr><td colspan='3'><b>") + wxString(_("Packager (.deb)")) + _T("</b></td></tr>");
	page += _T("<tr><td>&nbsp;</td><td colspan='2'>");
	page += FixEmail(s_packager);
	page += _T("</td></tr>");
	
	page += _T("<tr><td colspan='3'><b>") + wxString(_("Translations")) + _T("</b></td></tr>");
	
	for (unsigned int i = 0; i < sizeof(s_translations)/sizeof(s_translations[0]); i++)
		page += wxT("<tr><td>&nbsp;</td><td>") + s_translations[i][0] + wxT("</td><td width='100%'>")
				+ FixEmail(s_translations[i][1]) + wxT("</td></tr>");
	
	page += _T("<tr><td colspan='3'><b>") + wxString(_("Libraries and Tools")) + _T("</b></td></tr>");
	page += _T("<tr><td>&nbsp;</td><td>wxWidgets</td><td>Julian Smart, Robert Roebling and other</td></tr>");
	page += _T("<tr><td>&nbsp;</td><td>dvdauthor</td><td>Scott Smith</td></tr>");
	page += _T("<tr><td>&nbsp;</td><td>cdrtools</td><td>Joerg Schilling</td></tr>");
	page += _T("<tr><td>&nbsp;</td><td>dvd+rw-tools</td><td>Andy Polyakov</td></tr>");
	
	page += _T("</table>");
	page += _T("</body></html>");
	authorsPanel->SetPage(page);
	notebook->AddPage(authorsPanel, _("Authors"));
	
	// licence
	wxHtmlWindow* licencePanel = new wxHtmlWindow(notebook, -1,
	  wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHW_SCROLLBAR_AUTO);
	page = _T("<html><body>");
	page += _T("<p>");
	page += _("DVDStyler is <a href='http://www.gnu.org/philosophy/free-sw.html'>free software</a> \
distributed under <a href='http://www.gnu.org/copyleft/gpl.html'>GNU General Public License (GPL)</a>. \
Please visit those sites for details of each agreement.");
	page += _T("</p>");
	page += _T("<p>");
	page += _("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, \
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS \
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, \
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR \
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	page += _T("</p>");
	page += _T("</body></html>");
	licencePanel->SetPage(page);
	notebook->AddPage(licencePanel, _("Licence"));
	
	// buttons
	wxBoxSizer* totalpane = new wxBoxSizer(wxVERTICAL);
	totalpane->Add(notebook, 1, wxEXPAND|wxALL, 6);
    wxButton* okButton = new wxButton(this, wxID_OK, _("OK"));
    okButton->SetDefault();
    okButton->SetFocus();
    totalpane->Add(okButton, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT|wxBOTTOM, 10);
	
    SetSizerAndFit(totalpane);
	Center();

    ShowModal();
}

void About::OnLinkClicked(wxHtmlLinkEvent& event) {
	wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());
}
