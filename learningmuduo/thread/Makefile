CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

TESTS= Counter \
       Observer \
       Factory \
       SelfDeadLock

all: $(TESTS)

$(TESTS): 
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Counter: Counter.cc
Counter: LDFLAGS += -lmuduo_base -lpthread

Observer: Observer.cc
Observer: CXXFLAGS += --std=c++0x

Factory: Factory.cc
Factory: LDFLAGS += -lmuduo_base -lpthread

SelfDeadLock: SelfDeadLock.cc
SelfDeadLock: LDFLAGS += -lmuduo_base -lpthread

clean:
	rm -f $(TESTS)
