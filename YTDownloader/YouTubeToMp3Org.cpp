#include "YouTubeToMp3Org.h"
#include "voodoo.js"
#include "duktape.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "FileDeleter.h"
#include "FileReader.h"
#include <wx/msgdlg.h>
#include "CommonDirs.h"

wxString YouTubeToMp3Org::m_printOutput;

YouTubeToMp3Org::YouTubeToMp3Org()
{
    static bool first = true;
    if(first) {
        Network::Initialize();
        first = false;
    }
}

YouTubeToMp3Org::~YouTubeToMp3Org() {}

void YouTubeToMp3Org::PushCodeAsString(duk_context* ctx) const
{
    duk_push_lstring(ctx, (const char*)urlToEncode, (duk_size_t)strlen(urlToEncode));
}

duk_ret_t YouTubeToMp3Org::NativePrint(duk_context* ctx)
{
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    m_printOutput = duk_safe_to_string(ctx, -1);
    return 0;
}

wxString YouTubeToMp3Org::SignUrl(const wxString& url) const
{
    duk_context* ctx = duk_create_heap_default();
    // Register the 'print' function
    duk_push_c_function(ctx, YouTubeToMp3Org::NativePrint, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    duk_push_string(ctx, url.c_str());
    duk_put_global_string(ctx, "urlToEncode");
    PushCodeAsString(ctx);
    duk_peval(ctx); // eval the function
    duk_destroy_heap(ctx);
    wxString signedUrl;
    signedUrl << url << "&s=" << m_printOutput;
    return signedUrl;
}

time_t YouTubeToMp3Org::GetTimestamp() const
{
    timeval tv;
    gettimeofday(&tv, NULL);
    time_t ts = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return ts;
}

wxString YouTubeToMp3Org::PushItem(const wxString& videoID) const
{
    wxString url;
    url << "/a/pushItem/?item=https%3A//www.youtube.com/watch%3Fv%3D" << videoID
        << "&el=ma&bf=false&r=" << GetTimestamp();
    url = SignUrl(url);
    url.Prepend("http://www.youtube-mp3.org");
    return url;
}

wxString YouTubeToMp3Org::ItemInfo(const wxString& videoID) const
{
    wxString url;
    url << "/a/itemInfo/?video_id=" << videoID << "&ac=www&t=grp&r=" << GetTimestamp();
    url = SignUrl(url);
    url.Prepend("http://www.youtube-mp3.org");
    return url;
}

void YouTubeToMp3Org::Download(const wxString& videoID)
{
    CommonDirs dirs;
    wxString url1 = PushItem(videoID);
    wxFileName file1(dirs.GetTempDir(), videoID + "-1");
    m_network.Download(url1, file1.GetFullPath());

    // Next download the second URL
    wxString url2 = ItemInfo(videoID);
    wxFileName file2(dirs.GetTempDir(), videoID + "-2");
    m_network.Download(url2, file2.GetFullPath());
    
    FileDeleter d1(file1);
    FileDeleter d2(file2);
    
    FileReader reader(file2);
    wxString itemInfo;
    if(reader.ReadAll(itemInfo)) {
        ::wxMessageBox(itemInfo);
    }
}