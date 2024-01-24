#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

using std::string;
using std::swap;
using std::vector;

struct Rule {
  vector<vector<char>> input;
  vector<vector<char>> output;
};

vector<Rule> parse_input() {
  string line;
  vector<Rule> rules;
  while (std::getline(std::cin, line)) {
    Rule r;

    // parse input pattern
    auto start = line.begin();
    auto eol = std::find(start, line.end(), ' ');
    while (true) {
      auto end = std::find(start, eol, '/');
      r.input.push_back(vector<char>(start, end));
      if (end == eol) {
        break;
      }

      start = end + 1;
    }

    // parse output pattern
    start = eol + 4;
    eol = line.end();
    while (true) {
      auto end = std::find(start, eol, '/');
      r.output.push_back(vector<char>(start, end));
      if (end == eol) {
        break;
      }

      start = end + 1;
    }

    rules.push_back(r);
  }
  return rules;
}

vector<vector<char>> &flip(vector<vector<char>> &grid) {
  for (auto &row : grid) {
    std::reverse(row.begin(), row.end());
  }
  return grid;
}

vector<vector<char>> &rotate(vector<vector<char>> &grid) {
  size_t size = grid.size();
  // transpose the matrix
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = i; j < size; j++) {
      swap(grid[i][j], grid[j][i]);
    }
  }

  // reverse each column
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size / 2; j++) {
      swap(grid[j][i], grid[size - j - 1][i]);
    }
  }
  return grid;
}

vector<vector<vector<vector<char>>>> split(const vector<vector<char>> &grid,
                                           size_t n) {
  vector<vector<vector<vector<char>>>> tiles;
  size_t tw = (grid.size() / n);
  tiles.reserve(tw);
  for (size_t tr = 0; tr < tw; tr++) {
    vector<vector<vector<char>>> tile_row;
    tile_row.reserve(tw);
    for (size_t tc = 0; tc < tw; tc++) {
      vector<vector<char>> tile;
      tile.reserve(n);
      for (size_t r = 0; r < n; r++) {
        vector<char> row;
        row.reserve(n);
        for (size_t c = 0; c < n; c++) {
          row.push_back(grid[(tr * n) + r][(tc * n) + c]);
        }
        tile.push_back(row);
      }
      tile_row.push_back(tile);
    }
    tiles.push_back(tile_row);
  }

  return tiles;
}

vector<vector<char>> join(const vector<vector<vector<vector<char>>>> &tiles) {
  // determine size of grid
  size_t sz = tiles.size() * tiles[0][0].size();
  vector<vector<char>> grid(sz);
  for (size_t i = 0; i < sz; i++) {
    grid[i].resize(sz);
  }

  for (size_t tr = 0; tr < tiles.size(); tr++) {
    for (size_t tc = 0; tc < tiles[tr].size(); tc++) {
      const auto &tile = tiles[tr][tc];
      for (size_t r = 0; r < tile.size(); r++) {
        for (size_t c = 0; c < tile[r].size(); c++) {
          grid[tr * tile.size() + r][tc * tile.size() + c] = tile[r][c];
        }
      }
    }
  }

  return grid;
}

bool matches(const vector<vector<char>> &grid,
             const vector<vector<char>> &pattern) {

  size_t n = pattern.size();
  for (unsigned int r = 0; r < n; r++) {
    for (unsigned int c = 0; c < n; c++) {
      if (grid[r][c] != pattern[r][c]) {
        return false;
      }
    }
  }

  return true;
}

bool matches_any(vector<vector<char>> &grid,
                 const vector<vector<char>> &pattern) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 2; j++) {
      if (matches(grid, pattern)) {
        return true;
      }
      flip(grid);
    }
    rotate(grid);
  }

  return false;
}

void run_tests() {

  vector<vector<char>> grid = {
      {'1', '2', '3'},
      {'4', '5', '6'},
      {'7', '8', '9'},
  };
  flip(grid);

  assert(matches(grid, {
                           {'3', '2', '1'},
                           {'6', '5', '4'},
                           {'9', '8', '7'},
                       }));

  rotate(grid);
  assert(matches(grid, {
                           {'1', '4', '7'},
                           {'2', '5', '8'},
                           {'3', '6', '9'},
                       }));
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  run_tests();

  vector<vector<char>> grid = {
      {'.', '#', '.'},
      {'.', '.', '#'},
      {'#', '#', '#'},
  };

  vector<Rule> rules = parse_input();

  for (int i = 0; i < 18; i++) {
    size_t sq_size = grid.size() % 2 == 0 ? 2 : 3;
    auto tiles = split(grid, sq_size);

    // go over tiles and look for match
    for (auto &tr : tiles) {
      for (auto &t : tr) {
        for (const Rule &rule : rules) {
          if (rule.input.size() != sq_size) {
            continue;
          }
          if (matches_any(t, rule.input)) {
            t = rule.output;
            goto next_tile;
          }
        }
      next_tile:
      }
    }

    grid = join(tiles);

    if (i == 4) {
      for (size_t r = 0; r < grid.size(); r++) {
        for (size_t c = 0; c < grid[r].size(); c++) {
          if (grid[r][c] == '#') {
            pt1++;
          }
        }
      }
    }
  }

  // 170: too high
  // 168 too high
  // 161 too high
  // 29: not right
  // 160?
  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid[r].size(); c++) {
      if (grid[r][c] == '#') {
        pt2++;
      }
    }
  }

  std::cout << "--- Day 21: Fractal Art ---\n";
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
