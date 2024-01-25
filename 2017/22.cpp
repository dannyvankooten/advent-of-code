#include <chrono>
#include <iostream>
#include <unordered_map>

using std::unordered_map;

struct Point {
  int row;
  int col;

  bool operator==(const Point &o) const noexcept {
    return row == o.row && col == o.col;
  }

  void operator+=(const Point &o) {
    row += o.row;
    col += o.col;
  }
};

constexpr char CLEAN = 0;
constexpr char WEAKENED = 1;
constexpr char INFECTED = 2;
constexpr char FLAGGED = 3;

namespace std {
template <> struct hash<Point> {
  inline size_t operator()(const Point &p) const noexcept {
    return static_cast<size_t>((p.row << 16) ^ p.col);
  }
};
} // namespace std

unordered_map<Point, unsigned char> parse_input() {

  std::string line;
  unordered_map<Point, unsigned char> grid;
  Point pos = {0, 0};
  while (std::getline(std::cin, line)) {

    // if start position is at our initialization value
    if (pos == Point{0, 0}) {
      // update start pos to be relative to middle
      // so 0,0 is {12,12} in our input
      pos.row = static_cast<int>(line.size() / 2) * -1;
      pos.col = pos.row;
    }

    for (const char ch : line) {
      grid[pos] = ch == '#' ? INFECTED : CLEAN;
      pos.col++;
    }
    pos.row += 1;
    pos.col = static_cast<int>(line.size() / 2) * -1;
  }

  return grid;
}

int solve_pt1(unordered_map<Point, unsigned char> grid) {
  int infection_count = 0;
  Point pos = {0, 0};
  Point directions[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  unsigned int dir = 0; // start facing north
  for (int i = 0; i < 10000; i++) {
    dir = (grid[pos] ? dir + 1 : dir - 1) % 4;
    grid[pos] = grid[pos] == INFECTED ? CLEAN : INFECTED;
    if (grid[pos] == INFECTED) {
      infection_count++;
    }
    pos += directions[dir];
  }

  return infection_count;
}

int solve_pt2(unordered_map<Point, unsigned char> grid) {
  int infection_count = 0;
  Point pos = {0, 0};
  Point directions[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  // lookup tables so we can be branchless inside the loop
  unsigned char next_state[] = {WEAKENED, INFECTED, FLAGGED, CLEAN};
  int state_values[] = {0, 0, 1, 0};
  int dd[] = {-1, 0, 1, 2};

  unsigned int dir = 0; // start facing north
  for (int i = 0; i < 10000000; i++) {
    unsigned char s = grid[pos];
    grid[pos] = next_state[s];
    infection_count += state_values[next_state[s]];
    dir = static_cast<unsigned int>((static_cast<int>(dir) + dd[s])) % 4;
    pos += directions[dir];
  }

  return infection_count;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  unordered_map<Point, unsigned char> grid = parse_input();
  int pt1 = solve_pt1(grid);
  int pt2 = solve_pt2(grid);

  std::cout << "--- Day 22: Sporifica Virus ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
