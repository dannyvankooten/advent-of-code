CFLAGS+= -std=c11 -Wall -Wextra -Wpedantic 	\
-Wundef -Winline -Wimplicit-fallthrough -Wformat=2 -Wvla
BIN=aoc2020

all: release

debug: CFLAGS+=-DDEBUG -g -D_FORTIFY_SOURCE=2 	\
-D_GLIBCXX_ASSERTIONS -fsanitize=address 				\
-fsanitize=undefined -fstack-protector-strong
debug: BIN=aoc2020debug
debug: $(BIN)

profile: CFLAGS+= -pg
profile: BIN=aoc2020debug
profile: $(BIN)

release: CFLAGS+=-O2 -march=native -mtune=native -flto
release: $(BIN)

$(BIN): *.c
	$(CC) $(CFLAGS) $^ -lm -o $(BIN)

.PHONY: clean
clean:
	rm -f aoc2020debug
	rm -f aoc2020
