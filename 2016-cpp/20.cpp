#include <limits.h>
#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <regex>
#include <vector>

using std::sort;
using std::string;
using std::vector;

struct Range {
  unsigned long start;
  unsigned long end;
};

vector<Range> parse_input() {
  string input;
  vector<Range> ranges;

  std::regex rx("(\\d+)-(\\d+)");
  std::smatch matches;

  while (std::getline(std::cin, input)) {
    std::regex_match(input, matches, rx);
    if (matches.size() != 3) {
      throw std::exception();
    }
    unsigned long start = std::stoul(matches[1]);
    unsigned long end = std::stoul(matches[2]);
    ranges.push_back(Range{start, end});
  }

  struct {
    bool operator()(Range a, Range b) const { return a.start < b.start; }
  } cmp;
  sort(ranges.begin(), ranges.end(), cmp);
  return ranges;
};

unsigned long first_accepted_value(const vector<Range>& ranges) {
  unsigned long min = 0;
  for (auto const& r : ranges) {
    if (min >= r.start && min <= r.end) {
      min = r.end + 1;
    }

    if (min < r.start) {
      return min;
    }
  }

  return ULONG_MAX;
}

unsigned long count_accepted_values(const vector<Range>& ranges) {
  unsigned long min = 0;
  unsigned long count = 0;

  for (auto const& r : ranges) {
    // if current minimum falls inside range
    // update to end of range
    if (min >= r.start && min <= r.end) {
      min = r.end + 1;
    }

    // if we hve a gap, add it to count
    if (min < r.start) {
      count += r.start - min;
      min = r.end + 1;
    }
  }

  return count;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned long pt1 = 0;
  unsigned long pt2 = 0;

  vector<Range> ranges = parse_input();
  pt1 = first_accepted_value(ranges);
  pt2 = count_accepted_values(ranges);
  std::cout << "--- Day 20: Firewall Rules ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
