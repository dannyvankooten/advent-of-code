#include <chrono>
#include <iostream>
#include <queue>
#include <utility>

using std::pair, std::make_pair, std::priority_queue, std::vector;

struct Point {
  int col;
  int row;

  bool operator==(const Point& other) const {
    return (col == other.col && row == other.row);
  }

  Point operator+(const Point& other) const {
    return Point{
        col + other.col,
        row + other.row,
    };
  }
};

struct myComp {
  constexpr bool operator()(pair<int, Point> const& a,
                            pair<int, Point> const& b) const noexcept {
    return a.first > b.first;
  }
};

struct Point directions[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

unsigned int count_bits_on(unsigned int n) {
  unsigned int count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}

bool is_open_space(Point p, int favnum) {
  unsigned int n =
      p.col * p.col + 3 * p.col + 2 * p.col * p.row + p.row + p.row * p.row;
  n += favnum;
  return (count_bits_on(n) % 2) == 0;
}

pair<int, int> dijkstra(int favnum) {
  Point s = Point{1, 1};
  Point e = Point{31, 39};
  int nlocations = 0;
  bool visited[80 * 80] = {false};

  priority_queue<pair<int, Point>, vector<pair<int, Point>>, myComp> q;
  q.push(make_pair(0, s));

  while (!q.empty()) {
    const auto [d, v] = q.top();
    q.pop();

    if (v == e) {
      return make_pair(d, nlocations);
    }

    int h = (v.col * 80) + v.row;
    if (d <= 50 && !visited[h]) {
      nlocations++;
    }

    visited[h] = true;

    // visit neighbors
    for (auto delta : directions) {
      Point n = v + delta;

      if (n.col < 0 || n.row < 0) {
        continue;
      }

      if (visited[(n.col * 80) + n.row]) {
        continue;
      }

      if (!is_open_space(n, favnum)) {
        continue;
      }

      q.push(make_pair(d + 1, n));
    }
  }

  /* unreachable */
  __builtin_unreachable();
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  std::string input;
  std::getline(std::cin, input);
  int favnum = std::stoi(input);

  // dijkstra to find shortest path to 31, 39
  pair<int, int> answers = dijkstra(favnum);

  std::cout << "--- Day 13: A Maze of Twisty Little Cubicles ---\n";
  std::cout << "Part 1: " << answers.first << "\n";
  std::cout << "Part 2: " << answers.second << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
