#ifndef _BUFFER_H_
#define _BUFFER_H_

#include<vector>
#include<string>
#include<algorithm>
#include<assert.h>
namespace fei
{

class Buffer
{
public:
    static const size_t kPrepend = 8;
    static const size_t kInitSize = 1024;

public:
    Buffer()
    :buffer_(kPrepend + kInitSize)
    ,readerIdx_(kPrepend)
    ,writerIdx_(kPrepend)
    {
        
    }
    void swap(Buffer& rhs)
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIdx_, rhs.readerIdx_);
        std::swap(writerIdx_, rhs.writerIdx_);
    }

    size_t readableBytes() const { return writerIdx_ - readerIdx_; }
    size_t writeableBytes() const { return buffer_.size() - writerIdx_; }
    size_t prependBytes() const { return readerIdx_; }
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        readerIdx_ += len;
    }

    void retrieveUntil(const char* end)
    {
        assert(beginRead() <= end);
        assert(end <= beginWrite());
        retrieve(end - beginRead());
    }

    void retrieveAll()
    {
        readerIdx_ = kPrepend;
        writerIdx_ = kPrepend;
    }
    std::string retrieveAsString()
    {
        std::string str(beginRead(), readableBytes());
        retrieveAll();
        return str;
    }
    char* beginWrite() { return begin() + writerIdx_; }
    char* beginRead() { return begin() + readerIdx_; }
    const char* end() { return &buffer_.back(); }
    void hasWritten(size_t len) { writerIdx_ += len; }
    
    void ensureWriteableBytes(size_t len)
    {
        if(writeableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writeableBytes() >= len);
    }
    void append(const char* data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }
 
    void append(const std::string& str)
    {
        append(str.data(), str.length());
    }

    const char peek() const 
    {
        return *(begin() + readerIdx_);
    }

    ssize_t readfd(int fd, int* saveErrno);
    bool isvalidRequest()
    {
        if(readableBytes() < 4)
            return false;
        char* read = beginRead() + (readableBytes() - 4);
        if(*read == '\r')
        {
            read++;
            if(*read=='\n')
            {
                read++;
                if(*read=='\r')
                {
                    read++;
                    if(*read=='\n')
                        return true;
                }
                return false;

            }
            return false;

        }
        return false;

    }
private:
    char* begin(){ return &*buffer_.begin(); } 

    const char* begin() const { return &*buffer_.begin(); } 


    void makeSpace(size_t len)
    {
        if(writeableBytes() + readerIdx_ < len + kPrepend )
        {
            buffer_.resize(writerIdx_ + len);
        }
        else 
        {
            size_t readable = readableBytes();
            std::copy(begin() + readerIdx_, begin() + writerIdx_, begin() + kPrepend);
            readerIdx_ = kPrepend;
            writerIdx_ = readerIdx_ + readable;
            assert(readable == readableBytes());
        }

    }
    std::vector<char> buffer_;
    size_t readerIdx_;
    size_t writerIdx_;

};


}

#endif