#ifndef FILEDELETER_H
#define FILEDELETER_H

#include <wx/string.h>
#include <wx/filename.h>

class FileDeleter
{
    wxString m_path;

public:
    FileDeleter(const wxString& path);
    FileDeleter(const wxFileName& path);
    virtual ~FileDeleter();
};

#endif // FILEDELETER_H
