CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I$(wildcard ~)/build/release-install/include/

LDFLAGS=-L$(wildcard ~)/build/release-install/lib/

echo: echo.cc echo.h main.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lmuduo_base -lmuduo_net -lpthread

clean:
	rm -f echo
