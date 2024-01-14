#include <openssl/md5.h>
#include <chrono>
#include <iostream>
#include <queue>

using std::priority_queue;
using std::string;
using std::vector;

struct Point {
  int col;
  int row;

  bool oob() {
    return this->col < 0 || this->row < 0 || this->col >= 4 || this->row >= 4;
  }

  inline bool operator==(const Point& other) const {
    return this->col == other.col && this->row == other.row;
  }

  Point operator+(const Point& other) const {
    return Point{
        this->col + other.col,
        this->row + other.row,
    };
  }
};

struct Vertex {
  Point pos;
  string path;

  constexpr bool operator()(Vertex const& a, Vertex const& b) const noexcept {
    return a.path.length() > b.path.length();
  }
};

char dirnames[4] = {'U', 'D', 'L', 'R'};
Point directions[4] = {
    {-1, 0},  // U
    {1, 0},   // D
    {0, -1},  // L
    {0, 1},   // R
};

string md5(const string& str) {
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

string dijkstra(const string& passcode) {
  Point start = Point{0, 0};
  Point end = Point{3, 3};

  priority_queue<Vertex, vector<Vertex>, Vertex> q;
  q.push(Vertex{start, ""});
  while (!q.empty()) {
    Vertex v = q.top();
    q.pop();

    if (v.pos == end) {
      return v.path;
    }

    string h = md5(passcode + v.path);

    for (int i = 0; i < 4; i++) {
      // door is not open
      if (!(h[i] >= 'b' && h[i] <= 'f')) {
        continue;
      }

      // target position is out of bounds
      Point n = v.pos + directions[i];
      if (n.oob()) {
        continue;
      }

      q.push(Vertex{n, v.path + dirnames[i]});
    }
  }

  return "";
}

// longest path is NP-hard
// so we just brute force our way through
size_t longest_path(const string& passcode, Vertex v) {
  if (v.pos == Point{3, 3}) {
    return v.path.length();
  }

  std::string::size_type longest = 0;
  string h = md5(passcode + v.path);

  for (int i = 0; i < 4; i++) {
    // door is not open
    if (!(h[i] >= 'b' && h[i] <= 'f')) {
      continue;
    }

    // target position is out of bounds
    Point n = v.pos + directions[i];
    if (n.oob()) {
      continue;
    }

    std::string::size_type length =
        longest_path(passcode, Vertex{n, v.path + dirnames[i]});
    if (length > longest) {
      longest = length;
    }
  }

  return longest;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();

  std::string input;
  std::getline(std::cin, input);

  string pt1 = dijkstra(input);
  int pt2 = longest_path(input, Vertex{Point{0, 0}, ""});

  std::cout << "--- Day 17: Two Steps Forward ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
