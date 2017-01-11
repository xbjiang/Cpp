CXXFLAGS= -g -Wall -rdynamic -march=native
CXXFLAGS+=-O2
CXXFLAGS+=-I/home/jiangxiaobo/build/release-install/include/

LDFLAGS=-L/home/jiangxiaobo/build/release-install/lib/

Counter: Counter.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lmuduo_base -lpthread

clean:
	rm -f Counter
