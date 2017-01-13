#include <muduo/base/Mutex.h>

class Request
{
  public:
    void process() //__attribute__ ((noinline))
    {
      muduo::MutexLockGuard lock(mutex_);
      print();
    }
   
    void print() //__attribute__ ((noinline))
    {
      muduo::MutexLockGuard lock(mutex_);
    }

  private:
    muduo::MutexLock mutex_;
};

int main()
{
  Request req;
  req.process();
}
