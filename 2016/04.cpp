#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;

  while (std::getline(std::cin, input)) {

    // TODO: We could use a plain array here
    std::unordered_map<char, int> m = {};
    for (char c = 'a'; c <= 'z'; c++) {
      m[c] = 0;
    }

    std::string name;

    // count chars
    int i = 0;
    for (; (input[i] >= 'a' && input[i] <= 'z') || input[i] == '-'; i++) {
      // don't count dashes
      if (input[i] == '-') {
        continue;
      }

      m[input[i]] += 1;
      name.push_back(input[i]);
    }

    // parse sector id
    int sector_id = std::stoi(&input[i]);

    // skip forward to checksum opening bracket
    // TODO: strchr?
    while (input[i - 1] != '[') {
      i++;
    }

    // at checksum
    // each char should have higher count than char that follows
    // or same count but be alphetically lower
    bool valid = true;
    for (int ci = 0; ci < 5; ci++) {
      char mc = 0;
      int mv = 0;
      for (const auto& [key, value] : m) {

        if (value > mv || (value == mv && key < mc)) {
          mv = value;
          mc = key;
        }
      }
      m[mc] = 0;

      if (input[i + ci] != mc) {
        valid = false;
        break;
      }
    }

    if (valid) {
      pt1 += sector_id;
    }

    // rotate name & check for north pole room
    for (char& c : name) {
      c = ((c - 'a' + sector_id) % 26) + 'a';
    }
    if (pt2 == 0 && name.compare("northpoleobjectstorage") == 0) {
      pt2 = sector_id;
    }
  }

  std::cout << "--- Day 4: Security Through Obscurity ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
