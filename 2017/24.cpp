#include <chrono>
#include <iostream>
#include <vector>

using std::stoi;
using std::vector;

struct Port {
  int in;
  int out;
};

vector<Port> parse_input() {
  vector<Port> ports;
  std::string input;
  while (std::getline(std::cin, input)) {
    ports.push_back(Port{
        stoi(&input[0]),
        stoi(&input[input.find('/') + 1]),
    });
  }
  return ports;
}

struct Answers {
  int strength;
  int length;
  int longest_strength;
};

Answers make_bridges(const vector<Port> &ports, vector<bool> &used,
                     int strength, int length, int longest_strength, int need) {
  Answers a{strength, length, longest_strength};

  // recurse all available options
  for (size_t p = 0; p < ports.size(); p++) {
    if (used[p] || (ports[p].in != need && ports[p].out != need)) {
      continue;
    }

    // mark port as used
    used[p] = true;

    // dfs further down
    int n = ports[p].in == need ? ports[p].out : ports[p].in;
    int s = strength + ports[p].in + ports[p].out;
    int ls = std::max(s, longest_strength);
    Answers b = make_bridges(ports, used, s, length + 1, ls, n);

    // keep track of max strength & max strength for longest
    a.strength = std::max(a.strength, b.strength);
    if (b.length > a.length) {
      a.length = b.length;
      a.longest_strength = b.longest_strength;
    } else if (b.length == a.length) {
      a.longest_strength = std::max(a.longest_strength, b.longest_strength);
    }

    used[p] = false;
  }

  return a;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  vector<Port> ports = parse_input();
  vector<bool> used(ports.size());

  Answers a = make_bridges(ports, used, 0, 0, 0, 0);

  std::cout << "--- Day 24: Electromagnetic Moat ---\n";
  std::cout << "Part 1: " << a.strength << "\n";
  std::cout << "Part 2: " << a.longest_strength << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
