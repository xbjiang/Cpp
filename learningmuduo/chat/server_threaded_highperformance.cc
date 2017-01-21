#include "codec.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

#include <muduo/base/ThreadLocalSingleton.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Logging.h>

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

    void setThreadNum(int numThreads)
    {
      server_.setThreadNum(numThreads);
    }
  
    void start()
    {
      server_.setThreadInitCallback(boost::bind(&ChatServer::threadInit, this, _1));
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
        LocalConnections::instance().insert(conn); 
      }
      else 
      {
        LocalConnections::instance().erase(conn);
      }
    }

    void onStringMessage(const muduo::net::TcpConnectionPtr& conn,
                         const muduo::string& message, 
                         muduo::Timestamp time)
    {
      muduo::net::EventLoop::Functor f = boost::bind(&ChatServer::distributeMessage, this, message);
      muduo::MutexLockGuard lock(mutex_);
      for (std::set<muduo::net::EventLoop*>::const_iterator it = loops_.begin();
           it != loops_.end();
           ++it)
      {
        (*it)->queueInLoop(f);
      }
    }

    void distributeMessage(const muduo::string& message)
    {
      for (ConnectionList::const_iterator it = LocalConnections::instance().begin();
           it != LocalConnections::instance().end();
           ++it)
      {
        codec_.send(get_pointer(*it), message); 
      }
    }

    void threadInit(muduo::net::EventLoop* loop)
    {
      assert(LocalConnections::pointer() == NULL);
      LocalConnections::instance();
      assert(LocalConnections::pointer() != NULL);
      muduo::MutexLockGuard lock(mutex_);
      loops_.insert(loop);
    }

    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionList;
    typedef muduo::ThreadLocalSingleton<ConnectionList> LocalConnections;
    muduo::MutexLock mutex_;
    muduo::net::TcpServer server_;
    std::set<muduo::net::EventLoop*> loops_;
    LengthHeaderCodec codec_;
};

int main(int argc, char* argv[])
{
  if (argc > 1)
  {
    muduo::net::EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    muduo::net::InetAddress listenAddr(port);
    ChatServer server(&loop, listenAddr);
    if (argc > 2)
    {
      server.setThreadNum(atoi(argv[2]));
    }
    server.start();
    loop.loop();
  }
  else 
  {
    printf("Usage: %s port [thread_num]\n", argv[0]);
  }
}
