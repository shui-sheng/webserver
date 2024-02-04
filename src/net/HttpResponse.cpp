#include"../../include/net/HttpResponse.h"
using namespace fei;

HttpResponse::StatusCode  HttpResponse::setStatusCode(std::string target)
{
    //GET / 
    //LOG_INFO<<"target=" << target;
    size_t split = target.find('?',0);
    if(split != std::string::npos)
    {
        //have args in target
        //
    }
    std::string url = target.substr(0, split);
    std::cout<<"raw url=" << url<<std::endl;

    
    ///home/zyf/wcnm/wwwroot + /zyf.txt
    std::string abs_target_path = std::string(STR(HOME_DIR)) + url;

    struct stat buf;
    int ret = stat(abs_target_path.c_str(), &buf);
    std::cout<<"stat arg1=" << abs_target_path<<std::endl;
    assert(abs_target_path == "/home/zyf/webserver/wwwroot/index.html");
    if(ret == -1)
    {
        if(errno == EACCES)
        {
            return StatusCode::BadRequest;
        }
        else if(errno == EFAULT || errno == ENAMETOOLONG)
        {
            return StatusCode::NotFound;
        }
        return StatusCode::BadRequest;
    }
    if((buf.st_mode & S_IFMT) ==  S_IFREG)
    {
        file_size_ = buf.st_size;
    }
    else if((buf.st_mode & S_IFMT) == S_IFDIR)
    {
        return StatusCode::NotFound;
    }
    target_ = abs_target_path;

    return StatusCode::OK;

}
void HttpResponse::setHeaders()
{
    setOneHeader("Content-Type", "text/html; charset=utf-8; image/jpeg");
    if(file_size_ != SIZE_MAX)
        setOneHeader("Content-Length", std::to_string(file_size_));
}
void HttpResponse::setMessageBody()
{
    if(file_size_ == SIZE_MAX)
        return;
    if(statuscode_ != StatusCode::OK)
        return;
    
    FILE* file = fopen(target_.c_str(), "r");
    if(file == NULL)
    {
        LOG_INFO<<"File cannot open";
        setOneHeader("Content-Length", "0");
        statuscode_ = StatusCode::BadRequest;
        fclose(file);
        return;
    }
    void *dynamicmem = malloc(file_size_);
    size_t nread = fread(dynamicmem, file_size_, file_size_, file);
    if(nread == - 1)
    {
        LOG_INFO<<"File cannot read";
        setOneHeader("Content-Length", "0");
        statuscode_ = StatusCode::BadRequest;
        fclose(file);
        free(dynamicmem);
        return;
    }

    if(message_body_.size() != 0)
    {
        LOG_INFO<< "mesgbodysize = " << message_body_.size();
        std::cout <<"mesgbodysize = " << message_body_.size();
        sleep(1);
        assert(0);
    }
    message_body_.clear();
    message_body_.append((char*)dynamicmem, file_size_);
    fclose(file);
    free(dynamicmem);

}