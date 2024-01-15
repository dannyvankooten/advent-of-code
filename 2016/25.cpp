#include <chrono>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

enum OpCode {
  INC,
  DEC,
  COPY,
  JNZ,
  OUT,
};

struct Instruction {
  OpCode op;
  int a;
  int b;
};

int run_vm(int registers[4], const vector<Instruction>& instructions) {

  auto ip = instructions.begin();
  auto end = instructions.end();
  bool out = true;
  int nsignals = 0;
  while (ip != end) {
    const Instruction& ins = *ip;

    switch (ins.op) {
      case OpCode::COPY: {
        int value;
        if (ins.a & (1 << 31)) {
          value = registers[ins.a & 0xFF];
        } else {
          value = ins.a;
        }
        registers[ins.b] = value;
        break;
      }
      case OpCode::INC: {
        registers[ins.a] += 1;
        break;
      }
      case OpCode::DEC: {
        registers[ins.a] -= 1;
        break;
      }
      case OpCode::JNZ: {
        int value;

        if (ins.a & (1 << 31)) {
          value = registers[ins.a & 0xFF];
        } else {
          value = ins.a;
        }

        if (value != 0) {
          ip += ins.b;
          continue;
        }
        break;
      }
      case OpCode::OUT: {
        int value = registers[ins.a & 0xFF];
        if (value != !out) {
          return false;
        }
        if (++nsignals > 8) {
          return true;
        }
        out = !out;
        break;
      }
    }

    ip += 1;
  }

  return false;
}

vector<Instruction> parse_input() {
  vector<Instruction> instructions;

  string line;
  while (std::getline(std::cin, line)) {
    switch (line[0]) {
      case 'c': {
        size_t pos = line.find(' ') + 1;
        int value;

        // if this refers to a register address
        // set the first bit to 1
        if (line[pos] >= 'a' && line[pos] <= 'd') {
          value = (line[pos] - 'a') | (1 << 31);
        } else {
          value = std::stoi(&line[pos]);
        }
        pos = line.find(' ', pos) + 1;
        instructions.push_back(
            Instruction{OpCode::COPY, value, line[pos] - 'a'});
        break;
      }
      case 'i': {
        size_t pos = line.find(' ') + 1;
        instructions.push_back(Instruction{OpCode::INC, line[pos] - 'a', 0});
        break;
      }
      case 'd': {
        size_t pos = line.find(' ') + 1;
        instructions.push_back(Instruction{OpCode::DEC, line[pos] - 'a', 0});
        break;
      }
      case 'j': {
        size_t pos = line.find(' ') + 1;
        int v1;
        if (line[pos] >= 'a' && line[pos] <= 'd') {
          v1 = (line[pos] - 'a') | (1 << 31);
        } else {
          v1 = std::stoi(&line[pos]);
        }

        pos = line.find(' ', pos) + 1;
        int v2 = std::stoi(&line[pos]);
        instructions.push_back(Instruction{OpCode::JNZ, v1, v2});
        break;
      }
      case 'o': {
        size_t pos = line.find(' ') + 1;
        int v1 = (line[pos] - 'a') | (1 << 31);
        instructions.push_back(Instruction{OpCode::OUT, v1, 0});
        break;
      }
    }
  }
  return instructions;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  vector<Instruction> instructions = parse_input();

  for (int i = 0; i < 1000; i++) {
    int registers_pt1[4] = {i, 0, 0, 0};
    if (run_vm(registers_pt1, instructions)) {
      pt1 = i;
      break;
    }
  }

  std::cout << "--- Day 25: Clock Signal ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
