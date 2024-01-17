#include <chrono>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using size_type = std::string::size_type;

// mutate original according to puzzle rules
// length of returned string is twice the input + 1
void step(vector<bool>& a) {
  size_type l = a.size();
  a.resize(l * 2 + 1);
  a[l] = false;

  for (size_type i = 0; i < l; i++) {
    a[l + i + 1] = a[l - i - 1] ? false : true;
  }
}

// create odd-length checksum for input string
void checksum(vector<bool>& a) {
  size_type sz = a.size();
  unsigned int n = 1;

  // find number of halvings to reach odd number
  do {
    sz /= 2;
    n *= 2;
  } while ((sz & 1) == 0);

  // create checksum bit for each position
  for (unsigned int i = 0; i < sz; i++) {
    bool bit = true;
    unsigned int start = i * n;
    unsigned int end = (i + 1) * n;
    for (unsigned int j = start; j < end; j += 2) {
      bit ^= !(a[j] ^ a[j + 1]) & 1;
    }

    a[i] = bit;
  }

  a.resize(sz);
}

vector<bool> fill_disk_of_size(vector<bool>& in, size_type sz) {
  vector<bool> state = in;
  state.reserve(sz * 2);
  while (state.size() < sz) {
    step(state);
  }
  state.resize(sz);
  checksum(state);
  return state;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  string input;
  std::getline(std::cin, input);

  std::vector<bool> bits;
  for (char ch : input) {
    bits.push_back(ch == '1');
  }

  string pt1;
  for (bool b : fill_disk_of_size(bits, 272)) {
    pt1.push_back(b ? '1' : '0');
  }

  string pt2;
  for (bool b : fill_disk_of_size(bits, 35651584)) {
    pt2.push_back(b ? '1' : '0');
  }

  std::cout << "--- Day 16: Dragon Checksum ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
