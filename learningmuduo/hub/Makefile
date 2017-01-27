CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+= -O2
CXXFLAGS+= -I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/
LDFLAGS+= -lmuduo_base -lmuduo_net -lpthread

TESTS= hub pub sub

all: $(TESTS)
 
$(TESTS): codec.h codec.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

hub: hub.cc

pub: pub.cc pubsub.h pubsub.cc

sub: sub.cc pubsub.h pubsub.cc

clean:
	rm -f $(TESTS)
