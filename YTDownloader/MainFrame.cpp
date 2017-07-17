#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include "YouTubeToMp3Org.h"
#include <wx/msgdlg.h>

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
}

MainFrame::~MainFrame() {}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
void MainFrame::OnGetURLs(wxCommandEvent& event) { m_service.Download("HFe6oqtjD_8"); }
