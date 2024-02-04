#ifndef _FILEUTIL_H_
#define _FILEUTIL_H_


#include<stdio.h>
#include<string>
#include<sys/types.h>
namespace fei
{
namespace FileUtil
{
class AppendFile 
{
public:
    explicit AppendFile(const char* filename);

    ~AppendFile();

    void append(const char* logline, size_t len);

    void flush();

    off_t writtenBytes() const { return writtenBytes_; }
private:

    size_t write(const char* logline, size_t len);

    std::FILE* fp_;
    char buffer_[64*1024];
    off_t writtenBytes_;
};
}


}
#endif