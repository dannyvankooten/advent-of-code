#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

static inline void *malloc_or_die(unsigned long size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

static unsigned int read_input_file(char *dest, unsigned int size, char *file) {
  FILE *fp = fopen(file, "r");
  if (!fp) {
    fprintf(stderr, "error reading %s", file);
    exit(EXIT_FAILURE);
    return 0;
  }
  unsigned int nread = fread(dest, 1, size, fp);
  // skip trailing newlines
  while (dest[nread-1] == '\n') nread--;
  dest[nread] = '\0';
  fclose(fp);
  return nread;
}

// parse_ident parses a string consisting of only lowercase a-z chars
static inline char *parse_ident(char *dst, char *s) {
  while (*s >= 'a' && *s <= 'z') {
    *dst++ = *s++;
  }
  *dst = '\0';
  return s;
}

// parse_int parses a (signed) integer, with optional plus sign
static inline char *parse_int(int *dst, char *s) {
  int n = 0;
  int mod = 1;
  if (*s == '-') {
    mod = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  while (*s >= '0' && *s <= '9') {
    n = (n * 10) + (*s - '0');
    s++;
  }
  *dst = n * mod;
  return s;
}


// parse_uint8 parses a single unsigned byte value
static inline char *parse_uint8(uint8_t *dst, char *s) {
  uint8_t n = 0;

  while (*s >= '0' && *s <= '9') {
    n = (n * 10) + (*s - '0');
    s++;
  }
  *dst = n;
  return s;
}

// skip advanced the pointer s as long as it matches the corresponding chars from expects
static inline char *skip(char *expects, char *s) {
  while (*s == *expects) {
    s++;
    expects++;
  };

  if (*expects != '\0') {
    printf("Parse error. Expected '%c', got '%c'.\n%s\n", *expects, *s, s);
    exit(EXIT_FAILURE);
  }
  return s;
}

static inline char *skip_optional(char c, char *s) {
  while (*s == c) {
    s++;
  }

  return s;
}

static inline char *skip_until(char c, char *s) {
  while (*s != c && *s != 0x0) {
    s++;
  }

  return s;
}

static clock_t timer_start() {
  return clock();
}

static double timer_stop(clock_t start_t) {
  clock_t end_t = clock();
  return (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
}
