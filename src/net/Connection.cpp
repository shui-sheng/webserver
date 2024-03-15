#include"../../include/net/Connection.h"
#include <netinet/tcp.h> 
using namespace fei;
Connection::Connection(EventLoop* loop, int acceptfd, InetAddr& localAddr, InetAddr& peerAddr, std::string name)
    :loop_(loop)
    ,connectSocket_(new Socket(acceptfd))
    ,connectChannel_(new Channel(loop, acceptfd))
    ,peerAddr_(peerAddr)
    ,localAddr_(localAddr)
    ,name_(name)
{
    LOG_INFO<<"new connection name = " << name ; 
    connectChannel_->setReadCallback(std::bind(&Connection::handleRead, this));
    connectChannel_->setWriteCallback(std::bind(&Connection::handleWrite, this));
    connectChannel_->setCloseCallback(std::bind(&Connection::handleClose, this));
    connectChannel_->setErrorCallback(std::bind(&Connection::handleError, this));
    

}

void Connection::established()
{
    loop_->assertInLoopThread() ;
    assert(connectSocket_->Fd() == connectChannel_->fd());
    connectChannel_->enableReading();

}
void Connection::handleRead()
{
    loop_->assertInLoopThread();
    ssize_t n = inbuffer_.readfd(connectSocket_->Fd(), &errno);
    if(n > 0)
    {
        //msgInCallback_(connectSocket_->Fd(), peerAddr_);
    //printf("get info from %d, peeripport=%s\n", connectSocket_->Fd(), peerAddr_.toIpPort().c_str());
        // if(inbuffer_.isvalidRequest())
        // {
        //     context_.phraseRequest(inbuffer_);
        //     std::string response = context_.getResponse();
        //     send(response);
        // }       
        /*
            int ret = parse_and_build(inbuffer);
            if(ret == ok_send)
                send;
            else 
                
        */
       HttpContext::ProcessState ret = context_.parse_and_build(inbuffer_);
       if(ret == HttpContext::ProcessState::PROCESS_BUILD_FINISH)
            send(context_.outstr_);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else 
    {

    }
    
}
void Connection::handleWrite()
{
    //outbuffer_.append(inbuffer_.peek());
    // outbuffer_.append("1234over");
    //msgOutCallback_(this, outbuffer_, Timestamp::now());

    loop_->assertInLoopThread();
    if(outbuffer_.readableBytes() == 0)
    {
        abort();
    }
    bool isError = false;
    size_t remaing = outbuffer_.readableBytes();
    ssize_t nwrote =  write(connectChannel_->fd(),outbuffer_.beginRead(), outbuffer_.readableBytes());
    if(nwrote >= 0)
    {
        remaing -= nwrote;
        outbuffer_.retrieve(nwrote);
        assert(connectChannel_->isWriting());
        if(remaing == 0)//all data in outbuffer is sent 
        {
            connectChannel_->removeWriting();
        }
    }
    else 
    {
        isError = true;
    }


}
void Connection::handleClose()
{
    loop_->assertInLoopThread();

    connectChannel_->removeAll();
    close(connectSocket_->Fd());
    LOG_INFO<<"handleClose fd = " << connectSocket_->Fd();
    closeCallback_(this);

}
void Connection::handleError()
{

}
void Connection::send(const char* str)
{
    sendInLoop(str, strlen(str));

}

void Connection::send(const std::string& str)
{
    loop_->assertInLoopThread();

    //outbuffer_.swap(inbuffer_);

    sendInLoop(str.data(), str.size());

}
void Connection::send(char* data, size_t len)
{
    sendInLoop(data, len);
}

void Connection::sendInLoop(const char* data, size_t len)
{
    if(context_.headers["Connection"] == " Keep-Alive")
    {
         int keepAlive = 1;      //开启keepalive属性,缺省值:0(关闭)
        int keepIdle = 5;       //如果在5秒内没有任何数据交互,则进行探测,缺省值:7200(s)
        int keepInterval = 2;   //探测时发探测包的时间间隔为2秒,缺省值:75(s)
        int keepCount = 1;      //探测重试的次数,全部超时则认定连接失效,缺省值:9(次)
        setsockopt(connectChannel_->fd(), SOL_SOCKET,SO_KEEPALIVE,(void *)&keepAlive,sizeof(keepAlive));
       

        setsockopt(connectChannel_->fd(), IPPROTO_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
        setsockopt(connectChannel_->fd(), IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
        setsockopt(connectChannel_->fd(), IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));

        //loop_->runAfter(30, std::bind(&Connection::handleClose, this));
    }
    loop_->assertInLoopThread();
    bool iserror = false;
    size_t remaining = len;
    ssize_t nwrote = 0;
    if(!connectChannel_->isWriting() && outbuffer_.readableBytes() == 0)
    {
        nwrote = write(connectChannel_->fd(), data, len);
        if(nwrote >= 0)
        {
            remaining = len - nwrote;
            if(remaining == 0)
            {
                //do nothing
                //handleClose();
            }
        }
        else 
        {
            //error
            iserror = true;
        }
    }
    if(!iserror && remaining > 0)
    {
        outbuffer_.append(data + nwrote, remaining);
        if(!connectChannel_->isWriting())
        {
            connectChannel_->enableWriting();
        }
    }


}