#include <chrono>
#include <iostream>

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::string input;
  std::getline(std::cin, input);

  for (size_t i = 0; i < input.length(); i++) {
    size_t j = i == input.length() - 1 ? 0 : i + 1;
    if (input[i] == input[j]) {
      pt1 += input[i] - '0';
    }

    size_t k = (i + (input.length() / 2)) % input.length();
    if (input[i] == input[k]) {
      pt2 += input[i] - '0';
    }
  }

  std::cout << "--- Day 1: Inverse Captcha ---\n";
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
