#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using std::string;
using std::swap;
using std::vector;

struct Rule {
  vector<vector<vector<char>>> input;
  vector<vector<char>> output;
};

vector<vector<char>> flip(const vector<vector<char>> &grid) {
  vector<vector<char>> copy = grid;
  for (auto &row : copy) {
    std::reverse(row.begin(), row.end());
  }
  return copy;
}

vector<vector<char>> rotate(const vector<vector<char>> &grid) {
  vector<vector<char>> copy = grid;

  size_t size = grid.size();
  // transpose the matrix
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i; j < size; ++j) {
      swap(copy[i][j], copy[j][i]);
    }
  }

  // reverse each column
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size / 2; ++j) {
      swap(copy[j][i], copy[size - j - 1][i]);
    }
  }
  return copy;
}

vector<Rule> parse_input() {
  string line;
  vector<Rule> rules;
  while (std::getline(std::cin, line)) {
    Rule r;

    // parse input pattern
    auto start = line.begin();
    auto eol = std::find(start, line.end(), ' ');
    vector<vector<char>> tile;
    while (true) {
      auto end = std::find(start, eol, '/');
      tile.emplace_back(vector<char>(start, end));
      if (end == eol) {
        break;
      }

      start = end + 1;
    }
    r.input.emplace_back(tile);

    // parse output pattern
    start = eol + 4;
    eol = line.end();
    while (true) {
      auto end = std::find(start, eol, '/');
      r.output.emplace_back(vector<char>(start, end));
      if (end == eol) {
        break;
      }

      start = end + 1;
    }

    rules.emplace_back(r);
  }

  // generate transformations of every tile in rule patterns
  for (Rule &rule : rules) {
    vector<vector<char>> grid = rule.input[0];
    for (int j = 0; j < 3; j++) {
      grid = flip(grid);
      rule.input.emplace_back(grid);
      grid = rotate(grid);
      rule.input.emplace_back(grid);
      grid = flip(grid);
      rule.input.emplace_back(grid);
    }
  }

  return rules;
}

vector<vector<vector<vector<char>>>> split(const vector<vector<char>> &grid,
                                           const size_t n) {
  size_t tw = (grid.size() / n);
  vector<vector<vector<vector<char>>>> tiles(tw);
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
          row.emplace_back(grid[(tr * n) + r][(tc * n) + c]);
        }
        tile.emplace_back(row);
      }
      tile_row.emplace_back(tile);
    }
    tiles[tr] = tile_row;
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

  for (size_t tr = 0; tr < tiles.size(); ++tr) {
    for (size_t tc = 0; tc < tiles[tr].size(); ++tc) {
      const auto &tile = tiles[tr][tc];
      const size_t ts = tile.size();
      for (size_t r = 0; r < ts; ++r) {
        const size_t gr = tr * ts + r;
        for (size_t c = 0; c < ts; ++c) {
          grid[gr][tc * ts + c] = tile[r][c];
        }
      }
    }
  }

  return grid;
}

void match_rule(vector<vector<char>> &tile, const vector<Rule> &rules) {
  for (const Rule &rule : rules) {
    if (rule.input[0].size() != tile.size()) {
      continue;
    }

    for (const vector<vector<char>> &pattern : rule.input) {
      if (tile == pattern) {
        tile = rule.output;
        return;
      }
    }
  }
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

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
        match_rule(t, rules);
      }
    }

    grid = join(tiles);

    if (i == 4) {
      for (const auto &row : grid) {
        for (const auto &cell : row) {
          if (cell == '#') {
            pt1++;
          }
        }
      }
    }
  }

  for (const auto &row : grid) {
    for (const auto &cell : row) {
      if (cell == '#') {
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
