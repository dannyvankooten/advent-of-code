CFLAGS+=-Wextra -Wall
BIN=aoc2020

all: release

debug: CFLAGS+=-DDEBUG -g
debug: BIN=aoc2020debug
debug: $(BIN)

profile: CFLAGS+= -pg
profile: BIN=aoc2020debug
profile: $(BIN)

release: CFLAGS+=-DNDEBUG -Ofast -march=native
release: $(BIN)

$(BIN): *.c
	$(CC) $(CFLAGS) $^ -lm -o $(BIN)

.PHONY: clean
clean: 
	rm -f aoc2020debug
	rm -f aoc2020