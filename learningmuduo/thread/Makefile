CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

TESTS= Counter \
       Observer \
       Factory \
       SelfDeadLock \
       CopyOnWrite_test \
       MutualDeadLock  \
       RequestInventory_test \
       RequestInventory_test2

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

CopyOnWrite_test: CopyOnWrite_test.cc
CopyOnWrite_test: LDFLAGS += -lmuduo_base -lpthread

MutualDeadLock: MutualDeadLock.cc
MutualDeadLock: LDFLAGS += -lmuduo_base -lpthread

RequestInventory_test: RequestInventory_test.cc
RequestInventory_test: LDFLAGS += -lmuduo_base -lpthread

RequestInventory_test2: RequestInventory_test2.cc
RequestInventory_test2: LDFLAGS += -lmuduo_base -lpthread

clean:
	rm -f $(TESTS)
