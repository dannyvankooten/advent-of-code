CXXFLAGS+=-g -std=c++20 -O2 -Wundef -Winline  -Wall -Wextra -Wpedantic -Wimplicit-fallthrough -Wformat=2 -Wconversion
SHELL=bash

# Only consider C++ for implicit Make targets
.SUFFIXES:
.SUFFIXES: .cpp .cc

.PHONY: all
all:
	make {01..25}

# Some dats needs to be linked with OpenSSL (-lcrypto) for MD5
05: 05.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@
14: 14.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@
17: 17.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@

.PHONY: clean
clean:
	rm -f {01..25}
