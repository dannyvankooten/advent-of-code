#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::string;
using std::vector;

enum class Op {
  SPIN,
  SWAP_POS,
  SWAP_CHAR,
};

struct Move {
  Op op;
  char a;
  char b;
};

vector<Move> parse_input() {
  vector<Move> moves;
  std::string input;
  while (std::getline(std::cin, input, ',')) {
    switch (input[0]) {
    case 's': {
      char s = static_cast<char>(std::stoi(&input[1]));
      moves.push_back({Op::SPIN, s, 0});
      break;
    }

    case 'x': {
      char i = static_cast<char>(std::stoi(&input[1]));
      char j = static_cast<char>(std::stoi(&input.at(input.find('/') + 1)));
      moves.push_back({Op::SWAP_POS, i, j});
      break;
    }

    case 'p': {
      moves.push_back({Op::SWAP_CHAR, input.at(1), input.at(3)});
      break;
    }
    }
  }
  return moves;
}

string dance(const vector<Move> &moves, string &programs) {
  const auto len = programs.size();

  for (const Move &m : moves) {
    switch (m.op) {
    case Op::SPIN: {
      // O(n) here, unless we use something like a ring-buffer
      programs.insert(0, programs.substr(len - m.a, m.a));
      programs.resize(len);
      break;
    }
    case Op::SWAP_POS: {
      std::swap(programs.at(m.a), programs.at(m.b));
      break;
    }
    case Op::SWAP_CHAR: {
      auto i = programs.find(m.a);
      auto j = programs.find(m.b);
      std::swap(programs[i], programs[j]);
      break;
    }
    }
  }

  return programs;
}

string solve_pt2(const vector<Move> &moves) {
  std::unordered_map<string, int> seen;
  string programs = "abcdefghijklmnop";

  // first, detect cycle start + length
  // we simply use a hashmap for previous states
  // trading memory for speed
  // if cycle length is very large
  // we should use something like tortoise and hare here
  int c = 0;
  int cycle_length = 0;
  for (; c < 1000000000; c++) {
    dance(moves, programs);

    if (seen.contains(programs)) {
      cycle_length = c - seen[programs];
      c = seen[programs] + 1;
      break;
    }

    seen.insert({programs, c});
  }

  for (; c < 1000000000 % cycle_length; c++) {
    dance(moves, programs);
  }

  return programs;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  vector<Move> moves = parse_input();
  string programs = "abcdefghijklmnop";
  string pt1 = dance(moves, programs);
  string pt2 = solve_pt2(moves);

  std::cout << "--- Day 16: Permutation Promenade ---\n";
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
