#include <chrono>
#include <iostream>

#include <openssl/md5.h>
#include <string>

using std::string;
using std::to_string;

// md5 hash of str, n times, writing result into str
void md5(string& str, int n) {
  static unsigned char hash[MD5_DIGEST_LENGTH];
  static const char characters[] = "0123456789abcdef";
  str.reserve(MD5_DIGEST_LENGTH * 2);

  for (int i = 0; i < n; i++) {
    MD5(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);

    // write directly to input string
    str.clear();

    // faster method of converting to hexadecimal
    auto buf = std::back_inserter(str);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
      auto byte = hash[i];
      *buf++ = characters[byte >> 4];
      *buf++ = characters[byte & 0x0F];
    }
  }
}

int solve(const string& input, int n) {
  // generate first 1000 hashes
  string hashes[1000];
  for (int i = 0; i < 1000; i++) {
    hashes[i] = input + to_string(i);
    md5(hashes[i], n);
  }

  int nkeys = 0;
  for (int i = 0;; i++) {
    // pick first hash in deque
    string h = hashes[i % 1000];

    // generate next hash (md5 n times)
    hashes[i % 1000] = input + std::to_string(i + 1000);
    md5(hashes[i % 1000], n);

    // if this hash contains 3 repeating chars
    // store in seq so we can find whether it's valid key
    string seq;

    for (size_t j = 0; j < h.length() - 2; j++) {
      if (h[j] == h[j + 1] && h[j] == h[j + 2]) {
        seq = string(5, h[j]);
        break;
      }
    }

    // go through next 1000 hashes to find matching
    if (seq != "") {
      for (auto const& n : hashes) {
        if (n.find(seq) != string::npos) {
          if (++nkeys == 64) {
            return i;
          }

          break;
        }
      }
    }
  }

  /* unreachable */
  __builtin_unreachable();
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  string input;
  std::getline(std::cin, input);

  pt1 = solve(input, 1);
  pt2 = solve(input, 2017);

  std::cout << "--- Day 14: One-Time Pad ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
