#include <chrono>
#include <iostream>
#include <unordered_map>

enum {
  NORTH = 0,
  EAST,
  SOUTH,
  WEST,
};

struct Point {
  int x;
  int y;

  void operator+=(const Point& other) {
    x += other.x;
    y += other.y;
  };

  int hash() { return (x << 16) + y; }
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

  Point start = {0, 0};
  Point pos = {0, 0};
  int dir = NORTH;

  std::string input;
  std::getline(std::cin, input);

  std::unordered_map<int, bool> m = {};

  int i = 0;
  while (true) {
    if (input[i++] == 'R') {
      dir = (dir == 3) ? 0 : dir + 1;
    } else {
      dir = (dir == 0) ? 3 : dir - 1;
    }

    int amount = std::stoi(&input[i]);

    while (amount > 0) {
      pos += directions[dir];

      // check if we've been at this x, y before
      if (pt2 == -1 && m.count(pos.hash()) > 0) {
        pt2 = manhattan_distance(pos, start);
      }

      m.insert({pos.hash(), true});

      amount--;
    }

    // skip forward to next number
    i = input.find(',', i + 1);
    if (i == std::string::npos) {
      break;
    } else {
      i += 2;
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
