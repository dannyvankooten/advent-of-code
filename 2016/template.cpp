#include <chrono>
#include <iostream>

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    std::string input;
    std::getline(std::cin, input);

    std::cout << "--- Day 1: No Time for a Taxicab ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
