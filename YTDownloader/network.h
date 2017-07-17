#ifndef __network__
#define __network__

#include <wx/string.h>
#include <wx/ffile.h>
#include <curl/curl.h>
#include <wx/app.h>

wxDECLARE_EVENT(wxEVT_DOWNLOAD_IN_PROGRESS, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_DOWNLOAD_COMPLETED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_DOWNLOAD_CANCELED, wxCommandEvent);

class Network
{
    CURL* m_curl;
    wxString m_error;
    wxFFile m_file;
    wxString m_fileName;
    double m_dlTotal;
    double m_dlNow;
    wxMemoryBuffer m_pageContent;
    wxMemoryBuffer m_postData;

public:
    static bool gs_quit;

protected:
    bool OpenFile();
    size_t Append(void* buffer, size_t byte_size, size_t count);

public:
    Network();
    virtual ~Network();

    static void Initialize();
    static void UnInitialize();

    /**
     * @brief return last error message
     */
    const wxString& Error() const;

    /**
     * @brief wget page content
     * @param url the URL to get
     * @return true on success, false otherwise
     */
    bool wget(const wxString& url);

    /**
     * @brief post request using the postBuffer
     */
    bool post(const wxString& url, const wxMemoryBuffer& postBuffer, const wxString& contentType = "");
    /**
     * @brief download URL into targetFile
     * @param url source URL
     * @param targetFile name of the file which will contain the downloaded content
     * @return true on success, false otherwise
     */
    bool Download(const wxString& url, const wxString& targetFiles);

    /**
     * @brief CURL callback for writing downloaded information
     * @param buffer pointer to data
     * @param size char size
     * @param nmemb number of bytes to write
     * @param argument passed to CURL with CURLOPT_WRITEDATA option
     * @return number of bytes writtern, or (size_t)-1 on failure
     */
    static size_t WriteCallback(void* buffer, size_t size, size_t nmemb, void* arg);

    /**
     * @brief CURL callback for writing downloaded information
     * @param buffer pointer to data
     * @param size char size
     * @param nmemb number of bytes to write
     * @param argument passed to CURL with CURLOPT_WRITEDATA option
     * @return number of bytes writtern, or (size_t)-1 on failure
     */
    static size_t WriteCallbackMemory(void* buffer, size_t size, size_t nmemb, void* arg);

    /**
     * @brief
     * @param arg
     * @param t download total
     * @param d downloaded now
     * @param ultotal
     * @param ulnow
     * @return
     */
    static int
    CurlProgressCallback(void* arg, double t, /* dltotal */ double d, /* dlnow */ double ultotal, double ulnow);

    /**
     * @brief set the output file name
     * @param fileName
     */
    void SetFileName(const wxString& fileName) { this->m_fileName = fileName; }

    /**
     * @brief return the output file name
     */
    const wxString& GetFileName() const { return m_fileName; }

    void SetDlNow(const double& dlNow) { this->m_dlNow = dlNow; }

    void SetDlTotal(const double& dlTotal) { this->m_dlTotal = dlTotal; }

    const double& GetDlNow() const { return m_dlNow; }

    const double& GetDlTotal() const { return m_dlTotal; }

    const wxMemoryBuffer& GetPageContent() const { return m_pageContent; }

    wxString GetPageContentAsString() const
    {

        wxMemoryBuffer mb = m_pageContent;
        mb.AppendByte(0);

        wxString str((const char*)mb.GetData(), wxConvUTF8);
        return str;
    }
};

#endif // __network__
