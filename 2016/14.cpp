#include <chrono>
#include <iostream>

#include <openssl/md5.h>
#include <string>

using std::string;

string md5(const string &str) {
    static unsigned char hash[MD5_DIGEST_LENGTH];
    static const char characters[] = "0123456789abcdef";

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    // faster method of converting to hexadecimal
    string ret;
    ret.reserve(MD5_DIGEST_LENGTH * 2);
    auto buf = std::back_inserter(ret);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        auto byte = hash[i];
        *buf++ = characters[byte >> 4];
        *buf++ = characters[byte & 0x0F];
    }

    return ret;
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
        for (size_t j = 0; j < h.length() - 2; j++) {
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
