CXXFLAGS+= -std=c++20 -Wall -Wextra -Wpedantic 	\
-Wundef -Winline -Wimplicit-fallthrough -Wformat=2 			\
-Wconversion
PROGRAMS=01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

ifndef RELEASE
CXXFLAGS+= -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS 	\
	-fsanitize=address -fsanitize=undefined -g 						\
	-fstack-protector-strong -D_GLIBCXX_DEBUG
else
CXXFLAGS+= -O2 -march=native -mtune=native
endif

.PHONY: all clean

all: $(PROGRAMS)

# These days need to be linked against -lcrypto
05 14 17: LDLIBS += -lcrypto

$(PROGRAMS):

clean:
	rm -f $(PROGRAMS)
