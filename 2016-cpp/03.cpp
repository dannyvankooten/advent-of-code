#include <chrono>
#include <iostream>
#include <string>

bool is_valid_triangle(int a, int b, int c) {
    return a + b > c && a + c > b && b + c > a;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    int a, b, c;
    std::string input;
    int window[3][3];
    int wi = 0;

    while (std::getline(std::cin, input)) {
        a = std::stoi(&input[0]);
        b = std::stoi(&input[5]);
        c = std::stoi(&input[10]);

        if (is_valid_triangle(a, b, c)) {
            pt1++;
        }

        window[0][wi] = a;
        window[1][wi] = b;
        window[2][wi] = c;

        // every 3 rows
        if (wi == 2) {
            for (int i = 0; i < 3; i++) {
                if (is_valid_triangle(window[i][0], window[i][1],
                                      window[i][2])) {
                    pt2++;
                }
            }
            wi = 0;
        } else {
            wi++;
        }
    }

    std::cout << "--- Day 3: Squares With Three Sides ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
