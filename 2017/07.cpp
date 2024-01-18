#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Program {
  string name;
  int weight;
  vector<string> childnames;
};

vector<Program> parse_input() {
  vector<Program> programs;
  std::string input;
  while (std::getline(std::cin, input)) {
    auto s = input.find_first_of(' ');
    Program p = {input.substr(0, s), std::stoi(&input[s + 2]), {}};

    size_t childstart = input.find(" -> ");

    if (childstart != string::npos) {
      const auto done = input.end();
      auto start = input.begin() + childstart + 4;
      while (true) {
        auto end = std::find_if(start, done, [](char ch) { return ch == ','; });
        p.childnames.push_back(string(start, end));
        if (end == done) {
          break;
        }
        start = end + 2;
      }
    }

    programs.push_back(p);
  }
  return programs;
}

bool is_root(const vector<Program>& programs, const string& name) {
  for (const Program& p : programs) {
    for (const string& childname : p.childnames) {
      if (childname == name) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  string pt1;
  int pt2 = 0;

  vector<Program> programs = parse_input();

  for (const Program& a : programs) {
    // skip all leafs
    if (a.childnames.size() == 0) {
      continue;
    }

    // potential root, check if referenced from any other program
    if (is_root(programs, a.name)) {
      pt1 = a.name;
      break;
    }
  }

  std::cout << "--- Day 7: Recursive Circus ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
