#include"../../include/net/Buffer.h"

#include <errno.h>
#include <memory.h>
#include <sys/uio.h>
using namespace fei;


ssize_t Buffer::readfd(int fd, int* saveErrno)
{
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writeable = writeableBytes();
    vec[0].iov_base = begin() + writerIdx_;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const ssize_t n = readv(fd, vec, 2);
    if(n < 0)
    {
        *saveErrno = errno;
    }
    else if((size_t)n <= writeable)
    {
        writerIdx_ += n;
    }
    else 
    {
        writerIdx_ = buffer_.size();
        append(extrabuf, n - writeable);
    }
    return n;
}
