#ifndef YOUTUBETOMP3ORG_H
#define YOUTUBETOMP3ORG_H

#include <wx/event.h>
#include "duk_config.h"
#include <wx/string.h>
#include "network.h"

class YouTubeToMp3Org : public wxEvtHandler
{
    static wxString m_printOutput;
    Network m_network;

protected:
    void PushCodeAsString(duk_context* ctx) const;
    static duk_ret_t NativePrint(duk_context* ctx);
    wxString SignUrl(const wxString& url) const;
    time_t GetTimestamp() const;
    wxString PushItem(const wxString& videoID) const;
    wxString ItemInfo(const wxString& videoID) const;

public:
    YouTubeToMp3Org();
    virtual ~YouTubeToMp3Org();

    /**
     * @brief download youtube video ID
     */
    void Download(const wxString& videoID);
};

#endif // YOUTUBETOMP3ORG_H
