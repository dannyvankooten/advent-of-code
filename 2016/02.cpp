#include <chrono>
#include <iostream>

struct pos {
    int row;
    int col;
};

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();

    char kp1[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
    };
    char kp2[5][5] = {
        {0, 0, '1', 0, 0},         //
        {0, '2', '3', '4', 0},     //
        {'5', '6', '7', '8', '9'}, //
        {0, 'A', 'B', 'C', 0},     //
        {0, 0, 'D', 0, 0},         //
    };

    std::string input;
    int index = 0;

    struct pos pt1 = {1, 1};
    struct pos pt2 = {2, 0};
    char code_pt1[100];
    char code_pt2[100];
    while (std::getline(std::cin, input)) {
        for (char c : input) {
            switch (c) {
            case 'U':
                if (pt1.row > 0) {
                    pt1.row -= 1;
                }
                if (pt2.row > 0 && kp2[pt2.row - 1][pt2.col] != 0) {
                    pt2.row -= 1;
                }
                break;
            case 'R':
                if (pt1.col < 2) {
                    pt1.col += 1;
                }
                if (pt2.col < 4 && kp2[pt2.row][pt2.col + 1] != 0) {
                    pt2.col += 1;
                }
                break;
            case 'D':
                if (pt1.row < 2) {
                    pt1.row += 1;
                }

                if (pt2.row < 4 && kp2[pt2.row + 1][pt2.col] != 0) {
                    pt2.row += 1;
                }
                break;
            case 'L':
                if (pt1.col > 0) {
                    pt1.col -= 1;
                }

                if (pt2.col > 0 && kp2[pt2.row][pt2.col - 1] != 0) {
                    pt2.col -= 1;
                }
                break;
            }
        }

        code_pt1[index] = kp1[pt1.row][pt1.col];
        code_pt2[index] = kp2[pt2.row][pt2.col];
        index++;
    }
    code_pt1[index] = 0;
    code_pt2[index] = 0;

    std::cout << "--- Day 2: Bathroom Security ---\n";
    std::cout << "Part 1: " << code_pt1 << "\n";
    std::cout << "Part 2: " << code_pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
