#include <chrono>
#include <iostream>
#include <list>

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  unsigned int steps;
  std::cin >> steps;

  std::list<int> ring = {0};
  auto it = ring.begin();

  for (int i = 0; i < 2017; i++) {
    // step forward steps times
    for (unsigned int s = 0; s < steps % ring.size(); s++) {
      if (++it == ring.end()) {
        it = ring.begin();
      }
    }

    ring.insert(it, i + 1);
  }
  pt1 = *it;

  int pos = 0;
  int limit = 50000000;
  int n = 0;
  while (n < limit) {
    if (pos == 1) {
      pt2 = n;
    }

    int fits = (n - pos) / steps;
    n += fits + 1;
    pos = (pos + (fits + 1) * (steps + 1) - 1) % n + 1;
  }

  std::cout << "--- Day 17: Spinlock ---\n";
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
