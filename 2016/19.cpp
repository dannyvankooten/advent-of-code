#include <chrono>
#include <cstddef>
#include <deque>
#include <iostream>
#include <vector>

// Josephus Problem
// https://www.youtube.com/watch?v=uCsD3ZGzMgE

// Elf is a singly-linked list
// so we can remove elves from the table in O(1)
struct Elf {
  int number;
  Elf* next;
};

int white_elephant_party(int nplayers) {
  std::vector<Elf> elves(nplayers);
  elves[0] = {1, NULL};
  Elf* head = &elves[0];
  Elf* prev = head;

  for (int e = 2; e <= nplayers; e++) {
    Elf* cur = &elves[e - 1];
    cur->number = e;
    prev->next = cur;
    prev = cur;
  }
  prev->next = head;

  Elf* e = head;
  for (int i = 0; i < nplayers - 1; i++) {
    Elf* n = e->next;
    e->next = n->next;
    e = e->next;
  }

  return e->number;
}

int winner(int n) {
  int w = 1;

  for (int i = 1; i < n; i++) {
    w = w % i + 1;
    if (w > (i + 1) / 2) {
      w++;
    }
  }
  return w;
}

int white_elephant_party_2(int nplayers) {
  std::deque<int> left;
  std::deque<int> right;

  // create table
  for (int i = 1; i < nplayers + 1; i++) {
    if (i < (nplayers / 2) + 1) {
      left.push_back(i);
    } else {
      right.push_front(i);
    }
  }

  // alternate smallest side and then rotate table
  while (left.empty() == false && right.empty() == false) {
    if (left.size() > right.size()) {
      left.pop_back();
    } else {
      right.pop_back();
    }

    // rotate
    right.push_front(left.front());
    left.pop_front();
    left.push_back(right.back());
    right.pop_back();
  }

  return left.front();
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  int number_of_elves = std::stoi(input);
  // number_of_elves = 5;

  pt1 = white_elephant_party(number_of_elves);
  // pt2 = white_elephant_party_2(number_of_elves);
  pt2 = winner(number_of_elves);

  std::cout << "--- Day 19: An Elephant Named Joseph ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
