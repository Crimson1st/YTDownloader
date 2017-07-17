#ifndef COMMONDIRS_H
#define COMMONDIRS_H

#include <wx/string.h>
class CommonDirs
{
public:
    CommonDirs();
    virtual ~CommonDirs();
    
    void CreateDirectories();
    wxString GetTempDir() const;
    wxString GetDownloadsDir() const;
};

#endif // COMMONDIRS_H
