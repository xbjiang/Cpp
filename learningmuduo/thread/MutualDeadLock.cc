#include <set>
#include <stdio.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

class Request;

class Inventory
{
  public:
    void add(Request* request)
    {
      muduo::MutexLockGuard lock(mutex_);
      requests_.insert(request);
    }

    void remove(Request* request) __attribute__ ((noinline))
    {
      muduo::MutexLockGuard lock(mutex_);
      requests_.erase(request);
    }

    void printAll() const;

  private:
    mutable muduo::MutexLock mutex_;
    std::set<Request*> requests_;
};

Inventory g_inventory;

class Request
{
  public:
    void process()
    {
      muduo::MutexLockGuard lock(mutex_);
      g_inventory.add(this);
    }

    ~Request() __attribute__ ((noinline))
    {
      muduo::MutexLockGuard lock(mutex_);
      sleep(1);
      g_inventory.remove(this);
    }

    void print() const __attribute__ ((noinline))
    {
      muduo::MutexLockGuard lock(mutex_);
    }
  private:
    mutable muduo::MutexLock mutex_;
};

void Inventory::printAll() const
{
  muduo::MutexLockGuard lock(mutex_);
  for (std::set<Request*>::const_iterator it = requests_.begin();
       it != requests_.end(); it++)
  {
    (*it)->print();
  }
  printf("Inventory::printAll() unlocked\n");
}

void threadFunc()
{
  Request* req = new Request;
  req->process();
  delete req;
}

int main()
{
  muduo::Thread thread(threadFunc);
  thread.start();
  usleep(500 * 1000);
  g_inventory.printAll();
  thread.join();
}


