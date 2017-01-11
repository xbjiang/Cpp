#include <muduo/base/Mutex.h>
using muduo::MutexLock;
using muduo::MutexLockGuard;

class Counter : boost::noncopyable
{
  public:
    Counter() : value_(0) {}
    Counter& operator=(const Counter& rhs);
    int64_t value() const;
    int64_t getAndIncrease();
    
    friend void swap(Counter& a, Counter& b);
    
  private:
    mutable MutexLock mutex_;
    int64_t value_;
};

Counter& Counter:: operator=(const Counter& rhs)
{
  if (this == &rhs)
    return *this;

  MutexLockGuard myLock(mutex_);
  MutexLockGuard itsLock(mutex_);
  value_ = rhs.value_;
  return *this;
}

int64_t Counter::value() const
{
  MutexLockGuard lock(mutex_);
  return value_;
}

int64_t Counter::getAndIncrease()
{
  MutexLockGuard lock(mutex_);
  value_++;
  return value_;
}

void swap(Counter& a, Counter& b)
{
  MutexLockGuard aLock(a.mutex_);
  MutexLockGuard bLock(b.mutex_);
  a.value_ = b.value_;
}

int main(int argc, char* argv[])
{
  Counter c;
  c.getAndIncrease();
}
