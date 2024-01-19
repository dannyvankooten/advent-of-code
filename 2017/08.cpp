#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using std::regex;
using std::regex_match;
using std::smatch;
using std::string;
using std::string_view;
using std::unordered_map;

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  string input;
  unordered_map<string, int> registers;
  regex rx("^(\\w+) (\\w+) (-?\\d+) if (\\w+) ([!><=]+) (-?\\d+)$");
  smatch matches;

  while (std::getline(std::cin, input)) {
    regex_match(input, matches, rx);

    // first, parse and execute guard clause
    const string &cr = matches[4];
    const string &cop = matches[5];
    const int cv = std::stoi(matches[6]);
    bool guard = false;
    if (cop == ">=") {
      guard = registers[cr] >= cv;
    } else if (cop == "<=") {
      guard = registers[cr] <= cv;
    } else if (cop == ">") {
      guard = registers[cr] > cv;
    } else if (cop == "<") {
      guard = registers[cr] < cv;
    } else if (cop == "==") {
      guard = registers[cr] == cv;
    } else if (cop == "!=") {
      guard = registers[cr] != cv;
    }
    if (!guard) {
      continue;
    }

    // only then, execute instruction
    const string &op = matches[2];
    const string &r = matches[1];
    const int amount = std::stoi(matches[3]);
    if (op == "inc") {
      registers[r] += amount;
    } else {
      registers[r] -= amount;
    }

    // keep track of running max for part 2
    if (registers[r] > pt2) {
      pt2 = registers[r];
    }
  }

  // find max final register value for part 1
  for (const auto &[_, value] : registers) {
    if (value > pt1) {
      pt1 = value;
    }
  }

  std::cout << "--- Day 8: I Heard You Like Registers ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
