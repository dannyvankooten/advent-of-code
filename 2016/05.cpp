#include <chrono>
#include <iomanip>
#include <iostream>

#include <openssl/md5.h>

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

int main() {
    auto tstart = std::chrono::high_resolution_clock::now();

    std::string input;
    std::getline(std::cin, input);

    char pt1[9] = {0};
    char pt2[9] = {0};

    int pi = 0;
    int pl = 0;

    for (int i = 0;; i++) {
        std::string s = input + std::to_string(i);
        std::string r = md5(s);

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
