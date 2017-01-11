CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

TESTS= Counter \
       Observer

all: $(TESTS)

$(TESTS): 
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Counter: Counter.cc
Counter: LDFLAGS += -lmuduo_base -lpthread

Observer: Observer.cc
Observer: CXXFLAGS += --std=c++0x

clean:
	rm -f $(TESTS)
