#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "network.h"
#include <map>
#include "EventNotifier.h"

wxDEFINE_EVENT(wxEVT_DOWNLOAD_IN_PROGRESS, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_DOWNLOAD_COMPLETED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_DOWNLOAD_CANCELED, wxCommandEvent);

bool Network::gs_quit = false;

Network::Network()
    : m_curl(NULL)
{
    m_curl = curl_easy_init();
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);
    gs_quit = false;
}

Network::~Network()
{
    if(m_curl) {
        curl_easy_cleanup(m_curl);
        m_curl = NULL;
    }
}

bool Network::Download(const wxString& url, const wxString& targetFile)
{
    if(!m_curl) {
        return false;
    }

    CURLcode res;
    SetFileName(targetFile);

    const wxCharBuffer curl = url.mb_str();
    curl_easy_setopt(m_curl, CURLOPT_URL, curl.data());

    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(m_curl,
                     CURLOPT_USERAGENT,
                     "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3");

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, Network::WriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, Network::CurlProgressCallback);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);

    // execute CURL operation
    res = curl_easy_perform(m_curl);
    if(res != CURLE_OK) {
        m_error.Printf(wxT("Error: %d"), res);
        curl_easy_reset(m_curl);
        return false;
    }

    if(m_file.IsOpened()) {
        m_file.Close();
    }

    curl_easy_reset(m_curl);
    return true;
}

size_t Network::WriteCallback(void* buffer, size_t size, size_t nmemb, void* arg)
{
    Network* net = (Network*)arg;
    if(net) {
        if(!net->OpenFile()) {
            return (size_t)-1;
        }
    }

    if(gs_quit) {
        gs_quit = false;
        return (size_t)-1;
    }

    return net->Append(buffer, size, nmemb);
}

int Network::CurlProgressCallback(void* arg, double t, double d, double ultotal, double ulnow)
{
    Network* net = (Network*)arg;
    if(arg) {
        net->SetDlNow(d);
        net->SetDlTotal(t);

        // Send an event
        wxCommandEvent evt(wxEVT_DOWNLOAD_IN_PROGRESS);
        std::pair<int, int>* p = new std::pair<int, int>(d, t);
        evt.SetClientData(p);
        EventNotifier::Get()->AddPendingEvent(evt);
    }
    return 0;
}

size_t Network::Append(void* buffer, size_t byte_size, size_t count)
{
    if(m_file.IsOpened()) {
        size_t bytes = m_file.Write(buffer, count);
        m_file.Flush();
        return bytes;
    }
    return (size_t)-1;
}

const wxString& Network::Error() const { return m_error; }

bool Network::OpenFile()
{
    if(m_file.IsOpened()) {
        return true;
    }
    return m_file.Open(GetFileName(), wxT("w+b"));
}

void Network::Initialize() { curl_global_init(CURL_GLOBAL_DEFAULT); }

void Network::UnInitialize() { curl_global_cleanup(); }

bool Network::wget(const wxString& url)
{
    m_pageContent.Clear();
    if(!m_curl) {
        return false;
    }

    CURLcode res;
    const wxCharBuffer curl = url.mb_str(wxConvUTF8);
    curl_easy_setopt(m_curl, CURLOPT_URL, curl.data());

    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(m_curl,
                     CURLOPT_USERAGENT,
                     "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3");

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, Network::WriteCallbackMemory);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);

    // execute CURL operation
    res = curl_easy_perform(m_curl);
    if(res != CURLE_OK) {
        m_error.Printf(wxT("Error: %d"), res);
        curl_easy_reset(m_curl);
        return false;
    }

    curl_easy_reset(m_curl);
    return true;
}

size_t Network::WriteCallbackMemory(void* buffer, size_t size, size_t nmemb, void* arg)
{
    Network* net = (Network*)arg;
    if(gs_quit) {
        gs_quit = false;
        return (size_t)-1;
    }

    net->m_pageContent.AppendData(buffer, nmemb);
    return nmemb;
}

bool Network::post(const wxString& url, const wxMemoryBuffer& postBuffer, const wxString& contentType)
{
    m_pageContent.Clear();
    if(!m_curl) {
        return false;
    }

    m_postData = postBuffer;

    CURLcode res;
    const wxCharBuffer curl = url.mb_str(wxConvUTF8);
    curl_easy_setopt(m_curl, CURLOPT_URL, curl.data());

    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(m_curl,
                     CURLOPT_USERAGENT,
                     "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3");

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, Network::WriteCallbackMemory);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);

    // set the post buffer
    struct curl_slist* headers(NULL);
    if(contentType.IsEmpty())
        headers = curl_slist_append(headers, "Content-Type: text/html; charset=UTF-8;");
    else
        headers = curl_slist_append(headers, contentType.mb_str(wxConvUTF8).data());

    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_postData.GetData());
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, m_postData.GetDataLen());
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);

    // execute CURL operation
    res = curl_easy_perform(m_curl);

    // Free the headers
    curl_slist_free_all(headers);

    if(res != CURLE_OK) {
        m_error.Printf(wxT("Error: %d"), res);
        curl_easy_reset(m_curl);
        return false;
    }

    curl_easy_reset(m_curl);
    return true;
}
