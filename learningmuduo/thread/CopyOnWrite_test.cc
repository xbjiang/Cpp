#include <muduo/base/Mutex.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <stdio.h>

class Foo
{
  public:
    void doit() const;
};

typedef std::vector<Foo> FooList;
typedef boost::shared_ptr<FooList> FooListPtr;
FooListPtr g_foos;
muduo::MutexLock mutex;

void post(const Foo& foo)
{
  printf("post\n");
  muduo::MutexLockGuard lock(mutex);
  if (!g_foos.unique())
  {
    g_foos.reset(new FooList(*g_foos));
    printf("Copy the whole list\n");
  }
  assert(g_foos.unique());
  g_foos->push_back(foo);
}

void traverse()
{
  FooListPtr foos;
  {
    muduo::MutexLockGuard lock(mutex);
    foos = g_foos;
    assert(!g_foos.unique());
  }
  
  for(std::vector<Foo>::iterator it = foos->begin();
      it != foos->end(); it++)
  {
    it->doit();
  }
}

void Foo::doit() const
{
  Foo foo;
  post(foo);
}

int main()
{
  g_foos.reset(new FooList);
  Foo f;
  post(f);
  traverse();
}
