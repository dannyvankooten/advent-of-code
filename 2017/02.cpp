#include <algorithm>
#include <cctype>
#include <chrono>
#include <climits>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

int find_evenly_divisble_pair(const vector<int> &numbers) noexcept {
  for (const int a : numbers) {
    for (const int b : numbers) {
      if (a == b) {
        continue;
      }

      if ((a % b) == 0) {
        return a / b;
      }
    }
  }

  return -1;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  string input;
  vector<int> numbers;
  while (std::getline(std::cin, input)) {
    numbers.clear();
    int max = INT_MIN;
    int min = INT_MAX;

    auto it = input.begin();
    while (it != input.end() && isdigit(*it)) {
      int n = std::stoi(&(*it));
      numbers.push_back(n);

      max = std::max(max, n);
      min = std::min(min, n);

      while (isdigit(*it)) {
        it++;
      }

      if (isspace(*it)) {
        it++;
      }
    }

    pt1 += (max - min);
    pt2 += find_evenly_divisble_pair(numbers);
  }

  std::cout << "--- Day 2: Corruption Checksum ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
