
#include"../include/net/HttpContext.h"
#include"../include/net/Buffer.h"
#include<string>
using namespace fei;
int main()
{
    Buffer buf;
    char data[10000] = "GET / HTTP/1.1\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
Cache-Control: max-age=0\r\n\
Connection: keep-alive\r\n\
Content-Length: 10\r\n\
\r\n\
1234567890";
    buf.append(data, strlen(data));
    HttpContext context;
    context.phraseRequest(buf);

    printf("----------------------------\n");
    std::string response = context.getResponse();
    std::cout<<response;
}

//GET / HTTP/1.1\r\nAccept: ;q=0.8,application/signed-exchange;v=b3;q=0.7\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\nCache-Control: max-age=0\r\nConnection: keep-alive\r\nContent-Length: 10\r\n\r\n1234567890