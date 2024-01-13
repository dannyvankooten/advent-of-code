#include <chrono>
#include <iostream>
#include <string>
#include <vector>

enum OpCode { SWAP_POS, SWAP_CHAR, ROTATE, ROTATE_ON_CHAR_POS, REVERSE, MOVE };

struct Instruction {
  OpCode op;
  int a;
  int b;
};

using std::string, std::string_view;
using std::vector;

string scramble(string s, vector<Instruction> instructions) {
  char tmp;

  for (Instruction& ins : instructions) {
    switch (ins.op) {
      case SWAP_POS: {
        tmp = s[ins.a];
        s[ins.a] = s[ins.b];
        s[ins.b] = tmp;
        break;
      }

      case SWAP_CHAR: {
        int a = s.find(ins.a);
        int b = s.find(ins.b);
        s[a] = (char)ins.b;
        s[b] = (char)ins.a;
        break;
      }
      case ROTATE: {
        string c = s;
        for (size_t i = 0; i < s.length(); i++) {
          int pos = i + ins.a;
          while (pos < 0) {
            pos += s.length();
          }
          while (pos >= s.length()) {
            pos -= s.length();
          }
          s[i] = c[pos];
        }
        break;
      }
      case ROTATE_ON_CHAR_POS: {
        size_t n = s.find(ins.a);
        n += (n >= 4) ? 2 : 1;
        n *= -1;
        string c = s;
        for (size_t i = 0; i < s.length(); i++) {
          int pos = i + n;
          while (pos < 0) {
            pos += s.length();
          }

          s[i] = c[pos];
        }

        break;
      }
      case REVERSE: {
        string c = s;
        for (int i = ins.a, j = ins.b; i <= ins.b; i++, j--) {
          s[i] = c[j];
        }
        break;
      }
      case MOVE: {
        char c = s.at(ins.a);
        // shift everything to the right of ins.a to the left
        for (size_t i = ins.a; i < s.length(); i++) {
          s[i] = s[i + 1];
        }

        // insert c at ins.b
        for (size_t i = s.length() - 1; i > ins.b; i--) {
          s[i] = s[i - 1];
        }
        s[ins.b] = c;

        break;
      }
    }
  }

  return s;
}

string permute(string& a, int l, int r, vector<Instruction> instructions) {
  if (l == r) {
    return (scramble(a, instructions) == "fbgdceah") ? a : "";
  }

  // recurse into all possible permutations
  for (int i = l; i < r; i++) {
    std::swap(a[l], a[i]);
    string answer = permute(a, l + 1, r, instructions);
    if (answer != "") {
      return answer;
    }
    std::swap(a[l], a[i]);
  }

  // unreachable
  return "";
}

vector<Instruction> parse_input() {
  string input;
  vector<Instruction> ins;
  while (std::getline(std::cin, input)) {
    if (input.starts_with("swap position")) {
      ins.push_back(Instruction{SWAP_POS, input[14] - '0', input[30] - '0'});
    } else if (input.starts_with("swap letter")) {
      ins.push_back(Instruction{SWAP_CHAR, input[12], input[26]});
    } else if (input.starts_with("rotate based")) {
      ins.push_back(Instruction{ROTATE_ON_CHAR_POS, input[35], 0});
    } else if (input.starts_with("rotate")) {
      ins.push_back(Instruction{
          ROTATE, input[7] == 'r' ? -(input[13] - '0') : input[12] - '0', 0});
    } else if (input.starts_with("reverse")) {
      ins.push_back(Instruction{REVERSE, input[18] - '0', input[28] - '0'});
    } else {
      ins.push_back(Instruction{MOVE, input[14] - '0', input[28] - '0'});
    }
  }
  return ins;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  vector<Instruction> ins = parse_input();
  string pt1 = scramble("abcdefgh", ins);

  // lazy pt2, just permute all 8**8 options
  string pw = "abcdefgh";
  string pt2 = permute(pw, 0, 8, ins);

  std::cout << "--- Day 21: Scrambled Letters and Hash ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}

// bedhafcg not right
