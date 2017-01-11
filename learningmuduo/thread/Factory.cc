#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <stdio.h>
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

int main()
{
  version1::StockFactory sf1;
  {
    boost::shared_ptr<Stock> s1 = sf1.get("Stock1");
  }
}
