#include <chrono>
#include <iostream>
#include <regex>
#include <string>

using std::vector, std::string;

struct Node {
  int x;
  int y;
  int size;
  int used;
  int available;

  bool operator==(const Node& other) const {
    return other.x == this->x && other.y == this->y;
  }
};

int solve_pt1(const vector<Node>& nodes) {
  int count = 0;

  for (const Node& a : nodes) {
    for (const Node& b : nodes) {
      if (a != b && a.used > 0 && a.used < b.available) {
        count++;
      }
    }
  }

  return count;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;

  // skip command and header line
  std::getline(std::cin, input);
  std::getline(std::cin, input);

  std::regex rx(
      "^\\/dev\\/grid\\/node-x(\\d+)-y(\\d+) +(\\d+)T +(\\d+)T +(\\d+)T "
      "+(\\d+)%$");
  std::smatch matches;

  std::vector<Node> nodes;

  while (std::getline(std::cin, input)) {
    std::regex_match(input, matches, rx);

    nodes.push_back(Node{
        std::stoi(matches[1].str()),
        std::stoi(matches[2].str()),
        std::stoi(matches[3].str()),
        std::stoi(matches[4].str()),
        std::stoi(matches[5].str()),
    });
  }

  pt1 = solve_pt1(nodes);
  std::cout << "--- Day 22: Grid Computing ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
