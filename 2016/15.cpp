#include <assert.h>
#include <chrono>
#include <iostream>
#include <regex>

using std::getline;
using std::regex;
using std::smatch;
using std::string;
using std::vector;

class Disc {
 public:
  int start;
  int npositions;
};

int drop_capsule(const vector<Disc>& discs) {
  // brute-force every start time until we were able to pass all disks
  // an alternative approach may be to use LCM?

  for (int tstart = 0;; tstart++) {
    int t = tstart;
    bool all = true;

    for (Disc d : discs) {
      t += 1;

      if ((d.start + t) % d.npositions != 0) {
        all = false;
        break;
      }
    }

    if (all) {
      return tstart;
    }
  }

  return -1;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  // parse puzzle input
  string input;
  regex rx(
      "^Disc #(\\d+) has (\\d+) positions; at time=0, it is at "
      "position (\\d+).");
  smatch matches;
  vector<Disc> discs;
  while (getline(std::cin, input)) {
    std::regex_match(input, matches, rx);
    assert(matches.size() == 4);
    int start = std::stoi(matches[3].str());
    int positions = std::stoi(matches[2].str());
    discs.push_back(Disc{start, positions});
  }

  pt1 = drop_capsule(discs);

  discs.push_back(Disc{0, 11});
  pt2 = drop_capsule(discs);

  std::cout << "--- Day 15: Timing is Everything ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
