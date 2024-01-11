#include <chrono>
#include <iomanip>
#include <iostream>

#include <openssl/md5.h>
#include <string>

using std::string;

std::string md5(const std::string &str) {
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}

int solve(string input, int n) {
    // generate first 1000 hashes
    std::string hashes[1000];
    for (int i = 0; i < 1000; i++) {
        std::string s = input + std::to_string(i);
        for (int j = 0; j < n; j++) {
            s = md5(s);
        }

        hashes[i] = s;
    }

    int nkeys = 0;
    for (int i = 0;; i++) {
        // pick first hash in deque
        std::string h = hashes[i % 1000];

        // generate next hash (md5 n times)
        std::string s = input + std::to_string(i + 1000);
        for (int j = 0; j < n; j++) {
            s = md5(s);
        }
        hashes[i % 1000] = s;

        // if this hash contains 3 repeating chars, add to sequences
        std::string seq;
        for (int j = 0; j < h.length() - 2; j++) {
            if (h[j] == h[j + 1] && h[j] == h[j + 2]) {
                seq = std::string(5, h[j]);
                break;
            }
        }

        // go through next 1000 hashes to find matching
        if (seq != "") {
            for (auto n : hashes) {
                if (n.find(seq) != string::npos) {
                    if (++nkeys == 64) {
                        return i;
                    }

                    break;
                }
            }
        }
    }

    /* unreachable */
    __builtin_unreachable();
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();
    int pt1 = 0;
    int pt2 = 0;

    std::string input;
    std::getline(std::cin, input);

    pt1 = solve(input, 1);
    pt2 = solve(input, 2017);

    std::cout << "--- Day 14: One-Time Pad ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
