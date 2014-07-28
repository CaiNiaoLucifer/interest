SRC = ./
CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	       -Wconversion -Wno-unused-parameter \
           -Wold-style-cast -Woverloaded-virtual \
           -Wpointer-arith -Wshadow -Wwrite-strings \
           -march=native -rdynamic -std=c++11 \

all: MapReduce

clean:
	rm -f *.o core

MapReduce: CLSliceMapper.cpp SLConfigParser.cpp MapReduce.cpp PrintError.cpp
	g++ $(CXXFLAGS) -o $@ $^

.PHONY: all clean