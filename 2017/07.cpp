#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

struct Program {
  string name;
  int weight;
  vector<string> childnames;
};

unordered_map<string, Program> parse_input() {

  unordered_map<string, Program> programs;
  std::string input;
  while (std::getline(std::cin, input)) {
    auto s = input.find_first_of(' ');
    Program p = {input.substr(0, s), std::stoi(&input[s + 2]), {}};

    size_t childstart = input.find(" -> ");
    if (childstart != string::npos) {
      const auto done = input.end();
      auto start =
          input.begin() + static_cast<string::difference_type>(childstart) + 4;
      while (true) {
        auto end = std::find_if(start, done, [](char ch) { return ch == ','; });
        p.childnames.push_back(string(start, end));
        if (end == done) {
          break;
        }
        start = end + 2;
      }
    }

    programs.insert({p.name, p});
  }

  return programs;
}

// Program is a root if has children
// and is not referenced by any other program as one of its children
bool is_root(const unordered_map<string, Program>& programs,
             const string& name) {
  for (const auto& [_, p] : programs) {
    for (const string& childname : p.childnames) {
      if (childname == name) {
        return false;
      }
    }
  }

  return true;
}

int sum_weight(const unordered_map<string, Program>& programs,
               const Program& root) {
  int sum = root.weight;
  for (const string& childname : root.childnames) {
    const Program& child = programs.at(childname);
    sum += sum_weight(programs, child);
  }

  return sum;
}

const Program& find_root(unordered_map<string, Program>& programs) {
  for (const auto& [_, a] : programs) {
    // skip all leafs
    if (a.childnames.size() == 0) {
      continue;
    }

    // potential root, check if referenced from any other program
    if (is_root(programs, a.name)) {
      return a;
    }
  }

  throw new std::exception();
}

int solve_pt2(const unordered_map<string, Program>& programs,
              const Program& root, int unbalanced) {
  vector<int> weights;
  for (const string& c : root.childnames) {
    weights.push_back(sum_weight(programs, programs.at(c)));
  }

  int diff = 0;

  for (unsigned int i = 0; i < root.childnames.size(); i++) {
    for (unsigned int j = 0; j < root.childnames.size(); j++) {
      if (i == j) {
        continue;
      }

      diff = weights[i] - weights[j];
      if (diff == 0) {
        break;
      }
    }

    if (diff != 0) {
      return solve_pt2(programs, programs.at(root.childnames[i]), diff);
    }
  }

  return root.weight - unbalanced;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  unordered_map<string, Program> programs = parse_input();

  const Program& root = find_root(programs);
  string pt1 = root.name;
  int pt2 = solve_pt2(programs, root, 0);

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
