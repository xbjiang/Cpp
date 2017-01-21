CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

TESTS= chat_server \
       chat_client \
       chat_server_threaded_highperformance

all: $(TESTS)

$(TESTS):
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lmuduo_base -lmuduo_net -lpthread
 
chat_server: server.cc codec.h

chat_client: client.cc codec.h 

chat_server_threaded_highperformance: server_threaded_highperformance.cc codec.h

clean:
	rm -f $(TESTS)
