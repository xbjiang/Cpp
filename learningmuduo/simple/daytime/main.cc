#include "daytime.h"
#include <muduo/net/EventLoop.h>

int main()
{
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(2013);
  DaytimeServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}
