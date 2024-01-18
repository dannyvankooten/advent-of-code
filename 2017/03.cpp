#include <chrono>
#include <functional>
#include <iostream>
#include <unordered_map>

using std::unordered_map;

struct Point {
  int x;
  int y;

  void operator+=(const Point& other) noexcept {
    x += other.x;
    y += other.y;
  }

  bool operator==(const Point& other) const noexcept {
    return x == other.x && y == other.y;
  }
};

int solve_pt1(int n) {
  Point pos = {0, 0};
  Point directions[] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  int steps = 1;
  unsigned int dir = 0;

  for (int i = 1; i < n;) {
    for (int j = 0; j < 2 && i < n; j++) {
      for (int k = 0; k < steps && i < n; k++, i++) {
        pos += directions[dir];
      }
      dir = (dir + 1) % 4;
    }
    steps += 1;
  }
  return abs(pos.x - 0) + abs(pos.y - 0);
}

int solve_pt2(int n) {
  Point pos = {0, 0};
  Point directions[] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  int steps = 1;
  unsigned int dir = 0;

  auto hash = [](const Point& p) {
    return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 16);
  };
  unordered_map<Point, int, decltype(hash)> grid(0, hash);
  grid[pos] = 1;

  auto sum_neighbors = [&grid](const Point& p) {
    int sum = 0;
    for (int y = -1; y <= 1; y++) {
      for (int x = -1; x <= 1; x++) {

        // skip self
        if ((x | y) == 0) {
          continue;
        }

        sum += grid[{p.x + x, p.y + y}];
      }
    }

    return sum;
  };

  for (int i = 1;;) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < steps; k++, i++) {
        pos += directions[dir];

        int value = sum_neighbors(pos);
        if (value > n) {
          return value;
        }

        grid[pos] = value;
      }
      dir = (dir + 1) % 4;
    }
    steps += 1;
  }

  return -1;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);
  int n = std::stoi(input);

  pt1 = solve_pt1(n);
  pt2 = solve_pt2(n);

  std::cout << "--- Day 3: Spiral Memory ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
