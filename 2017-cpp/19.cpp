#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

struct Point {
  int row;
  int col;
};

Point directions[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
enum direction {
  NORTH,
  EAST,
  SOUTH,
  WEST

};

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  int w = 0;
  vector<vector<char>> grid;
  std::string line;
  while (std::getline(std::cin, line)) {
    vector<char> row(line.begin(), line.end());
    grid.push_back(row);
    w = std::max(w, static_cast<int>(row.size()));
  }

  const int h = static_cast<int>(grid.size());

  // find | on first row

  unsigned int d = SOUTH;
  string letters;
  int r = 0;
  int c = static_cast<int>(std::find(grid[0].begin(), grid[0].end(), '|') -
                           grid[0].begin());
  int steps = 0;

  while (true) {
    steps += 1;
    r = r + directions[d].row;
    c = c + directions[d].col;

    if (r >= h || c >= w) {
      break;
    }

    const char tile = grid[r][c];
    if (isalpha(tile)) {
      letters.push_back(tile);
    } else if (tile == '+') {
      d = (d == NORTH || d == SOUTH) ? EAST : NORTH;
      const int nr = r + directions[d].row;
      const int nc = c + directions[d].col;
      if (nr < h && nc < w) {
        const char tile = grid[nr][nc];
        if (isalpha(tile) || tile == '-' || tile == '|') {
          continue;
        }
      }
      d += 2;
    } else if (tile != '-' && tile != '|') {
      break;
    }
  }

  std::cout << "--- Day 19: A Series of Tubes ---\n";
  std::cout << "Part 1: " << letters << "\n";
  std::cout << "Part 2: " << steps << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
