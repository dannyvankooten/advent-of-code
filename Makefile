CFLAGS+=-Wextra -Wall
BIN=build/aoc2020

all: release

debug: CFLAGS+=-DDEBUG -g
debug: BIN=build/aoc2020debug
debug: $(BIN)

release: CFLAGS+=-DNDEBUG -Ofast -march=native
release: $(BIN)

$(BIN): *.c
	mkdir -p build
	$(CC) $(CFLAGS) $^ -lm -o $(BIN)

.PHONY: clean
clean: 
	rm build/*