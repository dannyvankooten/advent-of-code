#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using std::find;
using std::priority_queue;
using std::vector;

enum atom_type {
  THULIUM,
  PLUTONIUM,
  STRONTIUM,
  PROMETHIUM,
  RUTHENIUM,

  // from test case
  HYDROGEN,
  LITHIUM,
};

enum object_type {
  MICROCHIP,
  GENERATOR,
};

struct Object {
  object_type type;
  atom_type atom;

  bool operator==(const Object& rhs) const {
    return type == rhs.type && atom == rhs.atom;
  };
};

struct State {
  int steps;
  int el;
  vector<vector<Object>> floors;

  bool is_valid() {
    bool valid = true;

    // check floor below, current and above
    // TODO: we can improve this by only checking previous and current floor
    for (int fl = std::max(el - 1, 0); fl < std::min(4, el + 1); fl++) {
      // no microchip should be on same floor as generator
      // unless connected to its own generator
      for (const auto& a : floors[fl]) {
        if (a.type != MICROCHIP) {
          continue;
        }

        // if generator is here too, we're good
        for (const auto& b : floors[fl]) {
          if (b.type != GENERATOR) {
            continue;
          }

          // generator of same type, good
          if (b.atom == a.atom) {
            valid = true;
            break;
          }

          // don't return yet because
          // floor may still contain a generator of same atom as microchip
          valid = false;
        }
      }

      if (!valid) {
        return false;
      }
    }

    return valid;
  };

  unsigned long long hash() const {
    unsigned long long hash = 0;
    // position is irrelevant
    // floor level + object combo's is
    // 14 objects, 4 floors
    // so can represent in 4*14 bits
    for (unsigned long long i = 0; i < 4; i++) {

      for (const auto& obj : floors[i]) {
        unsigned long long oi = (obj.type * 7) + obj.atom;
        // flip bit at i * oi
        hash |= (1 << (i * 14 + oi));
      }
    }

    return hash;
  };

  bool operator<(const State& rhs) const { return steps > rhs.steps; };
};

vector<vector<Object>> create_pairs(const vector<Object>& in) {
  vector<vector<Object>> out;

  for (auto i = in.begin(); i != in.end(); i++) {
    // one object
    out.push_back(vector<Object>{*i});

    for (auto j = i + 1; j != in.end(); j++) {
      out.push_back(vector<Object>{*i, *j});
    }
  }

  return out;
}

// 589 too high
// 1317 too high

int dijkstra(const vector<vector<Object>>& floors) {
  auto endsize =
      floors[0].size() + floors[1].size() + floors[2].size() + floors[3].size();

  std::cout << "upper floor should contain " << endsize << " objects"
            << std::endl;

  std::unordered_map<unsigned long long, bool> seen;
  priority_queue<State> q;
  q.push({0, 0, floors});

  // hash of initial state:

  while (!q.empty()) {
    auto u = q.top();
    q.pop();

    // mark as seen
    seen[u.hash()] = true;

    if (u.steps > 589) {
      std::cout << "At 589 steps without an answer... :(" << std::endl;
      return -1;
    }

    // check if done
    if (u.floors[3].size() == endsize) {
      return u.steps;
    }

    // for every object, move it down AND up
    vector<vector<Object>> pairs = create_pairs(u.floors[u.el]);
    // std::cout << "generated " << pairs.size() << " object pairs" << std::endl;
    for (const auto& pair : pairs) {
      for (int t = u.el - 1; t <= u.el + 1; t += 2) {
        if (t < 0 || t > 3) {
          continue;
        }

        State v = u;
        for (const Object& obj : pair) {
          auto old = find(v.floors[v.el].begin(), v.floors[v.el].end(), obj);
          // std::cout << "removing object at position" << std::endl;
          v.floors[t].push_back(obj);
          v.floors[v.el].erase(old);
        }
        v.steps += 1;
        v.el = t;
        if (v.is_valid() && !seen.contains(v.hash())) {
          q.push(v);
        }
      }
    }

    // break;
  }

  return -1;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  vector<vector<Object>> floors(4);
  // floors[0].push_back(Object{MICROCHIP, HYDROGEN});
  // floors[0].push_back(Object{MICROCHIP, LITHIUM});
  // floors[1].push_back(Object{GENERATOR, HYDROGEN});
  // floors[2].push_back(Object{GENERATOR, LITHIUM});

  floors[0].push_back({GENERATOR, THULIUM});
  floors[0].push_back({MICROCHIP, THULIUM});
  floors[0].push_back({GENERATOR, PLUTONIUM});
  floors[0].push_back({GENERATOR, STRONTIUM});

  floors[1].push_back({MICROCHIP, PLUTONIUM});
  floors[1].push_back({MICROCHIP, STRONTIUM});

  floors[2].push_back({GENERATOR, PROMETHIUM});
  floors[2].push_back({MICROCHIP, PROMETHIUM});
  floors[2].push_back({GENERATOR, RUTHENIUM});
  floors[2].push_back({MICROCHIP, RUTHENIUM});

  pt1 = dijkstra(floors);

  std::cout << "--- Day 11: Radioisotope Thermoelectric Generators ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";

  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << duration.count() << " μs"
            << "\n";
  return EXIT_SUCCESS;
}
