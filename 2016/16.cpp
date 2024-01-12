#include <chrono>
#include <iostream>

using std::string;

// mutate original according to puzzle rules
// length of returned string is twice the input + 1
void step(string &a) {
    size_t l = a.size();
    a.resize(l * 2 + 1);
    a[l] = '0';

    for (long i = l - 1, j = l + 1; i >= 0; i--, j++) {
        a[j] = a[i] == '1' ? '0' : '1';
    }
}

// create odd-length checksum for input string
void checksum(string &a) {
    for (size_t i = 0, j = 0; i < a.length() - 1; i += 2, j += 1) {
        a[j] = a[i] == a[i + 1] ? '1' : '0';
    }

    a.resize(a.length() / 2);
    if (a.length() % 2 == 0) {
        return checksum(a);
    }
}

string fill_disk_of_size(std::string input, size_t sz) {
    string state = input;
    input.reserve(sz * 2);
    while (state.length() < sz) {
        step(state);
    }
    state.resize(sz);
    checksum(state);
    return state;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();

    string input;
    std::getline(std::cin, input);

    string pt1 = fill_disk_of_size(input, 272);
    string pt2 = fill_disk_of_size(input, 35651584);

    std::cout << "--- Day 16: Dragon Checksum ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
