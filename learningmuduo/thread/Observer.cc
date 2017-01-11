#include <vector>
#include <algorithm>
#include <stdio.h>

class Observable;

class Observer
{
  public:
    virtual ~Observer();
    virtual void update() = 0;

    void observe(Observable* s);

  private:
    Observable* subject_;
};

class Observable
{
  public:
    void register_(Observer* x);
    void unregister(Observer* x);
    
    void notifyObservers();

  private:
    std::vector<Observer*> observers_;
};

Observer::~Observer()
{
  subject_->unregister(this);
}

void Observer::observe(Observable* s)
{
  s->register_(this);
  subject_ = s;
}

void Observable::register_(Observer* x)
{
  observers_.push_back(x);
}

void Observable::unregister(Observer* x)
{
  std::vector<Observer*>::iterator it = std::find(observers_.begin(), observers_.end(), x);
  if (it != observers_.end())
  {
    std::swap(*it, observers_.back());
    observers_.pop_back();
  }
}

void Observable::notifyObservers()
{
  for (size_t i = 0; i < observers_.size(); ++i)
  {
    Observer* x = observers_[i];
    if (x) {
      x->update();
    }
  }
}

class Foo : public Observer
{
  virtual void update() override
  {
    printf("Foo::update() %p\n", this);
  }
};

int main()
{
  Foo* p = new Foo;
  Observable subject;
  p->observe(&subject);
  subject.notifyObservers();
  delete p;
  subject.notifyObservers();
}
