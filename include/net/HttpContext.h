#ifndef _HTTPCONTEXT_H_
#define _HTTPCONTEXT_H_
#include"HttpRequest.h"
#include"HttpResponse.h"
#include<string>

namespace fei
{
class HttpContext
{

public:
    typedef HttpRequest::Method Method;
    typedef HttpRequest::Version Version;
    typedef HttpResponse::StatusCode  StatusCode;
public:
    //void loadRequest(Buffer& buf) { request_.loadRequest(buf); }

// struct stat {
//     dev_t     st_dev;         /* ID of device containing file */
//     ino_t     st_ino;         /* Inode number */
//     mode_t    st_mode;        /* File type and mode */
//     nlink_t   st_nlink;       /* Number of hard links */
//     uid_t     st_uid;         /* User ID of owner */
//     gid_t     st_gid;         /* Group ID of owner */
//     dev_t     st_rdev;        /* Device ID (if special file) */
//     off_t     st_size;        /* Total size, in bytes */
//     blksize_t st_blksize;     /* Block size for filesystem I/O */
//     blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

//     /* Since Linux 2.6, the kernel supports nanosecond
//         precision for the following timestamp fields.
//         For the details before Linux 2.6, see NOTES. */

//     struct timespec st_atim;  /* Time of last access */
//     struct timespec st_mtim;  /* Time of last modification */
//     struct timespec st_ctim;  /* Time of last status change */

// #define st_atime st_atim.tv_sec      /* Backward compatibility */
// #define st_mtime st_mtim.tv_sec
// #define st_ctime st_ctim.tv_sec
// };

    StatusCode phraseRequest(Buffer& inbuf)
    {
        request_.loadRequest(inbuf);
        response_.phraseRequest(request_);
    }

    std::string getResponse() 
    {   
        return response_.getResponse();
    }


private:

    HttpRequest request_;
    HttpResponse response_;
    Buffer inbuf_;
    
    StatusCode statuscode_;
};    
}


#endif