#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

int count_in_group(const unordered_map<string, vector<string>> &programs,
                   unordered_set<string> &seen, const string &name) {

  int count = 1;
  seen.insert(name);
  const vector<string> &connections = programs.at(name);
  for (const string &conn : connections) {
    if (seen.contains(conn)) {
      continue;
    }

    count += count_in_group(programs, seen, conn);
  }
  return count;
}

int count_groups(const unordered_map<string, vector<string>> &programs,
                 unordered_set<string> &seen) {
  int groups = 0;
  for (const auto &[name, _] : programs) {
    if (seen.contains(name)) {
      continue;
    }

    groups += 1;

    // re-use function from part 1
    // to recursively (dfs) mark all programs in this group as seen
    count_in_group(programs, seen, name);
  }

  return groups;
}

unordered_map<string, vector<string>> parse_input() {
  string input;
  unordered_map<string, vector<string>> programs;

  while (std::getline(std::cin, input)) {
    auto it = input.begin();
    auto end = std::find(it, input.end(), ' ');
    string name(it, end);
    end = std::find(end, input.end(), '>');

    while (end != input.end()) {
      it = end + 2;
      end = std::find(it, input.end(), ',');
      programs[name].push_back(string(it, end));
    }
  }

  return programs;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  unordered_map<string, vector<string>> programs = parse_input();
  std::unordered_set<string> seen;
  pt1 = count_in_group(programs, seen, "0");

  // we could also re-use the map from part 1 here and add 1
  // but for correctness, let's just start fresh
  seen.clear();
  pt2 = count_groups(programs, seen);

  std::cout << "--- Day 12: Digital Plumber ---\n";
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
