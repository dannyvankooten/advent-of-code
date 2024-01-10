#include <chrono>
#include <iostream>

void shift_row(char grid[6][50], int r, int n) {
    char copy[50];
    for (int c = 0; c < 50; c++) {
        copy[c] = grid[r][c];
    }

    for (int c = 0; c < 50; c++) {
        int src = c - n;
        if (src < 0) {
            src += 50;
        }
        grid[r][c] = copy[src];
    }
}

void shift_column(char grid[6][50], int c, int n) {
    char copy[6];
    for (int r = 0; r < 6; r++) {
        copy[r] = grid[r][c];
    }

    for (int r = 0; r < 6; r++) {
        int src = r - n;
        if (src < 0) {
            src += 6;
        }
        grid[r][c] = copy[src];
    }
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;

    std::string line;
    char grid[6][50] = {{0}};
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 50; c++) {
            grid[r][c] = 0;
        }
    }

    while (std::getline(std::cin, line)) {
        if (line.compare(0, 4, "rect") == 0) {
            int i = 5;
            int width = std::stoi(&line[i]);
            while (line[i - 1] != 'x') {
                i++;
            }
            int height = std::stoi(&line[i]);
            for (int r = 0; r < height; r++) {
                for (int c = 0; c < width; c++) {
                    grid[r][c] = 1;
                }
            }
        } else {
            char orientation = line[7];
            int i = 7;
            while (line[i] < '0' || line[i] > '9') {
                i++;
            }
            int src = std::stoi(&line[i]);
            i += 4;
            while (line[i] < '0' || line[i] > '9') {
                i++;
            }
            int amount = std::stoi(&line[i]);

            if (orientation == 'c') {
                shift_column(grid, src, amount);
            } else {
                shift_row(grid, src, amount);
            }
        }
    }

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 50; c++) {
            pt1 += grid[r][c];
        }
    }

    std::cout << "--- Day 8: Two-Factor Authentication ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2:\n";
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 50; c++) {
            pt1 += grid[r][c];
            std::cout << (grid[r][c] ? '#' : ' ');
        }
        std::cout << "\n";
    }

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
