#include <chrono>
#include <iostream>

#include <openssl/md5.h>

std::string md5(const std::string &str) {
    static unsigned char hash[MD5_DIGEST_LENGTH];
    static const char characters[] = "0123456789abcdef";

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    // faster method of converting to hexadecimal
    std::string ret;
    ret.reserve(4 * 2);
    auto buf = std::back_inserter(ret);

    for (int i = 0; i < 4; i++) {
        auto byte = hash[i];
        *buf++ = characters[byte >> 4];
        *buf++ = characters[byte & 0x0F];
    }

    return ret;
}

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();

    std::string input;
    std::getline(std::cin, input);

    char pt1[9] = {0};
    char pt2[9] = {0};

    int pi = 0;
    int pl = 0;

    for (int i = 0;; i++) {
        std::string r = md5(input + std::to_string(i));

        if (r.compare(0, 5, "00000") == 0) {
            if (pi < 8) {
                pt1[pi++] = r[5];
            }

            int pi2 = r[5] - '0';
            if (pl < 8 && pi2 >= 0 && pi2 < 8 && pt2[pi2] == 0) {
                pt2[r[5] - '0'] = r[6];
                pl += 1;
            }
        }

        if (pi >= 8 && pl >= 8) {
            break;
        }
    }

    std::cout << "--- Day 5: How About a Nice Game of Chess? ---\n";
    std::cout << "Part 1: " << pt1 << "\n";
    std::cout << "Part 2: " << pt2 << "\n";

    auto tstop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
    std::cout << "Time: " << duration.count() << " μs"
              << "\n";
    return EXIT_SUCCESS;
}
