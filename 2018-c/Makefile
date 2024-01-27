CFLAGS+= -std=c11 -Wall -Wextra -Wpedantic  \
-Wundef -Winline -Wimplicit-fallthrough -Wformat=2  \
-Wconversion
PROGRAMS= 01 02 03 04 05 06 07 08 09

ifndef RELEASE
CFLAGS+= -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS  \
	-fsanitize=address -fsanitize=undefined -g        \
	-fstack-protector-strong
else
CFLAGS+= -O2 -march=native
endif

.PHONY: all clean

all: $(PROGRAMS)

$(PROGRAMS): build
	$(CC) $(CFLAGS) $@.c -o build/$@

build:
		mkdir -p build

clean:
	rm -rf build
