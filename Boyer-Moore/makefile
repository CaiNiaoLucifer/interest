SRC = ./
CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	       -Wconversion -Wno-unused-parameter \
           -Wold-style-cast -Woverloaded-virtual \
           -Wpointer-arith -Wshadow -Wwrite-strings \
           -march=native -rdynamic \

all: StringMatch

clean:
	rm -f *.o core

StringMatch: $(SRC)*.cpp
	g++ $(CXXFLAGS) -o $@ $^

.PHONY: all clean