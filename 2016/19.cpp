#include <bit>
#include <chrono>
#include <iostream>

// Josephus Problem
// https://www.youtube.com/watch?v=uCsD3ZGzMgE
// a = largest power of 2 < n
// l = n - a
// Winner(n) = (2*l)+1
// Implemented here by shifting everything to the left
//    Setting the most significant bit to zero
//    And adding 1
unsigned int pt1(unsigned int n) {
  return ((n << 1) + 1) & ~((unsigned int)1 << (32 - std::countl_zero(n)));
}

unsigned int pt2(unsigned int n) {
  unsigned int w = 1;

  for (unsigned int i = 1; i < n; i++) {
    w = w % i + 1;
    if (w > (i + 1) / 2) {
      w++;
    }
  }
  return w;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  std::string input;
  std::getline(std::cin, input);

  unsigned int number_of_elves = (unsigned int)std::stoi(input);

  unsigned a1 = pt1(number_of_elves);
  unsigned a2 = pt2(number_of_elves);

  std::cout << "--- Day 19: An Elephant Named Joseph ---\n";
  std::cout << "Part 1: " << a1 << "\n";
  std::cout << "Part 2: " << a2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
