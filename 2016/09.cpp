#include <chrono>
#include <iostream>

using std::string;
using iterator = std::string::iterator;
using size_type = std::string::size_type;

size_t decompress(iterator s, size_type size, bool recurse) {
  size_type length = 0;

  std::string::iterator end = s + static_cast<long>(size);
  while (s != end) {
    if (*s != '(') {
      length += 1;
      s++;
      continue;
    }

    // at (, parse encoding
    size_type pattern_length = static_cast<size_type>(std::stoi(&*(s + 1)));

    while (*(s - 1) != 'x') {
      s++;
    }

    size_type pattern_repeats = static_cast<size_type>(std::stoi(&(*s)));
    while (*(s - 1) != ')') {
      s++;
    }

    // add decompressed size to length
    if (recurse) {
      length += decompress(s, pattern_length, true) * pattern_repeats;
    } else {
      length += pattern_length * pattern_repeats;
    }

    // skip decompressed block
    s += static_cast<long>(pattern_length);
  }

  return length;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  size_type pt1 = 0;
  size_type pt2 = 0;

  string input;
  std::getline(std::cin, input);

  // start at original size
  auto start = input.begin();
  auto length = input.length();
  pt1 = decompress(start, length, false);
  pt2 = decompress(start, length, true);

  std::cout << "--- Day 9: Explosives in Cyberspace ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
