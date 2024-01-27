CXXFLAGS+= -std=c++20 -Wall -Wextra -Werror -Wpedantic 	\
-Wundef -Winline -Wimplicit-fallthrough -Wformat=2 			\
-Wconversion
PROGRAMS= 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

ifndef RELEASE
CXXFLAGS+= -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS 	\
	-fsanitize=address -fsanitize=undefined -g 						\
	-fstack-protector-strong
else
CXXFLAGS+= -O2 -march=native -mtune=native
endif

.PHONY: all clean

all: $(PROGRAMS)

$(PROGRAMS):

clean:
	rm -f $(PROGRAMS)

