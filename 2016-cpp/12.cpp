#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>

int run_vm(std::unordered_map<char, int> registers,
           std::vector<std::string> instructions) {
    for (int ip = 0; ip < instructions.size();) {
        std::string ins = instructions[ip];

        switch (ins[0]) {
        case 'c': {
            size_t pos = ins.find(' ') + 1;
            int value;
            if (ins[pos] >= 'a' && ins[pos] <= 'd') {
                value = registers[ins[pos]];
            } else {
                value = std::stoi(&ins[pos]);
            }
            pos = ins.find(' ', pos) + 1;
            char r = ins[pos];
            registers[r] = value;
            break;
        }
        case 'i': {
            size_t pos = ins.find(' ') + 1;
            char r = ins[pos];
            registers[r] += 1;
            break;
        }
        case 'd': {
            size_t pos = ins.find(' ') + 1;
            char r = ins[pos];
            registers[r] -= 1;
            break;
        }
        case 'j':
            size_t pos = ins.find(' ') + 1;
            int value;
            if (ins[pos] >= 'a' && ins[pos] <= 'd') {
                value = registers[ins[pos]];
            } else {
                value = std::stoi(&ins[pos]);
            }

            if (value > 0) {
                pos = ins.find(' ', pos) + 1;
                value = std::stoi(&ins[pos]);
                ip += value;
                continue;
            }
            break;
        }

        ip += 1;
    }

    return registers['a'];
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    std::unordered_map<char, int> registers;
    std::vector<std::string> instructions;
    std::string line;
    while (std::getline(std::cin, line)) {
        instructions.push_back(line);
    }

    registers = {
        {'a', 0},
        {'b', 0},
        {'c', 0},
        {'d', 0},
    };
    pt1 = run_vm(registers, instructions);

    registers = {
        {'a', 0},
        {'b', 0},
        {'c', 1},
        {'d', 0},
    };
    pt2 = run_vm(registers, instructions);

    std::cout << "--- Day 12: Leonardo's Monorail ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
