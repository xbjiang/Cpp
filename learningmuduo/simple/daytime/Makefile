CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+= -O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

daytime: daytime.cc daytime.h main.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lmuduo_base -lmuduo_net -lpthread

clean:
	rm -f daytime 
