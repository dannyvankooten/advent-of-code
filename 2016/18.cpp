#include <chrono>
#include <iostream>
#include <string.h>

int count_traps(std::string input, int nrows) {
    char prev_row[100];
    char cur_row[100];
    int trapcount = 0;

    for (int c = 0; c < 100; c++) {
        prev_row[c] = input[c];

        if (input[c] == '^') {
            trapcount += 1;
        }
    }

    char tiles[3];

    for (int r = 1; r < nrows; r++) {
        for (int c = 0; c < 100; c++) {
            tiles[0] = c == 0 ? '.' : prev_row[c - 1];
            tiles[1] = prev_row[c];
            tiles[2] = c == 100 - 1 ? '.' : prev_row[c + 1];

            if (memcmp(tiles, "^^.", 3) == 0 || memcmp(tiles, ".^^", 3) == 0 ||
                memcmp(tiles, "^..", 3) == 0 || memcmp(tiles, "..^", 3) == 0) {
                trapcount++;
                cur_row[c] = '^';
            } else {
                cur_row[c] = '.';
            }
        }

        std::swap(prev_row, cur_row);
    }

    return nrows * 100 - trapcount;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    std::string input;
    std::getline(std::cin, input);

    pt1 = count_traps(input, 40);
    pt2 = count_traps(input, 400000);

    std::cout << "--- Day 18: Like a Rogue ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
