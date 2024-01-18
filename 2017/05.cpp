#include <chrono>
#include <iostream>
#include <vector>

using std::vector;

int solve_pt1(vector<int> jumps) {
  int steps = 0;
  unsigned int ji = 0;

  while (ji < jumps.size()) {
    int j = jumps[ji];
    jumps[ji] += 1;
    ji = ji + static_cast<unsigned int>(j);
    steps += 1;
  }

  return steps;
}

int solve_pt2(vector<int> jumps) {
  int steps = 0;
  unsigned int ji = 0;
  while (ji < jumps.size()) {
    int j = jumps[ji];
    jumps[ji] = j >= 3 ? j - 1 : j + 1;
    ji = ji + static_cast<unsigned int>(j);
    steps += 1;
  }
  return steps;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::vector<int> jumps;
  while (std::getline(std::cin, input)) {
    jumps.push_back(std::stoi(input));
  }

  pt1 = solve_pt1(jumps);
  pt2 = solve_pt2(jumps);

  std::cout << "--- Day 5: A Maze of Twisty Trampolines, All Alike ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
