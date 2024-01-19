#include <algorithm>
#include <chrono>
#include <iostream>
#include <string_view>

// see http://ondras.github.io/rot.js/manual/#hex/indexing
inline int distance_from_start(int x, int y) {
  return std::max(abs(x), abs(y) + (abs(x) / 2));
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  // see https://www.redblobgames.com/grids/hexagons/#coordinates-offset
  int x = 0;
  int y = 0;
  int max_distance = 0;

  auto it = input.begin();
  const auto eof = input.end();
  while (it < eof) {
    const auto end = std::find(it, eof, ',');
    const std::string_view op = std::string_view(it, end);

    const bool isodd = x & 1;
    if (op == "n") {
      x += 0;
      y += -1;
    } else if (op == "ne") {
      x += 1;
      y += isodd ? 0 : -1;
    } else if (op == "se") {
      x += 1;
      y += isodd ? 1 : 0;
    } else if (op == "s") {
      x += 0;
      y += 1;
    } else if (op == "sw") {
      x += -1;
      y += isodd ? 1 : 0;
    } else if (op == "nw") {
      x += -1;
      y += isodd ? 0 : -1;
    }

    max_distance = std::max(max_distance, distance_from_start(x, y));
    it = end + 1;
  }

  pt1 = distance_from_start(x, y);
  pt2 = max_distance;

  std::cout << "--- Day 11: Hex Ed ---\n";
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
