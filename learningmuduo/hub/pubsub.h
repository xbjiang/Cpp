#ifndef __LEARNINGMUDUO_HUB_PUBSUB_H__
#define __LEARNINGMUDUO_HUB_PUBSUB_H__

#include <muduo/net/TcpClient.h>
#include <boost/noncopyable.hpp>

namespace pubsub
{
using muduo::string;
using muduo::Timestamp;
class PubSubClient : boost::noncopyable
{
  public:
    typedef boost::function<void (const string& topic,
                                  const string& content,
                                  Timestamp)> SubscribeCallback;
    
    PubSubClient(muduo::net::EventLoop* loop,
                 const muduo::net::InetAddress& hubAddr,
                 const string& name);
    void start();
    void stop();
    bool connected() const;
    
    bool subscribe(const string& topic, const SubscribeCallback& cb);
    void unsubscribe(const string& topic);
    bool publish(const string& topic, const string& content);
  private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   Timestamp receiveTime);
    bool send(const string& message);

    muduo::net::TcpConnectionPtr conn_;
    SubscribeCallback subscribeCallback_;
    muduo::net::TcpClient client_;
};
}
#endif
