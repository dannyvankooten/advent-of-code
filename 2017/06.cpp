#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

using std::vector;

inline unsigned long hash_banks(const vector<int>& banks) noexcept {
  unsigned long h = 0;
  for (const auto& b : banks) {
    h <<= 4;
    h += static_cast<unsigned long>(b);
  }

  return h;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned int pt1 = 0;
  unsigned int pt2 = 0;

  std::string input;
  std::vector<int> banks;
  while (std::getline(std::cin, input, '\t')) {
    banks.push_back(std::stoi(input));
  }

  // perform redistribution cycles
  std::unordered_map<unsigned long, unsigned int> seen;
  const auto begin = banks.begin();
  const auto end = banks.end();
  while (true) {
    auto max = std::max_element(begin, end);
    for (auto it = max + 1; *max > 0; it++) {
      if (it == end) {
        it = begin;
      }

      (*it)++;
      (*max)--;
    }

    pt1++;
    const unsigned long h = hash_banks(banks);
    if (seen.contains(h)) [[unlikely]] {
      pt2 = pt1 - seen[h] + 1;
      break;
    }

    seen[h] = pt1;
  }

  std::cout << "--- Day 6: Memory Reallocation ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
