#include "codec.h"
#include <muduo/base/Logging.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <set>
#include <stdio.h>

class ChatServer : boost::noncopyable
{
  public:
    ChatServer(muduo::net::EventLoop* loop, 
               const muduo::net::InetAddress& listenAddr)
      : server_(loop, listenAddr, "ChatServer"),
        codec_(boost::bind(&ChatServer::onStringMessage, this, _1, _2, _3))
    {
      server_.setConnectionCallback(
          boost::bind(&ChatServer::onConnection, this, _1));
      server_.setMessageCallback(
          boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    }

    void start()
    {
      server_.start();
    }

  private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn)
    {
      LOG_INFO << "ChatServer - " << conn->peerAddress().toIpPort() << " -> "
               << conn->localAddress().toIpPort() << " is "
               << (conn->connected() ? "UP" : "DOWN");
      if (conn->connected())
      {
        connections_.insert(conn);
      }
      else 
      {
        connections_.erase(conn);
      }
    }

    void onStringMessage(const muduo::net::TcpConnectionPtr& conn,
                         const muduo::string& message, 
                         muduo::Timestamp receiveTime)
    {
      for (ConnectionList::iterator it = connections_.begin();
           it != connections_.end();
           ++it)
      {
        codec_.send(get_pointer(*it), message);
      } 
    }

    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionList;
    ConnectionList connections_;
    muduo::net::TcpServer server_;
    LengthHeaderCodec codec_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    muduo::net::EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    muduo::net::InetAddress listenAddr(port);
    ChatServer server(&loop, listenAddr);
    server.start();
    loop.loop();
  }
  else
  {
    printf("Usage: %s port\n", argv[0]);
  }
}
