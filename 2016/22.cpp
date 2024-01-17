#include <chrono>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>

using std::priority_queue;
using std::unordered_map;
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

struct Coords {
  int x;
  int y;

  bool operator==(const Coords& o) const { return x == o.x && y == o.y; }
};

Coords find_src_node(const vector<Node>& nodes) {
  Coords dest = {0, 0};
  for (const Node& n : nodes) {
    if (n.x > dest.x) {
      dest = {n.x, n.y};
    }
  }
  return dest;
}

Coords find_empty_node(const vector<Node>& nodes) {
  for (const Node& n : nodes) {
    if (n.used == 0) {
      return Coords{n.x, n.y};
    }
  }

  // invalid state
  return Coords{-1, -1};
}

int manhattan(Coords a, Coords b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

struct State {
  int steps;
  Coords u;

  bool operator>(const State& o) const { return steps > o.steps; }
};

int dijkstra(Coords src, Coords dest, const vector<Node>& nodes) {
  unordered_map<int, bool> visited;
  priority_queue<State, vector<State>, std::greater<State>> q;
  q.push({0, src});

  while (!q.empty()) {
    const auto [steps, u] = q.top();
    q.pop();

    if (u == dest) {
      return steps;
    }

    // TODO: This has O(n) complexity
    // We should use an adjacency graph of some sort here
    for (const Node& n : nodes) {
      if (n.size <= 100 && manhattan({n.x, n.y}, u) == 1 &&
          !visited[(n.x * 40) + n.y]) {
        q.push({steps + 1, {n.x, n.y}});
        visited[(n.x * 40) + n.y] = true;
      }
    }
  }

  return 0;
}

int solve_pt2(const vector<Node>& nodes) {
  Coords src = find_src_node(nodes);
  Coords dest = {0, 0};

  // first, dijkstra to find shortest path from empty node to src node
  Coords empty = find_empty_node(nodes);
  int steps = dijkstra(empty, src, nodes);

  // then, just shuffle way to source using that empty spot
  return steps + ((src.x - 1) - dest.x) * 5;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  unsigned int pt1 = 0;
  int pt2 = 0;

  vector<Node> nodes = parse_input();
  pt1 = solve_pt1(nodes);
  pt2 = solve_pt2(nodes);

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
