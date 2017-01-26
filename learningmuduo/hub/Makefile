CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+= -O2
CXXFLAGS+= -I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/
LDFLAGS+= -lmuduo_base -lmuduo_net -lpthread

Objects= codec.o pubsub.o hub.o pub.o sub.o
TESTS= hub pub sub

all: $(TESTS)
 
$(TESTS): 
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

hub: hub.o codec.o

pub: pub.o pubsub.o codec.o

sub: sub.o pubsub.o codec.o

codec.o: codec.h codec.cc
	g++ $(CXXFLAGS) -c codec.cc $(LDFLAGS) 

pubsub.o: pubsub.h pubsub.cc
	g++ $(CXXFLAGS) -c pubsub.cc $(LDFLAGS) 

hub.o: hub.cc
	g++ $(CXXFLAGS) -c hub.cc $(LDFLAGS) 

pub.o: pub.cc
	g++ $(CXXFLAGS) -c pub.cc $(LDFLAGS) 

sub.o: sub.cc
	g++ $(CXXFLAGS) -c sub.cc $(LDFLAGS) 

clean:
	rm -f $(TESTS) $(Objects)
