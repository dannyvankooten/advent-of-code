#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using std::any_of;
using std::string;
using std::vector;

struct Scanner {
  int depth;
  int range;
  int cycle;
};

vector<Scanner> parse_input() {
  vector<Scanner> scanners;
  string input;
  while (std::getline(std::cin, input)) {
    // parse depth
    const int depth = std::stoi(input);
    const auto it = std::find(input.begin(), input.end(), ':');

    // parse range
    const int range = std::stoi(&(*(it + 2)));
    scanners.push_back({depth, range, (range - 1) * 2});
  }
  return scanners;
}

int solve_pt1(const vector<Scanner> &scanners) {
  int severity = 0;

  for (const Scanner &s : scanners) {
    if ((s.depth % s.cycle) == 0) {
      severity += (s.depth * s.range);
    }
  }
  return severity;
}

int solve_pt2(const vector<Scanner> &scanners) {
  int delay = 0;

  while (any_of(scanners.begin(), scanners.end(), [delay](const Scanner &s) {
    return ((delay + s.depth) % s.cycle) == 0;
  })) {
    ++delay;
  }

  return delay;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  // map of scanners: key=depth, value=range
  vector<Scanner> scanners = parse_input();

  pt1 = solve_pt1(scanners);
  pt2 = solve_pt2(scanners);

  std::cout << "--- Day 13: Packet Scanners ---\n";
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
