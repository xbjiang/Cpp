#include "pubsub.h"
#include <muduo/net/InetAddress.h>
#include <muduo/net/Buffer.h>

#include <boost/bind.hpp>

using namespace pubsub;

PubSubClient::PubSubClient(muduo::net::EventLoop* loop,
                           const muduo::net::InetAddress& hubAddr,
                           const string& name)
    : client_(loop, hubAddr, name)
{
  client_.setConnectionCallback(
      boost::bind(&PubSubClient::onConnection, this, _1));
  client_.setMessageCallback(
      boost::bind(&PubSubClient::onMessage, this, _1, _2, _3));
}

void PubSubClient::start()
{
  client_.connect();
}

void PubSubClient::stop()
{
  client_.disconnect();
}

bool PubSubClient::connected() const 
{
  return conn_ && conn_->connected(); 
}

void PubSubClient::setConnectionCallback(const ConnectionCallback& cb)
{
  connectionCallback_ = cb;
}
bool PubSubClient::subscribe(const string& topic,
                             const SubscribeCallback& cb)
{
  string message = "sub " + topic + "\r\n";
  subscribeCallback_ = cb;
  return send(message);  
}

void PubSubClient::unsubscribe(const string& topic)
{
  string message = "unsub " + topic + "\r\n";
  send(message);
}

bool PubSubClient::publish(const string& topic, const string& content)
{
  string message = "pub " + topic + "\r\n" + content + "\r\n";
  return send(message);
}

void PubSubClient::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    conn_ = conn;
  } 
  else 
  {
    conn_.reset();
  }
}

void PubSubClient::onMessage(const muduo::net::TcpConnectionPtr& conn,
               muduo::net::Buffer* buf,
               Timestamp receiveTime)
{
  ParseResult result = kError;
  while (result == kSuccess)
  {
    string cmd;
    string topic;
    string content;
    result = parseMessage(buf, &cmd, &topic, &content);
    if (result == kSuccess)
    {
      if (cmd == "pub" && subscribeCallback_)
      {
        subscribeCallback_(topic, content, receiveTime);
      }
    }
    else if (result == kError)
    {
      conn->shutdown();
    }
  }
}

void PubSubClient::send(const string& message)
{
  bool succeed = false;
  if (conn_ && conn_->connected())
  {
    conn_->send(message);
    succeed = true;
  }
  return succeed;
} 
