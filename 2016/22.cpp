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

  int available() const { return size - used; };

  bool operator==(const Node& other) const {
    return other.x == x && other.y == y;
  };
  bool operator!=(const Node& other) const { return !(*this == other); };
};

unsigned int solve_pt1(const vector<Node>& nodes) {
  unsigned int count = 0;

  for (const Node& a : nodes) {
    for (const Node& b : nodes) {
      if (a != b && a.used > 0 && a.used < b.available()) {
        count++;
      }
    }
  }

  return count;
}

vector<Node> parse_input() {
  string input;
  vector<Node> nodes;

  // skip command and header line
  std::getline(std::cin, input);
  std::getline(std::cin, input);

  std::regex rx(
      "^\\/dev\\/grid\\/node-x(\\d+)-y(\\d+) +(\\d+)T +(\\d+)T +(\\d+)T "
      "+(\\d+)%$");
  std::smatch matches;

  while (std::getline(std::cin, input)) {
    std::regex_match(input, matches, rx);
    int x = std::stoi(matches[1].str());
    int y = std::stoi(matches[2].str());
    int size = std::stoi(matches[3].str());
    int used = std::stoi(matches[4].str());
    Node n = Node{x, y, size, used};
    nodes.push_back(n);
  }
  return nodes;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned int pt1 = 0;
  unsigned int pt2 = 0;

  vector<Node> nodes = parse_input();
  pt1 = solve_pt1(nodes);

  auto prev = nodes[0].x;
  for (const Node& n : nodes) {
    if (prev != n.x) {
      std::cout << std::endl;
    }
    prev = n.x;

    if (n.used > 0) {
      std::cout << n.used << "/" << n.size << "\t";
    } else {
      std::cout << "__/" << n.size << "\t";
    }
  }
  std::cout << std::endl;

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
