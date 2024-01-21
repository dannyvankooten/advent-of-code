#include <cctype>
#include <chrono>
#include <cstdint>
#include <iostream>

unsigned int solve_pt1(uint64_t a, uint64_t b) {
  unsigned int count = 0;
  for (int i = 0; i < 40e6; i++) {
    a = (a * 16807) % 2147483647;
    b = (b * 48271) % 2147483647;
    count += !((a ^ b) & 0xFFFF);
  }
  return count;
}

unsigned int solve_pt2(uint64_t a, uint64_t b) {
  unsigned int count = 0;
  for (int i = 0; i < 5e6; i++) {
    do {
      a = (a * 16807) % 2147483647;
    } while ((a & 3));

    do {
      b = (b * 48271) % 2147483647;
    } while ((b & 7));

    count += !((a ^ b) & 0xFFFF);
  }
  return count;
}

struct Input {
  uint64_t a;
  uint64_t b;
};

Input parse_input() {
  std::string input;
  uint64_t a = 0;
  uint64_t b = 0;

  while (std::getline(std::cin, input)) {
    auto it = input.end() - 1;
    while (std::isdigit(*it)) {
      it--;
    }

    if (a == 0) {
      a = static_cast<uint64_t>(std::stoi(&(*it)));
    } else {
      b = static_cast<uint64_t>(std::stoi(&(*it)));
    }
  }
  return {a, b};
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned int pt1 = 0;
  unsigned int pt2 = 0;

  const auto [a, b] = parse_input();
  pt1 = solve_pt1(a, b);
  pt2 = solve_pt2(a, b);

  std::cout << "--- Day 15: Dueling Generators ---\n";
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
