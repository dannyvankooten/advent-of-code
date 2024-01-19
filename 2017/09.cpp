#include <chrono>
#include <iostream>

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  auto it = input.begin();
  auto end = input.end();
  int group_depth = 1;
  bool in_garbage = false;

  while (it != end) {
    if (in_garbage) {
      switch (*it) {
      case '!':
        it++;
        break;

      case '>':
        in_garbage = false;
        break;

      default:
        pt2 += 1;
        break;
      }
    } else {
      switch (*it) {
      case '{':
        group_depth += 1;
        break;

      case '}':
        group_depth -= 1;
        pt1 += group_depth;
        break;

      case '<':
        in_garbage = true;
        break;
      }
    }

    it++;
  }

  std::cout << "--- Day 9: Stream Processing ---\n";
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
