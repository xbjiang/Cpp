#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

#include <set>
#include <map>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;
 
namespace pubhub
{

typedef std::set<string> ConnectionSubscription;

class Topic 
{
  public: 
    Topic(const string& topic)
      : topic_(topic);
    {
    }

    void add(const TcpConnectionPtr& conn)
    {
      audiences_.insert(conn);
      if (lastPubTime_.valid())
      {
        conn->send(makeMessage());
      }
    }

    void remove(const TcpConnectionPtr& conn)
    {
      audiences_.erase(conn);
    }

    void publish(const string& content, Timestamp time)
    {
      content_ = content;
      lastPubTime = time;
      string message = makeMessage();
      for (std::set<TcpConnectionPtr>::iterator it = audiences_.begin();
           it != audiences_.end();
           ++it)
      {
        (*it)->send(message);
      }
    }
    
  private:
    
    string makeMessage()
    {
      return "pub " + topic_ + "\r\n" + content_ + "\r\n";
    }

    string topic_;
    string content_;
    Timestamp lastPubTime_;
    std::set<TcpConnectionPtr> audiences_;
};

class PubSubServer : boost::noncopyable
{
  public:
    PubSubServer(EventLoop* loop, const InetAddress& listenAddr)
        : loop_(loop), server_(loop, listenAddr, "PubSubServer")
    {
      server_.setConnectionCallback(
          boost::bind(&PubSubServer::onConnection, this, _1));
      server_.setMessageCallback(
          boost::bind(&PubSubServer::onMessage, this, _1, _2, _3));
      loop_.runEvery(1.0, &PubSubServer::timePublish, this);
    }

    void start()
    {
      server_.start();
    }

  private:

    void onConnection(const TcpConnectionPtr& conn)
    {
      if (conn->connected())
      {
        conn->setContext(ConnectionSubscription());
      }
      else 
      {
        ConnectionSubscription& connSub 
            = boost::any_cast<ConnectionSubscription&>(conn->getContext());
        for (ConnnectionSubscription::iterator it = connSub.begin();
             it != connSub.end())
        {
          doUnsubscribe(conn, *it++);
        }
      }
    }

    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf, 
                   Timestamp receiveTime)
    {
      ParseResult result = kSuccess;
      while (result == kSuccess)
      {
        string cmd;
        string topic;
        string content;
        result = parseMessage(buf, &cmd, &topic, &content);
        if (result == kSuccess)
        {
          if (cmd == "pub")
          {
            doPublish(conn->name(), topic, content, receiveTime);
          }
          else if (cmd == "sub")
          {
            LOG_INFO << conn->name() << " subscribes " << topic;
            doSubscribe(conn, topic);
          }
          else if (cmd == "unsub")
          {
            doUnsubscribe(conn, topic);
          }
          else 
          {
            conn->shutdown();
            result = kError;
          }
        }
        else if (result == kError)
        {
          conn->shutdown();
        }
      }
    }

    void doPublish(const string& source,
                   const string& topic,
                   const string& content,
                   Timestamp time)
    {
      getTopic(topic).publish(content, time); 
    }

    void doSubscribe(const TcpConnectionPtr& conn,
                     const string& topic)
    {
      ConnectionSubscription* connSub = 
          boost::any_cast<ConnectionSubscription*>(conn->getMutableContext());
      connSub->insert(topic);
      getTopic(topic).add(conn);
    }

    void doUnsubscribe(const TcpConnectionPtr& conn,
                       const string& topic)
    {
      LOG_INFO << conn->name() << " unsubscribes " << topic;
      getTopic(topic).remove(conn);
      ConnnectionSubscription* connSub = 
          boost::any_cast<ConnectionSubscription*>(conn->getMutableContext());
      connSub->erase(topic);
    }
    
    Topic& getTopic(const string& topic)
    {
      std::map<string, Topic>::iterator it = topics_.find(topic);
      if (it == topics_.end())
      {
        it = topics_.insert(make_pair(topic, Topic(topic))).first;
      }
      return it->second;
    }

    EventLoop* loop_;
    TcpServer server_;
    std::map<string, Topic> topics_;
};
}

int main(int argc, char* argv[])
{
  if (argc > 1)
  {
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress listenAddr(port);
    PubSubServer server(&loop, port);
    server.start();
    loop.loop();
  }
  else 
  {
    printf("Usage: %s pubsub_port", argv[0]);
  }
}
