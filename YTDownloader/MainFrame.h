#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include "YouTubeToMp3Org.h"

class MainFrame : public MainFrameBaseClass
{
    YouTubeToMp3Org m_service;

public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

protected:
    virtual void OnGetURLs(wxCommandEvent& event);
};
#endif // MAINFRAME_H
