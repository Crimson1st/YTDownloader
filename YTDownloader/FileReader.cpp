#include "FileReader.h"
#include <wx/ffile.h>

FileReader::~FileReader() {}

FileReader::FileReader(const wxString& path)
    : m_path(path)
{
}

FileReader::FileReader(const wxFileName& path)
    : m_path(path.GetFullPath())
{
}

bool FileReader::ReadAll(wxString& content, const wxMBConv& conv) const
{
    wxFFile fp(m_path, "rb");
    if(!fp.IsOpened()) {
        return false;
    }

    if(!fp.ReadAll(&content, conv)) {
        return false;
    }

    return true;
}