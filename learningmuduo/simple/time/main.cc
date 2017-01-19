#include "time.h"
#include <muduo/net/EventLoop.h>

int main()
{
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(2037);
  TimeServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}
