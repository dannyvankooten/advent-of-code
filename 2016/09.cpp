#include <chrono>
#include <iostream>

size_t decompress(std::string::iterator s, size_t size, bool recurse) {
  size_t length = 0;

  auto end = s + size;
  while (s != end) {
    if (*s != '(') {
      length += 1;
      s++;
      continue;
    }

    // at (, parse encoding
    int pattern_length = std::stoi(&*(s + 1));

    while (*(s - 1) != 'x') {
      s++;
    }

    int pattern_repeats = std::stoi(&(*s));
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
    s += pattern_length;
  }

  return length;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  size_t pt1 = 0;
  size_t pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  // start at original size
  pt1 = decompress(input.begin(), input.length(), false);
  pt2 = decompress(input.begin(), input.length(), true);

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
