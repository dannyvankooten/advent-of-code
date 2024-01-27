#include <cctype>
#include <chrono>
#include <deque>
#include <iostream>
#include <queue>
#include <vector>

using std::deque;
using std::greater;
using std::priority_queue;
using std::string;
using std::vector;

typedef vector<vector<char>> grid_t;

struct Point {
  size_t col;
  size_t row;
  unsigned int nr;
};

struct Edge {
  Point position;
  unsigned int dist;
};

struct Delta {
  int col;
  int row;
};

Delta directions[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

vector<vector<char>> parse_input() {
  string line;
  vector<vector<char>> grid;

  while (std::getline(std::cin, line)) {
    vector<char> row;
    row.reserve(line.size());

    for (const char ch : line) {
      row.push_back(ch);
    }
    grid.push_back(row);
  }

  return grid;
}

vector<Point> find_poi(const grid_t &grid) {
  vector<Point> pois(8);

  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid[r].size(); c++) {
      if (std::isdigit(grid[r][c])) {
        unsigned int idx = static_cast<unsigned int>(grid[r][c] - '0');
        pois[idx] = Point{c, r, idx};
      }
    }
  }
  return pois;
}

vector<vector<Edge>> create_adjacency_graph(const grid_t &grid,
                                            const vector<Point> &pois) {
  vector<vector<Edge>> graph;
  deque<Edge> q;
  vector<bool> visited;

  // for every point of interest
  // find shortest path to every other point of interest
  // breadth-first
  for (const auto &poi : pois) {
    q.clear();
    visited.clear();
    visited.resize((1 << 8) * grid.size());

    vector<Edge> adj;
    q.push_back(Edge{poi, 0});

    while (!q.empty()) {
      auto [u, dist] = q.front();
      q.pop_front();
      visited[(u.row << 8) + u.col] = true;

      // if this is a point of interest
      // add it to adj graph
      if (dist > 0 && isdigit(grid[u.row][u.col])) {
        u.nr = static_cast<unsigned int>(grid[u.row][u.col] - '0');
        adj.push_back(Edge{u, dist});
        continue;
      }

      // visit neighbors
      for (const Delta &d : directions) {
        Point v{static_cast<unsigned int>(static_cast<int>(u.col) + d.col),
                static_cast<unsigned int>(static_cast<int>(u.row) + d.row), 0};
        if (v.row >= grid.size() || v.col >= grid[0].size() ||
            grid[v.row][v.col] == '#' || visited[(v.row << 8) + v.col]) {
          continue;
        }

        q.push_back(Edge{v, dist + 1});
        visited[(v.row << 8) + v.col] = true;
      }
    }

    graph.push_back(adj);
  }

  return graph;
}

int dijkstra(const vector<vector<Edge>> &graph, bool return_to_start) {
  struct State {
    unsigned int dist;
    unsigned int nr;
    unsigned int visited;

    inline bool operator>(const State &o) const noexcept {
      return dist > o.dist;
    };
  };

  priority_queue<State, vector<State>, greater<State>> q;
  q.push(State{0u, 0u, 0u});

  unsigned int done = 0u;
  for (unsigned int i = 0; i < graph.size(); i++) {
    done |= (1 << i);
  }

  while (!q.empty()) {
    auto [dist, u, visited] = q.top();
    q.pop();

    // mark poi as visited
    visited |= (1u << u);
    if (visited == done && (!return_to_start || u == 0)) {
      return static_cast<int>(dist);
    }

    for (const auto &el : graph[u]) {
      auto [v, d] = el;
      q.push(State{dist + d, v.nr, visited});
    }
  }

  return 0;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  vector<vector<char>> grid = parse_input();
  vector<Point> pois = find_poi(grid);
  vector<vector<Edge>> graph = create_adjacency_graph(grid, pois);

  pt1 = dijkstra(graph, false);
  pt2 = dijkstra(graph, true);

  std::cout << "--- Day 24: Air Duct Spelunking ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
