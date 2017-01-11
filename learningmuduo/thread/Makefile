CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

Counter: Counter.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lmuduo_base -lpthread

clean:
	rm -f Counter
