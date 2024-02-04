#include"../../include/net/HttpRequest.h"

using namespace fei;

void HttpRequest::loadRequest(Buffer& buffer)
{
    size_t readablebytes = buffer.readableBytes();
    std::string rawRequest = buffer.retrieveAsString();
    ssize_t length = rawRequest.size();
    assert(length == readablebytes);

    loadRequestLine(rawRequest);
    loadHeaders(rawRequest);
    loadMessageBody(rawRequest);
    
    printf("-------------------------\n");
    if(method_ == HttpRequest::Method::GET)
        std::cout<<"the method is get\n";
    std::cout<<"target is=" <<target_ <<std::endl;
    if(version_ == HttpRequest::Version::HTTP11)
        std::cout<<"http11\n";
    else if(version_ == HttpRequest::Version::HTTP2)
        std::cout<<"http2\n";
    
    //std::cout<<"this is request headers\n";
    for(auto it : headers_)
    {
        std::cout<<it.first << ":" <<it.second <<std::endl;
    }
    std::cout<<"request headers over\n";

    if(headers_.count("Content-Length") == 0)
        std::cout<<"no content-length\n";
    else std::cout<<"Content-length=" << std::stoul(headers_["Content-Length"]) <<std::endl;
    std::cout<<message_body_;
}

void  HttpRequest::loadRequestLine(std::string& rawRequest)
{
    nextPhraseIdx_ = 0;
    size_t nextCR = findCRLF(rawRequest, nextPhraseIdx_);
    assert(nextCR != std::string::npos);

    //size_t blankplaceIdx = rawRequest.find(" ", nextPhraseIdx_, nextCR - nextPhraseIdx_ );
    size_t blankplaceIdx = SearchChar(rawRequest, ' ', nextPhraseIdx_, nextCR - nextPhraseIdx_ );

    //size_t blankplaceIdx = rawRequest.find(" ", nextPhraseIdx_, nextCR - nextPhraseIdx_ );
    if(rawRequest.substr(nextPhraseIdx_, blankplaceIdx - nextPhraseIdx_) == "GET")
        method_ = HttpRequest::Method::GET;
    else if(rawRequest.substr(nextPhraseIdx_, blankplaceIdx - nextPhraseIdx_) == "POST")
        method_ = HttpRequest::Method::POST;
    
    nextPhraseIdx_ = blankplaceIdx + 1;

    //assert(method_ == HttpRequest::Method::GET);

    blankplaceIdx = SearchChar(rawRequest, ' ', nextPhraseIdx_, nextCR - nextPhraseIdx_ );
    target_ = rawRequest.substr(nextPhraseIdx_, blankplaceIdx - nextPhraseIdx_);
    std::cout<<"raw url=" << target_ <<std::endl; 
    nextPhraseIdx_ = blankplaceIdx + 1;

    if(rawRequest.substr(nextPhraseIdx_, nextCR - nextPhraseIdx_) == "HTTP/1.1")
        version_ = HttpRequest::Version::HTTP11;
    else if(rawRequest.substr(nextPhraseIdx_, nextCR - nextPhraseIdx_) == "HTTP/2")
        version_ = HttpRequest::Version::HTTP2;

    nextPhraseIdx_ = nextCR + 2;

}
void HttpRequest::loadHeaders(std::string& rawRequest)
{
    while(nextPhraseIdx_ < rawRequest.length())
    {
        size_t nextCR = findCRLF(rawRequest, nextPhraseIdx_);
        if(nextCR == nextPhraseIdx_)
        {
            nextPhraseIdx_ += 2;
            return;
        }

        //size_t colonIdx = rawRequest.find(":", nextPhraseIdx_, nextCR - nextPhraseIdx_);
        size_t colonIdx = SearchChar(rawRequest, ':', nextPhraseIdx_, nextCR - nextPhraseIdx_);
        headers_[rawRequest.substr(nextPhraseIdx_, colonIdx - nextPhraseIdx_)] = rawRequest.substr(colonIdx + 2, nextCR - colonIdx - 2);
        nextPhraseIdx_ = nextCR + 2;
    }

}
void HttpRequest::loadMessageBody(std::string& rawRequest)
{
    if(nextPhraseIdx_ >= rawRequest.length())
        return;
    
    std::string length_str = headers_["Content-Length"];
    size_t content_length  = std::stoul(length_str);
    if(nextPhraseIdx_ + content_length != rawRequest.length() )
    {
        LOG_INFO<<"Content-Length error or read incomplete data";
        abort();
    }   
    message_body_ = rawRequest.substr(nextPhraseIdx_, content_length);
    
}