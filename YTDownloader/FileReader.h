#ifndef FILEREADER_H
#define FILEREADER_H

#include <wx/string.h>
#include <wx/filename.h>

class FileReader
{
    wxString m_path;

public:
    FileReader(const wxString& path);
    FileReader(const wxFileName& path);
    virtual ~FileReader();

    bool ReadAll(wxString& content, const wxMBConv& conv = wxConvUTF8) const;
};

#endif // FILEREADER_H
