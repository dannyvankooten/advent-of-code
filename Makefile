CFLAGS += -Wextra -Wall

debug: CFLAGS += -DDEBUG -g -DSTEP
debug: all

release: CFLAGS += -Ofast -march=native
release: all

all:
	$(CC) $(CFLAGS) *.c -lm -o aoc2020
