#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using std::stoi;
using std::string;
using std::vector;

int main() {
  vector<int> weights;
  string line;
  while (getline(std::cin, line)) {
    weights.emplace_back(stoi(line));
  }

  // for part 1, a greedy algorithm works
  std::sort(weights.begin(), weights.end(), std::greater<int>());
  int sum = std::accumulate(weights.begin(), weights.end(), 0);
  int group_sum = sum / 3;
  uint64_t qe = 1;
  for (int s = 0, i = 0; i < weights.size() && s < group_sum; i++) {
    if (s + weights[i] < group_sum) {
      s += weights[i];
      qe *= weights[i];
    }
  }

  std::cout << qe << std::endl;
}
