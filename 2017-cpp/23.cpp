#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

using std::stoi;
using std::string;
using std::vector;

enum class Op {
  SET,
  SUB,
  MUL,
  JNZ,
};

enum param_type { REGISTER_ADDRESS, VALUE };

struct Param {
  param_type type;
  int value;
};

struct Instruction {
  Op op;
  Param a;
  Param b;
};

vector<Instruction> parse_input() {
  string line;
  vector<Instruction> ins;

  while (std::getline(std::cin, line)) {
    Instruction i;

    // parse op
    if (line.starts_with("set")) {
      i.op = Op::SET;
    } else if (line.starts_with("sub")) {
      i.op = Op::SUB;
    } else if (line.starts_with("mul")) {
      i.op = Op::MUL;
    } else if (line.starts_with("jnz")) {
      i.op = Op::JNZ;
    } else {
      throw std::runtime_error(std::string("invalid line: " + line));
    }

    // parse first parameter
    if (isalpha(line.at(4))) {
      i.a = {REGISTER_ADDRESS, line.at(4) - 'a'};
    } else {
      i.a = {VALUE, stoi(&line[4])};
    }

    // parse 2nd parameter
    if (line.length() >= 6) {
      if (isalpha(line.at(6))) {
        i.b = {REGISTER_ADDRESS, line.at(6) - 'a'};
      } else {
        i.b = {VALUE, stoi(&line[6])};
      }
    }

    ins.push_back(i);
  }

  return ins;
}

int solve_pt1(const vector<Instruction> ins) {
  int mul_count = 0;
  std::array<int, 26> registers = {0};

  for (size_t ip = 0; ip < ins.size(); ip++) {
    const Instruction &in = ins[ip];

    switch (in.op) {
    case Op::SET: {
      registers[in.a.value] =
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    };
    case Op::SUB: {
      registers[in.a.value] -=
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    };
    case Op::MUL: {
      mul_count++;
      registers[in.a.value] *=
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    };
    case Op::JNZ:
      int value =
          in.a.type == REGISTER_ADDRESS ? registers[in.a.value] : in.a.value;
      int jump_offset =
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      if (value != 0) {
        ip += jump_offset - 1;
        continue;
      }
      break;
    }
  }

  return mul_count;
}

// part 2: 1001 too high
int solve_pt2() {
  int b = 79 * 100 + 100000;
  int c = b + 17000;
  int d = 0, f = 0, g = 0, h = 0;

  do {
    f = 1;
    d = 2;

    for (; d * d < b; ++d) {
      if (b % d == 0) {
        f = 0;
        break;
      }
    }
    if (f == 0) {
      h++;
    }
    g = b - c;
    b += 17;
  } while (g != 0);

  return h;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  vector<Instruction> ins = parse_input();

  pt1 = solve_pt1(ins);
  pt2 = solve_pt2();

  std::cout << "--- Day 23: Coprocessor Conflagration ---\n";
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
