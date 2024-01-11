#include <chrono>
#include <iostream>
#include <unordered_map>

enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
};

int directions[4][2]{
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt2 = -1;
    int x = 0;
    int y = 0;
    int dir = NORTH;

    std::string input;
    std::getline(std::cin, input);

    std::unordered_map<int, bool> m = {};

    int i = 0;
    while (true) {
        if (input[i++] == 'R') {
            dir = (dir == 3) ? 0 : dir + 1;
        } else {
            dir = (dir == 0) ? 3 : dir - 1;
        }

        int amount = input[i++] - '0';
        while (input[i] >= '0' && input[i] <= '9') {
            amount = (amount * 10) + (input[i++] - '0');
        }

        while (amount > 0) {
            x += directions[dir][0];
            y += directions[dir][1];

            // check if we've been at this x, y before
            if (pt2 == -1 && m.count((x << 16) + y) > 0) {
                pt2 = manhattan_distance(x, y, 0, 0);
            }

            m.insert({(x << 16) + y, true});

            amount--;
        }

        if (input[i] == ',') {
            i += 2;
        } else {
            break;
        }
    }

    int pt1 = manhattan_distance(x, y, 0, 0);

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
