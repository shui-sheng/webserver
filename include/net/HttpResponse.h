#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_
#include<string>
#include"HttpRequest.h"
#include"Buffer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

#define STR_HELPER(R) #R
#define STR(R) STR_HELPER(R)

#define HOME_DIR /home/zyf/wcnm/wwwroot
#define DEFAULT_NAME index.html
namespace fei
{
class HttpResponse
{
public:
    typedef HttpRequest::Version Version;

    //https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
    enum class StatusCode
    { 
        OK = 200,
        BadRequest = 400,
        NotFound = 404,
    };
    void phraseRequest(HttpRequest& request)
    {
        setVersion(request.getVersion());
        statuscode_ = setStatusCode(request.getTarget());
        //setHeader("Content-Type", "text/html; charset=utf-8");
        headers_.clear();
        setHeaders();
        message_body_.clear();

        setMessageBody();
    }

    std::string getResponse()
    {
        std::string rawResponse;
        addVersion(rawResponse);
        addStatus(rawResponse);
        addHeaders(rawResponse);
        addMessageBody(rawResponse);
        return rawResponse;
        
    }
public:
    StatusCode getStatusCode() { return statuscode_; }
private:


// HTTP/1.1 200 OK
// Date: Sun, 21 Jan 2024 08:30:01 GMT
    void setVersion(Version version) { version_ = version; }
    StatusCode setStatusCode(std::string target);
    void setHeaders();
    
    void setMessageBody();

    


private:
    void addVersion(std::string& rawResponse)
    {
        if(version_ == Version::HTTP11)
            rawResponse.append("HTTP/1.1 ");
    }
    void addStatus(std::string& rawResponse)
    {
        if(statuscode_ == StatusCode::OK) 
        {
            rawResponse.append("200 OK\r\n");
        }
        else if(statuscode_ == StatusCode::BadRequest)
        {
            rawResponse.append("400 OK\r\n");
        }
        else if(statuscode_ == StatusCode::NotFound)
        {
            rawResponse.append("404 Not Found\r\n");
        }
    }
    void addHeaders(std::string& rawResponse)
    {
        for(auto& item : headers_)
        {
            appendLine(rawResponse, item.first, item.second);
        }
        rawResponse.append("\r\n");
    }   
    void addMessageBody(std::string& rawResponse)
    {
        rawResponse.append(message_body_);
    }
    void setOneHeader(const std::string& key, const std::string& value)
    {
        headers_[key] = value;
    }
    void appendLine(std::string& rawResponse,const std::string& key ,const std::string& value)
    {
            rawResponse.append(key + ": " + value + "\r\n");

    }
    Version version_;
    StatusCode statuscode_;
    std::string reason_phrase_;
    std::unordered_map<std::string, std::string> headers_;
    std::string message_body_;

    std::string target_;
    size_t file_size_ = SIZE_MAX;



};


// HTTP/1.1 200 OK
// Date: Sun, 21 Jan 2024 08:30:01 GMT
// Content-Type: text/html; charset=utf-8
// Transfer-Encoding: chunked
// Connection: keep-alive
// Server: WAF
// Last-Modified: Sun, 21 Jan 2024 08:29:02 GMT
// Vary: Accept-Encoding
// ETag: W/"65acd5ce-81456"
// Strict-Transport-Security: max-age=0; preload
// X-Request-Id: decfc4987f994cd54b732cea184b4a77
// X-Cache: BYPASS


} 


#endif