#include "codec.h"
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <stdio.h>
#include <iostream>

class ChatClient : boost::noncopyable
{
  public:
    ChatClient(muduo::net::EventLoop* loop, 
               const muduo::net::InetAddress& serverAddr)
      : client_(loop, serverAddr, "ChatClient"), 
        codec_(boost::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
    {
      client_.setConnectionCallback(
          boost::bind(&ChatClient::onConnection, this, _1));
      client_.setMessageCallback(
          boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    }

    void connect()
    {
      client_.connect();
    }

    void disconnect()
    {
      client_.disconnect(); 
    }
   
    void write(const muduo::StringPiece& message)
    {
      muduo::MutexLockGuard lock(mutex_);
      if (connection_)
      {
        codec_.send(get_pointer(connection_), message);
      }
    }

  private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn)
    {
       LOG_INFO << conn->localAddress().toIpPort() << " -> "
                << conn->peerAddress().toIpPort() << " is " 
                << (conn->connected() ? "UP" : "DOWN");
       muduo::MutexLockGuard lock(mutex_);
       if (conn->connected())
       {
         connection_ = conn;
       }
       else 
       {
         connection_.reset();
       }
    }

    void onStringMessage(const muduo::net::TcpConnectionPtr&,
                         const muduo::string& message,
                         muduo::Timestamp)
    {
      printf("<<< %s\n", message.c_str());
    }

    muduo::net::TcpClient client_;
    LengthHeaderCodec codec_;
    muduo::MutexLock mutex_;
    muduo::net::TcpConnectionPtr connection_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 2)
  {
    muduo::net::EventLoopThread loopThread;
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    muduo::net::InetAddress serverAddr(argv[1], port);
    
    ChatClient client(loopThread.startLoop(), serverAddr);
    client.connect();
    std::string line;
    while (std::getline(std::cin, line))
    {
      client.write(line);
    }
    client.disconnect();
    muduo::CurrentThread::sleepUsec(1000*1000);
  }
  else 
  {
    printf("Usage: %s host_ip port\n", argv[0]);
  }
}
