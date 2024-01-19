#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

void reverse(vector<int> &list, unsigned int start, unsigned int length) {
  for (unsigned int i = 0; i < length / 2; i++) {
    vector<int>::size_type idx_a = (start + i) % (list.size());
    vector<int>::size_type idx_b = (start + length - i - 1) % (list.size());
    std::swap(list[idx_a], list[idx_b]);
  }
}

int solve_pt1(const vector<int> &lengths) {
  vector<int> list(256);
  for (int i = 0; i < 256; i++) {
    list[i] = i;
  }

  int pos = 0;
  int skip = 0;
  for (const int length : lengths) {
    reverse(list, pos, length);
    pos += length + skip;
    skip += 1;
  }

  return list.at(0) * list.at(1);
}

string solve_pt2(const vector<int> &lengths) {
  vector<int> list(256);
  for (int i = 0; i < 256; i++) {
    list[i] = i;
  }

  int pos = 0;
  int skip = 0;
  for (unsigned int i = 0; i < 64; i++) {
    for (const int length : lengths) {
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

  // now, dense hash to hexadecimal
  std::stringstream stream;
  for (unsigned int i = 0; i < 16; i++) {
    stream << std::hex << dense_hash[i];
  }

  return stream.str();
}

struct Lengths {
  vector<int> numerical;
  vector<int> ascii;
};

Lengths parse_input() {
  vector<int> lengths_pt1;
  vector<int> lengths_pt2;
  std::string input;
  std::getline(std::cin, input);

  auto it = input.begin();
  auto end = input.end();
  lengths_pt1.push_back(std::stoi(&*it));
  while (it != end) {
    if (*it == ',') {
      lengths_pt1.push_back(std::stoi(&(*(it + 1))));
    }

    lengths_pt2.push_back(*it);
    it++;
  }
  lengths_pt2.insert(lengths_pt2.end(), {17, 31, 73, 47, 23});

  return {lengths_pt1, lengths_pt2};
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  const auto [lengths_pt1, lengths_pt2] = parse_input();
  int pt1 = solve_pt1(lengths_pt1);
  string pt2 = solve_pt2(lengths_pt2);

  std::cout << "--- Day 10: Knot Hash ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
