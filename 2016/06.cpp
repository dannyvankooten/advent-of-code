#include <chrono>
#include <iostream>
#include <unordered_map>

using std::string;

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  std::unordered_map<char, int> maps[8];
  for (unsigned int col = 0; col < 8; col++) {
    for (char ch = 'a'; ch <= 'z'; ch++) {
      maps[col][ch] = 0;
    }
  }
  string input;
  while (std::getline(std::cin, input)) {
    for (unsigned int col = 0; col < 8; col++) {
      maps[col][input[col]]++;
    }
  }

  string pt1(8, 'a');
  string pt2(8, 'a');

  for (unsigned int col = 0; col < 8; col++) {
    for (char ch = 'b'; ch <= 'z'; ch++) {
      if (maps[col][ch] > maps[col][pt1[col]]) {
        pt1[col] = ch;
      }

      if (maps[col][ch] < maps[col][pt2[col]]) {
        pt2[col] = ch;
      }
    }
  }

  std::cout << "--- Day 6: Signals and Noise ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
