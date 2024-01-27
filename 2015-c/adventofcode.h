#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Allocate SIZE bytes of memory.  */
static inline void *malloc_or_die(unsigned long size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */
static inline void *calloc_or_die(unsigned long n, unsigned long size) {
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    perror("calloc error");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

static size_t read_input_file(char *dest, unsigned int size, char *file) {
  FILE *fp = fopen(file, "r");
  if (!fp) {
    fprintf(stderr, "error reading %s", file);
    exit(EXIT_FAILURE);
    return 0;
  }
  size_t nread = fread(dest, 1, size, fp);
  // skip trailing newlines
  while (dest[nread - 1] == '\n')
    nread--;
  dest[nread] = '\0';
  fclose(fp);
  return nread;
}

// parse_ident parses a string until the next whitespace
static inline const char *parse_ident(char *dst, const char *s) {
  while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')) {
    *dst++ = *s++;
  }
  *dst = '\0';
  return s;
}

// parse_int parses a (signed) integer, with optional plus sign
static inline const char *parse_int(int *dst, const char *s) {
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

// parse_int parses a (signed) integer, with optional plus sign
static inline const char *parse_uint16(int16_t *dst, const char *s) {
  return parse_int((int *)dst, s);
}

// parse_uint8 parses a single unsigned byte value
static inline const char *parse_uint8(uint8_t *dst, char *s) {
  return parse_int((int *)dst, s);
}

// skip advanced the pointer s as long as it matches the corresponding chars
// from expects
static inline const char *skip(const char *expects, const char *s) {
  while (*s == *expects && *expects != 0x0) {
    s++;
    expects++;
  };

  if (*expects != '\0') {
    printf("Parse error. Expected '%c', got '%c'.\n%s\n", *expects, *s, s);
    exit(EXIT_FAILURE);
  }
  return s;
}

static inline const char *skip_optional(char c, const char *s) {
  if (*s == c) {
    s++;
  }

  return s;
}

static inline const char *skip_until(char c, const char *s) {
  while (*s != c && *s != 0x0) {
    s++;
  }

  return s;
}

static inline const char *skip_until_digit(const char *s) {
  while ((*s < '0' || *s > '9') && *s != 0x0) {
    s++;
  }

  return s;
}

static clock_t clock_time(void) { return clock(); }

static double clock_time_since(clock_t start_t) {
  clock_t end_t = clock();
  return (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
}
