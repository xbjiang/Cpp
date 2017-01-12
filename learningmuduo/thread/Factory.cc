#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <stdio.h>
#include <boost/bind.hpp>
#include <muduo/base/Mutex.h>

using std::string;

class Stock : boost::noncopyable
{
  public:
    Stock(const string& name) : name_(name) 
    {
      printf("Stock[%p] %s\n", this, name.c_str());        
    }

    ~Stock()
    {
      printf("~Stock[%p] %s\n", this, name_.c_str());  
    }

    const string& key() {return name_;}

  private:
    string name_;
};

namespace version1
{
class StockFactory : boost::noncopyable
{
  public:
    boost::shared_ptr<Stock> get(const string& key)
    {
      muduo::MutexLockGuard lock(mutex_);
      boost::shared_ptr<Stock>& pStock = stocks_[key];
      if (!pStock) 
      {
        pStock.reset(new Stock(key));
      }
      return pStock;
    }

  private:
    mutable muduo::MutexLock mutex_;
    std::map< string, boost::shared_ptr<Stock> > stocks_;
};
}

namespace version2
{
class StockFactory : boost::noncopyable
{
  public:
    boost::shared_ptr<Stock> get(const string& key)
    {
      boost::shared_ptr<Stock> pStock;
      muduo::MutexLockGuard lock(mutex_);
      boost::weak_ptr<Stock>& wkStock = stocks_[key];
      pStock = wkStock.lock();
      if (!pStock)
      {
        pStock.reset(new Stock(key));
        wkStock = pStock;
      }
      return pStock;
    }
  private:
    mutable muduo::MutexLock mutex_;
    std::map< string, boost::weak_ptr<Stock> > stocks_;
};
}

namespace version3
{
class StockFactory : boost::noncopyable
{
  public:
    boost::shared_ptr<Stock> get(const string& key)
    {
      boost::shared_ptr<Stock> pStock;
      muduo::MutexLockGuard lock(mutex_);
      boost::weak_ptr<Stock>& wkStock = stocks_[key];
      pStock = wkStock.lock();
      if (!pStock)
      {
        pStock.reset(new Stock(key), boost::bind(&StockFactory::deleteStock, this, _1));
        wkStock = pStock;
      }
      return pStock;
    }
  private:
    mutable muduo::MutexLock mutex_;
    std::map< string, boost::weak_ptr<Stock> > stocks_;
    
    void deleteStock(Stock* stock)
    {
      printf("deleteStock[%p]\n", stock);
      if (stock)
      {
        muduo::MutexLockGuard lock(mutex_);
        stocks_.erase(stock->key());
      }
      delete stock;
    }
};
}

namespace version4
{
class StockFactory : public boost::enable_shared_from_this<StockFactory>,
                     boost::noncopyable
{
  public:
    boost::shared_ptr<Stock> get(const string& key)
    {
      boost::shared_ptr<Stock> pStock;
      muduo::MutexLockGuard lock(mutex_);
      boost::weak_ptr<Stock>& wkStock = stocks_[key];
      pStock = wkStock.lock();
      if (!pStock)
      {
        pStock.reset(new Stock(key), boost::bind(&StockFactory::deleteStock, shared_from_this(), _1));
        wkStock = pStock;
      }
      return pStock;
    }

  private:
    mutable muduo::MutexLock mutex_;
    std::map< string, boost::weak_ptr<Stock> > stocks_;

    void deleteStock(Stock* stock)
    {
      printf("deleteStock[%p]\n", stock);
      if (stock)
      {
        muduo::MutexLockGuard lock(mutex_);
        stocks_.erase(stock->key());
      }
      delete stock;
    }
};
}

class StockFactory : boost::noncopyable, 
                     public boost::enable_shared_from_this<StockFactory>
{
  public:
    boost::shared_ptr<Stock> get(const string& key)
    {
      boost::shared_ptr<Stock> pStock;
      muduo::MutexLockGuard lock(mutex_);
      boost::weak_ptr<Stock>& wkStock = stocks_[key];
      pStock = wkStock.lock();
      if (pStock)
      {
        pStock.reset(new Stock(key), boost::bind(&StockFactory::weakDeleteCallback, 
                                                 boost::weak_ptr<StockFactory>(shared_from_this()),
                                                 _1));
        wkStock = pStock;
      }
      return pStock;
    }
  
  private:
    mutable muduo::MutexLock mutex_;
    std::map< string, boost::weak_ptr<Stock> > stocks_;
    
    static void weakDeleteCallback(const boost::weak_ptr<StockFactory>& wkFactory, 
                                   Stock* stock)
    {
      printf("deleteStock[%p]", stock);
      boost::shared_ptr<StockFactory> factory(wkFactory.lock());
      if (factory)
      {
        factory->removeStock(stock);
      }
      else
      {
        printf("factory is dead.\n");
      }
      delete stock;
    }
    
    void removeStock(Stock* stock)
    {
      if (stock)
      {
        muduo::MutexLockGuard lock(mutex_);
        stocks_.erase(stock->key());
      }
    }
};
int main()
{
 /* version1::StockFactory sf1;
  {
    boost::shared_ptr<Stock> s1 = sf1.get("Stock1");
  }
  printf("checkpoint0\n");
  
  version2::StockFactory sf2;
  {
    boost::shared_ptr<Stock> s2 = sf2.get("Stock2");
  }
  printf("checkpoint\n");
  boost::shared_ptr<Stock> s2 = sf2.get("Stock2");
  printf("checkpoint2\n");*/
  version3::StockFactory sf3;
  {
    boost::shared_ptr<Stock> s3 = sf3.get("Stock3");
  }

  boost::shared_ptr<version4::StockFactory> sf4(new version4::StockFactory);
  {
    boost::shared_ptr<Stock> s4 = sf4->get("Stock4");
  }
  
  boost::shared_ptr<StockFactory> sf5(new StockFactory);
  {
    boost::shared_ptr<Stock> s5 = sf5->get("Stock5");
  }
}
