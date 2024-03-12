#ifndef _HTTPCONTEXT_H_
#define _HTTPCONTEXT_H_

#include"Buffer.h"
#include<unordered_map>
#include<string>
//#define WWWROOT_ABSPATH /home/zyf/webserver/wwwroot
namespace fei
{
class HttpContext
{

public:

    enum class Method { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH };
    enum class Version { HTTP10, HTTP11, HTTP2, HTTP3 };
    enum class StatusCode
    { 
        OK,
        FORBIDDEN,
        ERROR,
        NOCONTENT,
        CREATED
    };
    enum class ProcessState
    { 
        PROCESS_PARSE_URL,
        PROCESS_PARSE_HEADERS,
        PROCESS_PARSE_BODY,
        PROCESS_PARSE_ANALYSIS_AND_BUILD,
        PROCESS_PARSE_BUILD,
        PROCESS_BUILD_FINISH

    };
    enum class ParseUrlRet
    {
        PARSE_SUCCESS,
        PARSE_AGAIN,
        PARSE_ERROR
    };
    enum class ParseHeadersRet
    {
        PARSE_SUCCESS,
        PARSE_AGAIN,
        PARSE_ERROR
    };
    enum class ParseBodysRet
    {
        PARSE_SUCCESS,
        PARSE_AGAIN,
        PARSE_ERROR
    };
    enum class AnalysissRet
    {
        ANALYSIS_OK,
        ANALYSIS_FORBIDDEN,
        ANALUSIS_ERROR

    };
    HttpContext()
        //:processstate_(ProcessState::PROCESS_PARSE_URL)
        //,instr_()
    {
        
    }
public:

    ProcessState parse_and_build(Buffer& inbuf)
    {
        std::string appendstr = inbuf.retrieveAsString();//append the new data to instr_ so
        instr_.append(appendstr);
        // [old_data---new_data---future_data]
        do
        {
            //we need clear the state
            if(processstate_ == ProcessState::PROCESS_BUILD_FINISH)
            {
                processstate_ = ProcessState::PROCESS_PARSE_URL;
                outstr_.clear();
            }
            if(processstate_ == ProcessState::PROCESS_PARSE_URL)
            {
                ParseUrlRet ret = parse_Url();
                if(ret == ParseUrlRet::PARSE_SUCCESS)
                {
                    processstate_ = ProcessState::PROCESS_PARSE_HEADERS;
                }
                else if(ret == ParseUrlRet::PARSE_AGAIN)
                {
                    break;
                }
                else if(ret == ParseUrlRet::PARSE_ERROR)
                {
                    //error
                }
            }
            if(processstate_ == ProcessState::PROCESS_PARSE_HEADERS)
            {
                ParseHeadersRet ret = parse_headers();
                if(ret == ParseHeadersRet::PARSE_SUCCESS)
                {
                    if(method_ == Method::POST)
                        processstate_ = ProcessState::PROCESS_PARSE_BODY;
                    else if(method_ == Method::GET)
                        processstate_ = ProcessState::PROCESS_PARSE_ANALYSIS_AND_BUILD;
                }
                else if(ret == ParseHeadersRet::PARSE_AGAIN)
                {
                    break;
                }
                else if(ret == ParseHeadersRet::PARSE_ERROR)
                {
                    
                }
            }
            if(processstate_ == ProcessState::PROCESS_PARSE_BODY)
            {
                //ok, we have parse the empty line
                ParseBodysRet ret = parse_body();
                if(ret == ParseBodysRet::PARSE_AGAIN)
                {
                    break;
                }
                else if(ret == ParseBodysRet::PARSE_SUCCESS)
                {
                    processstate_ = ProcessState::PROCESS_PARSE_ANALYSIS_AND_BUILD;
                }
                else 
                {

                }
                
            }
            if(processstate_ == ProcessState::PROCESS_PARSE_ANALYSIS_AND_BUILD)
            {
                AnalysissRet ret = analysis_and_build();
                if(ret == AnalysissRet::ANALYSIS_OK)
                {
                    processstate_ = ProcessState::PROCESS_PARSE_BUILD;
                }


            }
            if(processstate_ == ProcessState::PROCESS_PARSE_BUILD)
            {
                build_response();
                processstate_ = ProcessState::PROCESS_BUILD_FINISH;
            }

        } while (false);


        return processstate_;
        


    }

private:
    ParseUrlRet parse_Url();
    ParseHeadersRet parse_headers();
    ParseBodysRet parse_body();
    AnalysissRet analysis_and_build();
    void build_response();
private:
    Method method_;
    Version version_;
    std::string url_raw_path_;
    //size_t content_length_;

    std::unordered_map<std::string, std::string> url_args;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> body_args_;

public:
    

    //Buffer inbuf_;
    StatusCode statuscode_;
    
    ProcessState processstate_ = ProcessState::PROCESS_PARSE_URL;
    std::string instr_;
    std::string outstr_;
    std::string body_;


};    
}


#endif