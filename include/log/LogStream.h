#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_

#include <assert.h>
#include <string> // memcpy
#include<string.h>
#include<strings.h>
#include <algorithm>

namespace fei
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000*1000;

template<int SIZE>
class FixedBuffer
{
public:
    FixedBuffer()
      : cur_(data_)
    {}
    ~FixedBuffer()
    {}
    void append(const char* buf, size_t len)
    {
      if (avail() > len)
      {
        memcpy(cur_, buf, len);
        cur_ += len;
      }
    }
	void append(const char ch)
	{
		if(avail() > 1)
		{
			memcpy(cur_, &ch, 1);
			cur_ += 1;
		}
	}
    const char* data() const { return data_; }
    size_t length() const { return static_cast<size_t>(cur_ - data_); }

    char* current() { return cur_; }
    size_t avail() const { return static_cast<size_t>(end() - cur_); }
    void add(size_t len) { cur_ += len; }

    void reset() { cur_ = data_; }
    void bzero() 
	{ 
		//::bzero((void*)data_, sizeof(data_)); 
		reset();
	}

    std::string toString() const { return string(data_, length()); }


private:
    const char* end() const { return data_ + sizeof data_; }
    // Must be outline function for cookies.

    char data_[SIZE];
    char* cur_;
};



class LogStream 
{
public:
  	typedef LogStream self;
	typedef FixedBuffer<kSmallBuffer> Buffer;
	self& operator<<(void* p)
	{
		size_t sz = sizeof(void*);
		void** pp = &p;
		char buf[sizeof(void*)];
		snprintf(buf, sizeof(buf), "%x", *pp);

		buffer_.append(buf, sizeof(void*));
	}

	self& operator<<(bool v)
	{
		buffer_.append(v ? "1" : "0", 1);
		return *this;
	}
	self& operator<<(char v)
	{
		buffer_.append(&v, 1);
		return *this;
	}
	self& operator<<(const char* str)
	{
		if (str)
		{
			buffer_.append(str, strlen(str));
		}
		else
		{
			buffer_.append("(null)", 6);
		}
		return *this;
	}
	self& operator<<(unsigned int v)
	{
		if(v == 0)
		{
			buffer_.append('0');
			return *this;
		}
		char buf[10];
		size_t idx = 0;
		while(v)
		{
			buf[idx++] = (v % 10) + '0';
			v /= 10;
		}
		std::reverse(buf, buf + idx);
		buffer_.append(buf, idx);
		return *this;
	}
	self& operator<<(int v)
	{
		if(v == 0)
		{
			buffer_.append('0');
			return *this;
		}
		char buf[10];
		size_t idx = 0;
		while(v)
		{
			buf[idx++] = (v % 10) + '0';
			v /= 10;
		}
		std::reverse(buf, buf + idx);
		buffer_.append(buf, idx);
		return *this;
	}
	self& operator<<(unsigned long int v)
	{
		if(v == 0)
		{
			buffer_.append('0');
			return *this;
		}
		char buf[10];
		size_t idx = 0;
		while(v)
		{
			buf[idx++] = (v % 10) + '0';
			v /= 10;
		}
		std::reverse(buf, buf + idx);
		buffer_.append(buf, idx);
		return *this;
	}
	
	self& operator<<(const std::string& v)
	{
		buffer_.append(v.c_str(), v.size());
		return *this;
	}


	void append(const char* data, size_t len) { buffer_.append(data, len); }
	const Buffer& buffer() const { return buffer_; }
	void resetBuffer() { buffer_.reset(); }

private:

	Buffer buffer_;

};

}

#endif