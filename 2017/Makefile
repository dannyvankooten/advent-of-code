CXXFLAGS+=-g -std=c++20 -Wall -Wextra -Wpedantic -Wundef -Winline -Wimplicit-fallthrough -Wformat=2 -Wconversion -O2
SHELL=bash

# Only consider C++ for implicit Make targets
.SUFFIXES:
.SUFFIXES: .cpp
.PHONY: debug clean all

debug: CXXFLAGS += -fsanitize=undefined -fsanitize=address
debug: all

all: 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

# Some days needs to be linked with OpenSSL (-lcrypto) for MD5 hashing
#05 14 17: LDLIBS += -lcrypto

clean:
	rm -f {01..25}
