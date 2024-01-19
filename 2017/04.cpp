#include <cctype>
#include <chrono>
#include <iostream>
#include <string_view>
#include <unordered_set>

using std::string;
using std::string_view;
using std::unordered_set;

int character_value(const string_view &word) {
  int value = 1;
  for (const char ch : word) {
    value *= (ch - 'a' + 1);
  }
  return value;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  string input;
  unordered_set<std::string_view> words;
  unordered_set<int> anagrams;

  while (std::getline(std::cin, input)) {
    words.clear();
    anagrams.clear();

    bool valid = true;
    bool valid_pt2 = true;
    string::iterator it = input.begin();
    string::iterator lineend = input.end();

    while (valid || valid_pt2) {
      string::iterator wordstart = it;
      while (it != lineend && !std::isspace(*it)) {
        it++;
      }

      string_view word(wordstart, it);
      valid = valid && !words.contains(word);

      // TODO: We could get rid of the allocation here
      // By multiplying each character with its ordinal value and summing the
      // result string anagram(word); sort(anagram.begin(), anagram.end());
      int anagram = character_value(word);
      valid_pt2 = valid_pt2 && !anagrams.contains(anagram);

      if (it == lineend) {
        break;
      }

      it++;
      words.insert(word);
      anagrams.insert(anagram);
    }

    pt1 += valid;
    pt2 += valid_pt2;
  }

  std::cout << "--- Day 4: High-Entropy Passphrases ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
