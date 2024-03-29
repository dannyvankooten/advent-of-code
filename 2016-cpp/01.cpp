#include <chrono>
#include <iostream>
#include <unordered_set>

using std::unordered_set;

enum {
  NORTH = 0,
  EAST,
  SOUTH,
  WEST,
};

struct Point {
  int x;
  int y;

  void operator+=(const Point& other) noexcept {
    x += other.x;
    y += other.y;
  };

  bool operator==(const Point& other) const noexcept {
    return x == other.x && y == other.y;
  };

  inline int hash() const { return (x << 16) + y; }
};

Point directions[4]{
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

int manhattan_distance(const Point& a, const Point& b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt2 = -1;

  Point start = {500, 500};
  Point pos = start;
  int dir = NORTH;

  std::string input;
  std::getline(std::cin, input);

  auto hash = [](const Point& o) {
    return (o.x << 16) + o.y;
  };
  std::unordered_set<Point, decltype(hash)> visited(0, hash);

  for (auto s = input.begin(); s != input.end();) {
    if (*s++ == 'R') {
      dir = (dir == 3) ? 0 : dir + 1;
    } else {
      dir = (dir == 0) ? 3 : dir - 1;
    }

    int amount = std::stoi(&*s);
    while (std::isdigit(*s)) {
      s++;
    }

    while (amount > 0) {
      pos += directions[dir];

      // check if we've been at this x, y before
      if (pt2 == -1 && visited.contains(pos)) {
        pt2 = manhattan_distance(pos, start);
      }

      visited.insert(pos);
      amount--;
    }

    // skip forward to next number
    if (*s == ',') {
      s += 2;
    }
  }

  int pt1 = manhattan_distance(pos, start);

  std::cout << "--- Day 1: No Time for a Taxicab ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
