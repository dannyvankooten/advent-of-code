#include <chrono>
#include <iostream>
#include <unordered_map>

enum class State {
  A,
  B,
  C,
  D,
  E,
  F,
};

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;

  State state{State::A};
  int steps = 12459852;
  std::unordered_map<int, bool> lint;
  int slot = 0;

  for (int i = 0; i < steps; i++) {

    switch (state) {
    case State::A: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot++;
        state = State::B;
        pt1++;
      } else {
        slot--;
        state = State::E;
      }
      break;
    }
    case State::B: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot++;
        state = State::C;
        pt1++;
      } else {
        slot++;
        state = State::F;
      }
      break;
    }
    case State::C: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot--;
        state = State::D;
        pt1++;
      } else {
        lint[slot] = false;
        pt1--;
        slot++;
        state = State::B;
      }
      break;
    }
    case State::D: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot++;
        state = State::E;
        pt1++;
      } else {
        lint[slot] = false;
        slot--;
        state = State::C;
        pt1--;
      }
      break;
    }
    case State::E: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot--;
        state = State::A;
        pt1++;
      } else {
        lint[slot] = false;
        slot++;
        state = State::D;
        pt1--;
      }
      break;
    }
    case State::F: {
      if (false == lint[slot]) {
        lint[slot] = true;
        slot++;
        state = State::A;
        pt1++;
      } else {
        slot++;
        state = State::C;
      }
      break;
    }
    }
  }

  std::cout << "--- Day 25: The Halting Problem ---\n";
  std::cout << "Part 1: " << pt1 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
