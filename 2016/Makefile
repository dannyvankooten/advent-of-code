CXXFLAGS+=-g -std=c++20 -O2 -Wundef -Winline  -Wall -Wextra -Wpedantic -Wimplicit-fallthrough -Wformat=2 -Wconversion
SHELL=bash

# ignore .C files for implicit targets
.SUFFIXES:
.SUFFIXES: .cpp

.PHONY: all
all:
	make {01..25}

# Day 5 needs -lcrypto
05: 05.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@

# Day 14 needs -lcrypto
14: 14.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@

.PHONY: clean
clean:
	rm -f {01..25}
