#ifndef __LEARNINGMUDUO_SIMPLE_DISCARD_DISCARD_H__
#define __LEARNINGMUDUO_SIMPLE_DISCARD_DISCARD_H__

#include <muduo/net/TcpServer.h>

class DiscardServer
{
  public: 
    DiscardServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listenAddr);

    void start();

  private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time);

    muduo::net::TcpServer server_;
};
#endif
