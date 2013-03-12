#ifndef _utils_HTTP_H_
#define _utils_HTTP_H_

#include <string>
#include <vector>

typedef void CURL;
typedef void CURLM;
struct curl_slist;
struct curl_httppost;

namespace Utils {
namespace Http {

enum ErrorCode
{
    HTTP_NO_ERROR = 0,
    HTTP_HANDLE_INIT_ERROR,
    HTTP_REQUEST_INIT_ERROR,
    HTTP_RESPONSE_INIT_ERROR,
};

struct FormField {
    std::string name;
    std::string contents;
    FormField() : name(""), contents("") {;}
    FormField(const char* n, const char* c) : name(n), contents(c) {;}
};
typedef std::vector<FormField> FormFields;
struct UploadFile {
    std::string name;
    std::string file;
    FormFields formFields;
    UploadFile() : name(""), file(""), formFields() {}
    UploadFile(const char* n, const char* f) : name(n), file(f), formFields() {}
};

class Request;
class Response;

class Handle 
{
public:    
    ~Handle();

    static Handle& Instance();

    static size_t HeadFunc(char *data, size_t size, size_t nmemb, Response *response);
    static size_t WriteFunc(char *data, size_t size, size_t nmemb, Response *response);

    bool AddRequest(Request& request);
    // Remove it if you want to reuse a request already added to Http.
    bool RemoveRequest(Request& request);
    bool SendRequests();
    bool SendRequest(Request& request);
    int Running() { return m_curlRunningHandles; }
    inline ErrorCode Error() { return m_error; }
private:
    Handle();
    ErrorCode m_error;
    CURLM *m_curlMutiHandle;
    int m_curlRunningHandles;
};

Handle& Instance();

class Request 
{
    friend class Handle;
public:
    Request(const char* url = NULL);
    ~Request();
    ErrorCode Error() { return m_error; }
    const std::string& Url() { return m_url; }
    bool SetUrl(const char* url);
    bool SetPost(const char* post);
    bool SetUploadFile(const UploadFile& uploadFile);
    bool SetHeader(const char* header);
    bool SetCookie(const char* cookie);
    bool SetProxy(const char* proxy);
    bool FollowLocation(int max_redirs);
    bool SetTimeout(long sec);
    Response* GetResponse() { return m_response; }
    void Clear();
private:
    CURL *m_curlEasyHandle;
    curl_slist *m_curlSlist;
    curl_httppost *m_formpost;
    ErrorCode m_error;
    std::string m_url;
    Response *m_response;
};

class Response 
{
    friend class Handle;
    friend class Request;
public:
    ErrorCode Error() { return m_error; }
    const char* Body() { return m_body.c_str(); }
    const long& StatusCode() { return m_statusCode; }
    std::string GetHeaderValue(const char* header);

    void Clear();
private:
    Response(Request* request);
    ~Response();

    ErrorCode m_error;
    Request *m_request;
    long m_statusCode;
    std::string m_headers;
    std::string m_body;
};

} /* namespace Http */
} /* namespace Utils */

#endif /* _utils_HTTP_H_ */
