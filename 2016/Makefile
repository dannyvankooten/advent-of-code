CXXFLAGS+=-g -std=c++17 -O2 -Wall -Wextra -Wpedantic -Wundef -Winline -Wimplicit-fallthrough -Wformat=2 -Wconversion
SHELL=bash

# Only consider C++ for implicit Make targets
.SUFFIXES:
.SUFFIXES: .cpp .cc

.PHONY: all
all:
	make {01..25}

# Some days needs to be linked with OpenSSL (-lcrypto) for MD5 hashing
05: 05.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -lcrypto -o $@
14: 14.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -lcrypto -o $@
17: 17.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -lcrypto -o $@

.PHONY: clean
clean:
	rm -f {01..25}
