#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

int count_in_group(const vector<vector<int>> &programs, vector<bool> &seen,
                   const int name) {

  int count = 1;
  const vector<int> &connections = programs.at(name);

  seen[name] = true;

  for (const int conn : connections) {
    if (seen[conn]) {
      continue;
    }

    count += count_in_group(programs, seen, conn);
  }

  return count;
}

int count_groups(const vector<vector<int>> &programs) {
  int groups = 0;
  vector<bool> seen(programs.size());

  for (unsigned int p = 0; p < programs.size(); p++) {
    if (seen[p]) {
      continue;
    }

    groups += 1;

    // re-use function from part 1
    // to recursively (dfs) mark all programs in this group as seen
    count_in_group(programs, seen, p);
  }

  return groups;
}

vector<vector<int>> parse_input() {
  string input;
  vector<vector<int>> programs;
  programs.reserve(2000);

  while (std::getline(std::cin, input)) {
    auto it = input.begin();
    auto end = std::find(it, input.end(), '>');
    vector<int> conns;

    while (end != input.end()) {
      it = end + 2;
      end = std::find(it, input.end(), ',');
      int c = std::stoi(&(*it));
      conns.push_back(c);
    }
    programs.push_back(conns);
  }

  return programs;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  vector<vector<int>> programs = parse_input();
  vector<bool> seen(programs.size());
  pt1 = count_in_group(programs, seen, 0);
  pt2 = count_groups(programs);

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
