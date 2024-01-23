#include <array>
#include <chrono>
#include <iostream>
#include <vector>

using std::stoi;
using std::string;
using std::vector;

enum class Op {
  SND,
  SET,
  ADD,
  MUL,
  MOD,
  RCV,
  JGZ,
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
    if (line.starts_with("snd")) {
      i.op = Op::SND;
    } else if (line.starts_with("set")) {
      i.op = Op::SET;
    } else if (line.starts_with("add")) {
      i.op = Op::ADD;
    } else if (line.starts_with("mul")) {
      i.op = Op::MUL;
    } else if (line.starts_with("mod")) {
      i.op = Op::MOD;
    } else if (line.starts_with("rcv")) {
      i.op = Op::RCV;
    } else if (line.starts_with("jgz")) {
      i.op = Op::JGZ;
    } else {
      throw std::runtime_error(std::string("invalid line"));
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

int64_t solve_pt1(const vector<Instruction> &ins) {
  std::array<int64_t, 26> registers = {0};
  int64_t sound = 0;

  // 191 too low
  // 316 too low
  for (size_t i = 0; i < ins.size();) {
    const Instruction &in = ins[i];
    switch (in.op) {
    case Op::SND: {
      sound = registers[in.a.value];
      break;
    }
    case Op::SET: {
      registers[in.a.value] =
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    }
    case Op::ADD: {
      registers[in.a.value] +=
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    }
    case Op::MUL: {
      registers[in.a.value] *=
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    }
    case Op::MOD: {
      registers[in.a.value] %=
          in.b.type == REGISTER_ADDRESS ? registers[in.b.value] : in.b.value;
      break;
    }
    case Op::RCV: {
      if (registers[in.a.value] != 0) {
        return sound;
      }
      break;
    }
    case Op::JGZ: {
      int64_t value =
          in.a.type == REGISTER_ADDRESS ? registers[in.a.value] : in.a.value;
      if (value > 0) {
        i += (in.b.type == REGISTER_ADDRESS ? registers[in.b.value]
                                            : in.b.value);
        continue;
      }
      break;
    }
    }

    i++;
  }

  return 0;
}

using std::array;

int64_t solve_pt2(const vector<Instruction> &ins) {
  array<array<int64_t, 26>, 2> registers = {
      array<int64_t, 26>{0},
      array<int64_t, 26>{0},
  };
  registers[0]['p' - 'a'] = 0;
  registers[1]['p' - 'a'] = 1;
  array<vector<int64_t>, 2> tx;
  array<size_t, 2> txp = {0, 0};
  array<size_t, 2> ip = {0, 0};
  unsigned int active_program = 0;
  bool deadlock = false;

  // alternate between programs until a recv instruction
  while (ip[active_program] < ins.size()) {
    const Instruction &in = ins[ip[active_program]];
    switch (in.op) {
    case Op::SND: {
      tx[active_program].push_back(registers[active_program][in.a.value]);
      break;
    }
    case Op::SET: {
      registers[active_program][in.a.value] =
          in.b.type == REGISTER_ADDRESS ? registers[active_program][in.b.value]
                                        : in.b.value;
      break;
    }
    case Op::ADD: {
      registers[active_program][in.a.value] +=
          in.b.type == REGISTER_ADDRESS ? registers[active_program][in.b.value]
                                        : in.b.value;
      break;
    }
    case Op::MUL: {
      registers[active_program][in.a.value] *=
          in.b.type == REGISTER_ADDRESS ? registers[active_program][in.b.value]
                                        : in.b.value;
      break;
    }
    case Op::MOD: {
      registers[active_program][in.a.value] %=
          in.b.type == REGISTER_ADDRESS ? registers[active_program][in.b.value]
                                        : in.b.value;
      break;
    }
    case Op::RCV: {
      if (tx[active_program == 0 ? 1 : 0].size() < 1 + txp[active_program]) {
        active_program = active_program == 0 ? 1 : 0;
        if (deadlock) {
          return tx[1].size();
        } else {
          deadlock = true;
          continue;
        }
      } else {
        deadlock = false;
        registers[active_program][in.a.value] =
            tx[active_program == 0 ? 1 : 0][txp[active_program]];
        txp[active_program]++;
      }
      break;
    }
    case Op::JGZ: {
      int64_t value = in.a.type == REGISTER_ADDRESS
                          ? registers[active_program][in.a.value]
                          : in.a.value;
      if (value > 0) {
        ip[active_program] += (in.b.type == REGISTER_ADDRESS
                                   ? registers[active_program][in.b.value]
                                   : in.b.value);
        continue;
      }
      break;
    }
    }

    ip[active_program]++;
  }

  // if both programs are at a recv instruction
  // return length of tx[1]

  return tx[1].size();
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  std::string line;
  std::vector<Instruction> ins = parse_input();

  int64_t pt1 = solve_pt1(ins);
  int64_t pt2 = solve_pt2(ins);

  std::cout << "--- Day 18: Duet ---\n";
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
