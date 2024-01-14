#include <bitset>
#include <chrono>
#include <iostream>

int count_traps(const std::string& input, int nrows) {
  int trapcount = 0;

  // store row in bitset that's larger than row width
  // so we don't have to do any bounds checking inside the loop
  std::bitset<102> prev_row;
  std::bitset<102> cur_row;

  prev_row[0] = false;
  prev_row[101] = false;

  for (int c = 0; c < 100; c++) {
    prev_row[c + 1] = input[c] == '^' ? true : false;
    if (input[c] == '^') {
      trapcount += 1;
    }
  }

  int tiles;
  for (int r = 1; r < nrows; r++) {
    for (int c = 1; c <= 100; c++) {
      tiles = (prev_row[c - 1] << 2) + (prev_row[c] << 1) + prev_row[c + 1];
      cur_row[c] = (tiles == 0b110 || tiles == 0b011 || tiles == 0b100 ||
                    tiles == 0b001);
    }

    trapcount += static_cast<int>(cur_row.count());
    prev_row = cur_row;
  }

  return (nrows * 100) - trapcount;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  pt1 = count_traps(input, 40);
  pt2 = count_traps(input, 400000);

  std::cout << "--- Day 18: Like a Rogue ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
