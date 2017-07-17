#include "CommonDirs.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

CommonDirs::CommonDirs()
{
}

CommonDirs::~CommonDirs()
{
}

wxString CommonDirs::GetTempDir() const
{
    wxFileName f(wxStandardPaths::Get().GetUserLocalDataDir(), "");
    f.AppendDir("tmp");
    return f.GetPath();
}

wxString CommonDirs::GetDownloadsDir() const
{
    wxFileName f(wxStandardPaths::Get().GetUserLocalDataDir(), "");
    f.AppendDir("downloads");
    return f.GetPath();
}

void CommonDirs::CreateDirectories()
{
    wxFileName::Mkdir(GetTempDir(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    wxFileName::Mkdir(GetDownloadsDir(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
}
