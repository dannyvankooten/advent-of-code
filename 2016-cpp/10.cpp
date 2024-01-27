#include <cctype>
#include <chrono>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::vector;

struct Bot {
  vector<int>* targets[2];
  vector<int> chips;
};

struct Bin {
  vector<int> chips;
};

vector<int> parse_ints_in_line(const string& line) {
  vector<int> r;

  for (size_t i = 0; i < line.length(); i++) {
    if (!std::isdigit(line[i])) {
      continue;
    }

    // at number, parse it into vec
    r.push_back(std::stoi(&line[i]));

    // skip
    while (line[i] >= '0' && line[i] <= '9') {
      i++;
    }
  }
  return r;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  std::unordered_map<int, struct Bot> bots;
  std::unordered_map<int, struct Bin> output_bins;
  std::string input;
  std::regex rx(
      "^bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)$");
  std::smatch matches;
  while (std::getline(std::cin, input)) {
    if (input[0] == 'v') {
      vector nums = parse_ints_in_line(input);
      bots[nums[1]].chips.push_back(nums[0]);
    } else {
      std::regex_match(input, matches, rx);

      int b = std::stoi(matches[1].str());
      int tlow = std::stoi(matches[3].str());
      int thigh = std::stoi(matches[5].str());

      if (matches[2].compare("output") == 0) {
        bots[b].targets[0] = &output_bins[tlow].chips;
      } else {
        bots[b].targets[0] = &bots[tlow].chips;
      }

      if (matches[4].compare("output") == 0) {
        bots[b].targets[1] = &output_bins[thigh].chips;
      } else {
        bots[b].targets[1] = &bots[thigh].chips;
      }
    }
  }

  while (true) {
    int actions = 0;
    for (auto& [id, bot] : bots) {
      if (bot.chips.size() == 2) {
        int low = std::min(bot.chips[0], bot.chips[1]);
        int high = std::max(bot.chips[0], bot.chips[1]);
        bot.chips.clear();

        if (low == 17 && high == 61) {
          pt1 = id;
        }

        bot.targets[0]->push_back(low);
        bot.targets[1]->push_back(high);
        actions++;
      }
    }

    // stop if in a stable state (all chips have been distributed)
    if (actions == 0) {
      break;
    }
  }

  pt2 = output_bins[0].chips[0] * output_bins[1].chips[0] *
        output_bins[2].chips[0];

  std::cout << "--- Day 10: Balance Bots ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
