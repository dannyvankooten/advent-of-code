#include <chrono>
#include <iostream>

bool find_bab_inside(std::string line, char aba[2]) {
    bool inside_square_brackets = false;
    for (size_t j = 0; j < line.length() - 2; j++) {
        if (inside_square_brackets) {
            if (line[j] == aba[1] && line[j + 2] == aba[1] &&
                line[j + 1] == aba[0]) {
                return true;
            }
        }

        if (line[j] == '[' || line[j] == ']') {
            inside_square_brackets = !inside_square_brackets;
        }
    }
    return false;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    std::string input;
    while (std::getline(std::cin, input)) {
        bool inside_square_brackets = false;
        bool has_abba_outside = false;
        bool has_abba_inside = false;
        bool has_bab = false;
        char aba[2] = {0, 0};

        for (size_t i = 0; i < input.length(); i++) {
            // detect ABBA
            if ((i < input.length() - 3) && input[i] != input[i + 1] &&
                input[i] == input[i + 3] && input[i + 1] == input[i + 2]) {
                if (inside_square_brackets) {
                    has_abba_inside = true;
                } else {
                    has_abba_outside = true;
                }
            }

            // detect ABA
            if (!inside_square_brackets && i < input.length() - 2 &&
                input[i] != input[i + 1] && input[i] == input[i + 2]) {
                aba[0] = input[i];
                aba[1] = input[i + 1];

                // loop over entire line to find BAB
                if (find_bab_inside(input, aba)) {
                    has_bab = true;
                }
            }

            if (input[i] == '[' || input[i] == ']') {
                inside_square_brackets = !inside_square_brackets;
            }
        }

        // part 1: ABBA outside but not inside
        if (has_abba_outside && !has_abba_inside) {
            pt1 += 1;
        }

        // part 2: ABA
        if (has_bab) {
            pt2 += 1;
        }
    }

    std::cout << "--- Day 7: Internet Protocol Version 7 ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
