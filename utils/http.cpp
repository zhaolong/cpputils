#include "config.h"
#include "http.h"
#include "string.h"
#include <stdarg.h>

#define CURL_STATICLIB
#include <curl/curl.h>

namespace Utils {
namespace Http {

Handle& Instance()
{
    return Handle::Instance();
}

Handle& Handle::Instance()
{
    static Handle httpHandle;
    return httpHandle;
}

Handle::Handle()
    : m_error(HTTP_NO_ERROR)
    , m_curlRunningHandles(0)
{
    if(curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK
        || !(m_curlMutiHandle = curl_multi_init()))
        m_error = HTTP_HANDLE_INIT_ERROR;
}

Handle::~Handle()
{
    curl_multi_cleanup(m_curlMutiHandle);
    curl_global_cleanup();
}

bool Handle::AddRequest(Request& request)
{
    CURLMcode ret = curl_multi_add_handle(m_curlMutiHandle, request.m_curlEasyHandle);
    return (ret == CURLM_OK);
}

bool Handle::RemoveRequest(Request& request)
{
    CURLMcode ret = curl_multi_remove_handle(m_curlMutiHandle, request.m_curlEasyHandle);
    return (ret == CURLM_OK);
}

bool Handle::SendRequests()
{
    CURLMcode ret = CURLM_OK;
    while ((ret = curl_multi_perform(m_curlMutiHandle, &m_curlRunningHandles)) == CURLM_CALL_MULTI_PERFORM)
    {
        ;
    }
    return (ret == CURLM_OK);
}

bool Handle::SendRequest(Request& request)
{
    CURLcode ret = curl_easy_perform(request.m_curlEasyHandle);
    return (ret == CURLE_OK);
}

size_t Handle::HeadFunc(char *data, size_t size, size_t nmemb, Response *response)
{
    if(response == NULL)
        return 0;

    curl_easy_getinfo(response->m_request->m_curlEasyHandle, CURLINFO_RESPONSE_CODE, &response->m_statusCode);

    size_t len = size*nmemb;
    response->m_headers.append(data, len);
    return len;
}

size_t Handle::WriteFunc(char *data, size_t size, size_t nmemb, Response *response)
{
    if(response == NULL)
        return 0;

    size_t len = size*nmemb;
    response->m_body.append(data, len);
    return len;
}

Request::Request(const char* url /* = NULL */)
: m_curlEasyHandle(Http::Instance().Error() ? NULL : curl_easy_init())
, m_curlSlist(NULL)
, m_formpost(NULL)
, m_error(HTTP_NO_ERROR)
, m_url(url ? url : "")
, m_response(NULL)
{
    m_response = new Response(this);
    if(!m_curlEasyHandle || !m_response
        || curl_easy_setopt(m_curlEasyHandle, CURLOPT_WRITEDATA, m_response) != CURLE_OK
        || curl_easy_setopt(m_curlEasyHandle, CURLOPT_WRITEFUNCTION, Handle::WriteFunc) != CURLE_OK
        || curl_easy_setopt(m_curlEasyHandle, CURLOPT_HEADERDATA, m_response) != CURLE_OK
        || curl_easy_setopt(m_curlEasyHandle, CURLOPT_HEADERFUNCTION, Handle::HeadFunc) != CURLE_OK 
        )
        m_error = HTTP_REQUEST_INIT_ERROR;
    else
    {
        if(curl_easy_setopt(m_curlEasyHandle, CURLOPT_URL, url) != CURLE_OK)
            m_error = HTTP_REQUEST_INIT_ERROR;
    }
}

Request::~Request()
{
    curl_easy_cleanup(m_curlEasyHandle);
    curl_slist_free_all(m_curlSlist);
    curl_formfree(m_formpost);
    if(m_response)
        delete m_response;
}

bool Request::SetUrl(const char* url)
{
    if(String::IsEmpty(url))
        return false;
    m_url = url;
    if(curl_easy_setopt(m_curlEasyHandle, CURLOPT_URL, url) != CURLE_OK)
        return false;
    return true;
}

bool Request::SetPost(const char* post)
{
    return (curl_easy_setopt(m_curlEasyHandle, CURLOPT_COPYPOSTFIELDS, post) == CURLE_OK);
}

bool Request::SetUploadFile(const UploadFile& uploadFile)
{
    // http://curl.haxx.se/libcurl/c/curl_formadd.html
    // curl_formadd() is used to append sections when building a multipart/formdata HTTP POST. 
    // All list-data will be allocated by the function itself. 
    // NOTE: You must call curl_formfree(3) on the firstitemP after the form post has been done to 
    // free the resources. 

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;

    /* Fill in the file upload field */
    curl_formadd(&formpost,
        &lastptr,
        CURLFORM_COPYNAME, uploadFile.name.c_str(),
        CURLFORM_FILE, uploadFile.file.c_str(),
        CURLFORM_END);

    /* Fill in other fields */
    FormFields formFields = uploadFile.formFields;
    for (size_t i = 0; i < formFields.size(); i++)
    {
        curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, formFields[i].name.c_str(),
            CURLFORM_COPYCONTENTS, formFields[i].contents.c_str(),
            CURLFORM_END);
    }
    
    if (curl_easy_setopt(m_curlEasyHandle, CURLOPT_HTTPPOST, formpost) != CURLE_OK)
        return false;

    m_formpost = formpost;

    return true;
}

bool Request::SetHeader(const char* header)
{
    m_curlSlist = curl_slist_append(m_curlSlist, header);
    return (m_curlSlist != NULL);
}

bool Request::SetCookie(const char* cookie)
{
    return (curl_easy_setopt(m_curlEasyHandle, CURLOPT_COOKIE, cookie) == CURLE_OK);
}

bool Request::SetProxy(const char* proxy)
{
    return (curl_easy_setopt(m_curlEasyHandle, CURLOPT_PROXY, proxy) == CURLE_OK);
}

bool Request::FollowLocation(int max_redirs)
{
    return (curl_easy_setopt(m_curlEasyHandle, CURLOPT_FOLLOWLOCATION, 1) == CURLE_OK &&
            curl_easy_setopt(m_curlEasyHandle, CURLOPT_MAXREDIRS, max_redirs) == CURLE_OK);
}

bool Request::SetTimeout(long seconds)
{
    return (curl_easy_setopt(m_curlEasyHandle, CURLOPT_TIMEOUT, seconds) == CURLE_OK);
}

void Request::Clear()
{
    curl_easy_cleanup(m_curlEasyHandle);
    curl_slist_free_all(m_curlSlist);
    m_response->Clear();
    m_curlEasyHandle = curl_easy_init();
}

Response::Response(Request* request)
: m_statusCode(0)
, m_headers("")
, m_body("")
, m_request(request)
, m_error(m_request ? HTTP_NO_ERROR : Http::HTTP_RESPONSE_INIT_ERROR)
{
}

Response::~Response()
{
}

void Response::Clear()
{
    m_statusCode = 0;
    m_body.clear();
    m_headers.clear();
    m_error = HTTP_NO_ERROR;
}

std::string Response::GetHeaderValue(const char* header)
{
    std::string value = "";

    const char* pos = String::Find(m_headers.c_str(), header, true);
    if(!pos)
        return "";
    pos += strlen(header);
    if(String::IsEmpty(pos))
        return "";

    while(*pos == ':' || *pos == ' ' || *pos == '\t')
        pos++;

    for(; !String::IsEmpty(pos) && *pos != '\r' && *pos != '\n'; pos++)
        value.append(pos, 1);

    return value;
}

} /* namespace Http */
} /* namespace Utils */
