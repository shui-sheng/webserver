#include"../../include/log/FileUtil.h"
#include<assert.h>
#include<fcntl.h>
using namespace fei;

FileUtil::AppendFile::AppendFile(const char* filename)
    :fp_(::fopen(filename, "ae")),
    writtenBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
    
}

FileUtil::AppendFile::~AppendFile()
{
    ::fclose(fp_);
}

void FileUtil::AppendFile::append(const char* logline, size_t len)
{
    size_t written = 0;
    while(written != len)
    {
        size_t remain = len - written;
        size_t n = write(logline + written, remain);
        if(n != remain)
        {
            int err = ferror(fp_);
            if(err)
            {
                fprintf(stderr, "AppendFile::append() failed \n");
                break;
            }
        }
        written += n;
    }
    writtenBytes_ += written;
}
void FileUtil::AppendFile::flush()
{
    ::fflush(fp_);
}
size_t FileUtil::AppendFile::write(const char* logline, size_t len)
{
    return ::fwrite_unlocked(logline, 1, len, fp_);
}

