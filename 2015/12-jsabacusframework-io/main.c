#include "../adventofcode.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int pt1(const char *input) {
  int sum = 0;
  const char *s = input;
  int n;
  while (*s != '\0') {
    // since there are no strings
    // simply parse everything that looks like a number
    if (*s == '-' || (*s >= '0' && *s <= '9')) {
      s = parse_int(&n, s);
      sum += n;
    }

    // skip everything else
    s++;
  }
  return sum;
}

int pt2(const char *input) {
  const char *s = input;
  int n;

  const int MAX_DEPTH = 64;
  int objsums[MAX_DEPTH];
  memset(objsums, 0, MAX_DEPTH * sizeof(int));
  objsums[0] = 0;
  int objs = 0;
  int red = 0;
  while (*s != '\0') {

    // only add to sum if not in "red" object
    if ((*s == '-' || (*s >= '0' && *s <= '9'))) {
      s = parse_int(&n, s);
      objsums[objs] += n;
    }

    switch (*s) {
    case '{':
      objs++;
      objsums[objs] = 0;
      break;

    case '}':
      // check if we broke out of red objecty
      if (red == 0 && objs > 0) {
        objsums[objs - 1] += objsums[objs];
      } else if (objs == red) {
        red = 0;
      }

      objs--;
      break;

    case ':':
      // if property has value of "red"
      // mark current object as red
      // and substract everything added to sum within this object
      // or any of its children
      if (red == 0 && strncmp(s, ":\"red\"", 6) == 0) {
        red = objs;
      }
      break;
    }

    s++;
  }

  return objsums[0];
}

int main() {
  clock_t tstart = clock_time();
  char input[32 * 1024] = "";
  read_input_file(input, 1024 * 32, "input.txt");

  int a1 = 0, a2 = 0;
  a1 = pt1(input);
  a2 = pt2(input);

  // aoc samples
  assert(pt2("[1,2,3]") == 6);
  assert(pt2("[1,{\"c\":\"red\",\"b\":2},3]") == 4);
  assert(pt2("{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}") == 0);
  assert(pt2("[1,\"red\",5]") == 6);
  assert(pt2("[1,{\"a\":10,\"c\":\"red\",\"b\":2},3]") == 4);
  assert(pt2("[1,{\"c\":\"red\",\"b\":2},3]") == 4);
  assert(pt2("[{\"a\":5,\"color\":\"red\", \"b\": 4}]") == 0);
  assert(pt2("[{\"a\":5,\"b\":\"red\",\"c\":{\"a\": 5}}]") == 0);
  assert(pt2("[{\"a\":5},{\"a\":5,\"b\":\"red\",\"c\":{\"a\": 5}}]") == 5);
  assert(pt2("{\"a\":{\"a\":[1,2,3]}, \"b\":\"red\"}]") == 0);

  printf("--- Day 12: JSAbacusFramework.io ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(tstart));
  return 0;
}