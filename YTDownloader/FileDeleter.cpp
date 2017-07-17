#include "FileDeleter.h"
#include <wx/log.h>

FileDeleter::FileDeleter(const wxString& path)
    : m_path(path)
{
}

FileDeleter::FileDeleter(const wxFileName& path)
    : m_path(path.GetFullPath())
{
}

FileDeleter::~FileDeleter() 
{
    wxLogNull NoLog;
    wxRemoveFile(m_path);
}
