#include <chrono>
#include <iostream>

size_t decompress(std::string input, size_t size, bool recurse) {
    size_t length = 0;

    for (size_t i = 0; i < size; i++) {
        if (input[i] != '(') {
            length += 1;
            continue;
        }

        // at (, parse encoding
        int start = i;
        int pattern_length = std::stoi(&input[i + 1]);
        while (input[i - 1] != 'x') {
            i++;
        }
        int pattern_repeats = std::stoi(&input[i]);
        while (input[i - 1] != ')') {
            i++;
        }

        // subtract encoding block from length, eg (1x10)
        // length -= (i - start);

        // add decompressed size to length
        if (recurse) {
            length +=
                decompress(&input[i], pattern_length, true) * pattern_repeats;
        } else {
            length += pattern_length * pattern_repeats;
        }

        // skip decompressed block
        i += pattern_length - 1;
    }

    return length;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    size_t pt1 = 0;
    size_t pt2 = 0;

    std::string input;
    std::getline(std::cin, input);

    // start at original size
    pt1 = decompress(input, input.length(), false);
    pt2 = decompress(input, input.length(), true);

    std::cout << "--- Day 9: Explosives in Cyberspace ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
