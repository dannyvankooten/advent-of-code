CFLAGS+= -std=c17 -Wall -Wextra -Wpedantic  \
-Wundef -Winline -Wimplicit-fallthrough -Wformat=2  \
-Wconversion -Wvla -Wstringop-overflow=3

ifndef RELEASE
CFLAGS+= -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS  \
	-fsanitize=address -fsanitize=undefined -g        \
	-fstack-protector-strong -fstrict-aliasing
else
CFLAGS+= -O2 -march=native
endif

PROGRAMS= 01 02 03 04 05 06 07 08 09 10 11 12 13 14 16 17 18 19

.PHONY: all clean

all: $(PROGRAMS)

$(PROGRAMS):
	mkdir -p build
	$(CC) $(CFLAGS) $@.c -o build/$@

clean:
	rm -rf $(BINDIR)
