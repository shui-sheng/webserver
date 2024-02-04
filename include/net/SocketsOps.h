#ifndef _SOCKETSOPS_H_
#define _SOCKETSOPS_H_
#include <sys/types.h> 
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
namespace fei
{

namespace socket
{
class InetAddr;
//create,bind,listen,accept,

void setNonBlock(int sockfd);
int createNonBlock();

int bind(int sockfd, struct sockaddr_in& addr,socklen_t addrlen);

int listen(int sockfd,int backlog = 1024);

int accept(int sockfd, struct sockaddr_in& addr, socklen_t* addrlen);


void toHostPort(char* buf, size_t size,struct sockaddr_in& addr);
void toHostIp(char* buf, size_t size,  struct sockaddr_in& addr);
void toHostIpPort(char* buf, size_t size,  struct sockaddr_in& addr);
struct sockaddr_in getLocalAddr(int sockfd);




}



}



#endif