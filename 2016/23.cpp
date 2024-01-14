#include <cctype>
#include <chrono>
#include <iostream>

using std::string;

unsigned int factorial(unsigned int n) {
  int p = n--;

  while (n > 0) {
    p *= n--;
  }
  return p;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  string input;
  int c = 1;

  // parse input to find our constant
  // taken from the JNZ or CPY instruction
  // with first parameter larger than 10
  while (getline(std::cin, input)) {
    if (input[0] != 'c' && input[0] != 'j') {
      continue;
    }

    if (!std::isdigit(input[4])) {
      continue;
    }

    int v = std::stoi(&input[4]);
    if (v > 10) {
      c *= v;
    }
  }

  pt1 = factorial(7) + c;
  pt2 = factorial(12) + c;

  std::cout << "--- Day 23: Safe Cracking ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
