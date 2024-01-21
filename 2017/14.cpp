#include <bitset>
#include <charconv>
#include <chrono>
#include <iostream>
#include <vector>

using std::bitset;
using std::string;
using std::vector;

void reverse(vector<int> &list, unsigned int start, unsigned int length) {
  for (unsigned int i = 0; i < length / 2; i++) {
    vector<int>::size_type idx_a = (start + i) % (list.size());
    vector<int>::size_type idx_b = (start + length - i - 1) % (list.size());
    std::swap(list[idx_a], list[idx_b]);
  }
}

constexpr vector<int> knot_list() {
  vector<int> list(256);
  for (int i = 0; i < 256; i++) {
    list[i] = i;
  }
  return list;
}

bitset<128> create_row(const vector<int> &lengths, unsigned int row_idx) {
  vector<int> list = knot_list();

  int pos = 0;
  int skip = 0;
  const auto row_lengths = std::to_string(row_idx);

  for (unsigned int i = 0; i < 64; i++) {
    for (const int length : lengths) {
      reverse(list, pos, length);
      pos += length + skip;
      skip += 1;
    }

    for (const int length : row_lengths) {
      reverse(list, pos, length);
      pos += length + skip;
      skip += 1;
    }

    for (const int length : {17, 31, 73, 47, 23}) {
      reverse(list, pos, length);
      pos += length + skip;
      skip += 1;
    }
  }

  // we now have a "sparse hash" of 256 elements
  // turn it into a "dense hash" of 16 elements
  int dense_hash[16] = {0};
  for (unsigned int i = 0; i < 16; i++) {
    for (unsigned int j = i * 16, jend = (i + 1) * 16; j < jend; j++) {
      dense_hash[i] ^= list[j];
    }
  }

  // now, dense hash to bitset
  std::bitset<128> bits;
  for (unsigned int i = 0; i < 16; i++) {
    for (unsigned int j = 0; j < 8; j++) {
      bits[(i * 8) + j] = (dense_hash[i] >> (7 - j)) & 1;
    }
  }

  return bits;
}

vector<int> parse_input() {
  vector<int> lengths;
  std::string input;
  std::getline(std::cin, input);
  for (const char ch : input) {
    lengths.push_back(ch);
  }
  lengths.push_back('-');
  return lengths;
}

void mark_group(const vector<bitset<128>> &grid, bitset<128 * 128> &seen,
                const unsigned int row, const unsigned int col) {
  if (row >= 128 || col >= 128) {
    return;
  }

  if (grid[row][col] != true) {
    return;
  }

  if (seen[(row * 128) + col]) {
    return;
  }

  seen[(row * 128) + col] = true;
  mark_group(grid, seen, row + 1, col);
  mark_group(grid, seen, row - 1, col);
  mark_group(grid, seen, row, col + 1);
  mark_group(grid, seen, row, col - 1);
}

int solve_pt2(const vector<bitset<128>> &grid) {
  int regions = 0;
  bitset<128 * 128> seen;

  for (unsigned int row = 0; row < 128; row++) {
    for (unsigned int col = 0; col < 128; col++) {
      if (seen[(row * 128) + col]) {
        continue;
      }

      if (grid[row][col] == false) {
        continue;
      }

      regions++;
      mark_group(grid, seen, row, col);
    }
  }

  return regions;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned int pt1 = 0;
  unsigned int pt2 = 0;

  vector<int> lengths = parse_input();

  vector<bitset<128>> grid;
  for (unsigned int i = 0; i < 128; i++) {
    bitset<128> row = create_row(lengths, i);
    grid.push_back(row);
    pt1 += static_cast<unsigned int>(row.count());
  }

  pt2 = solve_pt2(grid);

  std::cout << "--- Day 14: Disk Defragmentation ---\n";
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
