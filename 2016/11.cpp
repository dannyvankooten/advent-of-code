#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <queue>
#include <unordered_map>

using std::priority_queue;
using std::unordered_map;

enum atom_type {
  // part 1
  THULIUM,
  PLUTONIUM,
  STRONTIUM,
  PROMETHIUM,
  RUTHENIUM,

  // part 2
  // ELERIUM,
  // DILITHIUM,

  // needs to be last enumeration element
  NATOMS,
};

enum object_type {
  MICROCHIP,
  GENERATOR,
  NTYPES,
};

static const uint64_t NOBJECTS = (uint64_t)NATOMS * (uint64_t)NTYPES;

static inline uint64_t hash_state(uint64_t el, uint64_t layout) {
  return ((el << (NOBJECTS * 3)) | layout);
}

static inline uint64_t get_floor(uint64_t layout, uint64_t i) {
  return (layout >> (i * 3)) & 0b111;
};

static inline uint64_t set_floor(uint64_t layout, uint64_t i, uint64_t floor) {
  return (layout & ~(0b111 << (i * 3))) | (floor << (i * 3));
};

static inline uint64_t object_id(object_type t, atom_type a) {
  return (uint64_t)a + (uint64_t)NATOMS * (uint64_t)t;
}

static uint64_t add_object(uint64_t layout, object_type t, atom_type a,
                           uint64_t floor) {
  return set_floor(layout, object_id(t, a), floor);
}

static uint64_t get_object(uint64_t layout, object_type t, atom_type a) {
  return get_floor(layout, object_id(t, a));
}

bool is_valid_layout(uint64_t layout) {
  // microchips are at index 0 - NATOMS
  for (uint64_t i = 0; i < NATOMS; i++) {
    uint64_t floor = get_floor(layout, i);

    // ignore everything on floor 0
    if (floor == 0) {
      continue;
    }

    // always safe if microchip is coupled to generator of same atom
    if (get_floor(layout, i + NATOMS) == floor) {
      continue;
    }

    // unsafe if any other generator on same floor
    for (uint64_t j = NATOMS; j < NOBJECTS; j++) {
      if (get_floor(layout, j) == floor) {
        return false;
      }
    }
  }

  return true;
};

struct State {
  uint64_t steps;
  uint64_t el;
  uint64_t layout;

  bool operator<(const State& rhs) const {
    return steps > rhs.steps || (steps == rhs.steps && el < rhs.el);
  };
};

int dijkstra(uint64_t _layout) {
  uint64_t ready = 0;
  for (uint64_t i = 0; i < NOBJECTS; i++) {
    if (get_floor(_layout, i) > 0) {
      ready = set_floor(ready, i, 4);
    }
  }

  unordered_map<uint64_t, bool> seen;
  priority_queue<State> q;
  q.push(State{0, 1, _layout});


  // hash of initial state:
  while (!q.empty()) {
    auto u = q.top();
    q.pop();

    // check if done
    if (u.layout == ready) {
      return (int)u.steps;
    }

    if (seen[hash_state(u.el, u.layout)]) {
      continue;
    }

    // mark elevator level + layout as seen
    // TODO: We can prune MUCH more aggressively here since pairs are interchangable
    seen[hash_state(u.el, u.layout)] = true;

    // generate valid next states
    for (uint64_t i = 0; i < NOBJECTS; i++) {

      // only consider objects on same floor as elevator
      if (get_floor(u.layout, i) != u.el) {
        continue;
      }

      // move (single) object down
      if (u.el > 1) {
        uint64_t el = u.el - 1;
        uint64_t layout = set_floor(u.layout, i, el);
        if (is_valid_layout(layout)) {
          q.push(State{u.steps + 1, el, layout});
        }
      }

      // move single object up
      if (u.el < 4) {
        uint64_t el = u.el + 1;
        uint64_t layout = set_floor(u.layout, i, el);
        if (is_valid_layout(layout)) {
          q.push(State{u.steps + 1, el, layout});
        }
      }

      // pair with every other object on this floor
      for (uint64_t j = i + 1; j < NOBJECTS; j++) {
        if (get_floor(u.layout, j) != u.el) {
          continue;
        }

        // move objects down
        if (u.el > 1) {
          uint64_t el = u.el - 1;
          uint64_t layout = u.layout;
          layout = set_floor(layout, i, el);
          layout = set_floor(layout, j, el);

          if (is_valid_layout(layout)) {
            q.push(State{u.steps + 1, el, layout});
          }
        }

        // move objects up
        if (u.el < 4) {
          uint64_t el = u.el + 1;
          uint64_t layout = u.layout;
          layout = set_floor(layout, i, el);
          layout = set_floor(layout, j, el);

          if (is_valid_layout(layout)) {
            q.push(State{u.steps + 1, el, layout});
          }
        }
      }
    }

  }

  return -1;
}

void run_tests();

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  int pt1 = 0;
  int pt2 = 0;

  run_tests();

  uint64_t layout = 0;
  layout = add_object(layout, GENERATOR, THULIUM, 1);
  layout = add_object(layout, MICROCHIP, THULIUM, 1);
  layout = add_object(layout, GENERATOR, PLUTONIUM, 1);
  layout = add_object(layout, GENERATOR, STRONTIUM, 1);
  layout = add_object(layout, MICROCHIP, PLUTONIUM, 2);
  layout = add_object(layout, MICROCHIP, STRONTIUM, 2);
  layout = add_object(layout, GENERATOR, PROMETHIUM, 3);
  layout = add_object(layout, MICROCHIP, PROMETHIUM, 3);
  layout = add_object(layout, GENERATOR, RUTHENIUM, 3);
  layout = add_object(layout, MICROCHIP, RUTHENIUM, 3);
  pt1 = dijkstra(layout);

  // layout = add_object(layout, GENERATOR, ELERIUM, 1);
  // layout = add_object(layout, MICROCHIP, ELERIUM, 1);
  // layout = add_object(layout, GENERATOR, DILITHIUM, 1);
  // layout = add_object(layout, MICROCHIP, DILITHIUM, 1);
  // pt2 = dijkstra(layout);

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

void run_tests() {
  uint64_t layout = 0;
  layout = add_object(layout, MICROCHIP, THULIUM, 1);    // HM, idx: 0
  layout = add_object(layout, MICROCHIP, STRONTIUM, 1);  // LM, idx: 1
  layout = add_object(layout, GENERATOR, THULIUM, 2);    // HG, idx: 2
  layout = add_object(layout, GENERATOR, STRONTIUM, 3);  // LG, idx: 3
  assert(is_valid_layout(layout) == true);

  const auto HM = object_id(MICROCHIP, THULIUM);
  const auto LM = object_id(MICROCHIP, STRONTIUM);
  const auto HG = object_id(GENERATOR, THULIUM);
  const auto LG = object_id(GENERATOR, STRONTIUM);

  assert(get_object(layout, MICROCHIP, THULIUM) == 1);
  assert(get_object(layout, MICROCHIP, STRONTIUM) == 1);
  assert(get_object(layout, GENERATOR, THULIUM) == 2);
  assert(get_object(layout, GENERATOR, STRONTIUM) == 3);
  assert(get_floor(layout, HM) == 1);
  assert(get_floor(layout, LM) == 1);
  assert(get_floor(layout, HG) == 2);
  assert(get_floor(layout, LG) == 3);

  // 1: move HM to floor 2
  layout = set_floor(layout, HM, 2);
  assert(get_floor(layout, HM) == 2);
  assert(is_valid_layout(layout) == true);

  // 2: move HM and HG to floor 3
  layout = set_floor(layout, HM, 3);
  layout = set_floor(layout, HG, 3);
  assert(get_floor(layout, HM) == 3);
  assert(get_floor(layout, HG) == 3);
  assert(is_valid_layout(layout) == true);

  // 3: move HM back to floor 2
  layout = set_floor(layout, HM, 2);
  assert(get_floor(layout, HM) == 2);
  assert(is_valid_layout(layout) == true);

  // 4: move HM back to floor 1
  layout = set_floor(layout, HM, 1);
  assert(get_floor(layout, HM) == 1);
  assert(is_valid_layout(layout) == true);

  // 5: move HM and LM to floor 2
  layout = set_floor(layout, HM, 2);
  layout = set_floor(layout, LM, 2);
  assert(get_floor(layout, HM) == 2);
  assert(get_floor(layout, LM) == 2);
  assert(is_valid_layout(layout) == true);

  // 6: move HM and LM to floor 3
  layout = set_floor(layout, HM, 3);
  layout = set_floor(layout, LM, 3);
  assert(get_floor(layout, HM) == 3);
  assert(get_floor(layout, LM) == 3);
  assert(is_valid_layout(layout) == true);

  // 7: move HM and LM to floor 4
  layout = set_floor(layout, HM, 4);
  layout = set_floor(layout, LM, 4);
  assert(is_valid_layout(layout) == true);

  // 8: move HM to floor 3
  layout = set_floor(layout, HM, 3);
  assert(get_floor(layout, HM) == 3);

  assert(is_valid_layout(layout) == true);

  // 9: move both generators to floor 4
  layout = set_floor(layout, HG, 4);
  assert(get_floor(layout, HG) == 4);

  layout = set_floor(layout, LG, 4);
  assert(get_floor(layout, LG) == 4);

  assert(is_valid_layout(layout) == true);

  // 10: move LM to floor 3
  layout = set_floor(layout, LM, 3);
  assert(is_valid_layout(layout) == true);

  // 11: HM and LM to floor 4
  layout = set_floor(layout, HM, 4);
  layout = set_floor(layout, LM, 4);
  assert(is_valid_layout(layout) == true);

  uint64_t ready = 0;
  ready = set_floor(ready, HM, 4);
  ready = set_floor(ready, LM, 4);
  ready = set_floor(ready, HG, 4);
  ready = set_floor(ready, LG, 4);
  assert(layout == ready);

  layout = 0;
  layout = add_object(layout, MICROCHIP, THULIUM, 1);
  layout = add_object(layout, MICROCHIP, STRONTIUM, 1);
  layout = add_object(layout, GENERATOR, THULIUM, 2);
  layout = add_object(layout, GENERATOR, STRONTIUM, 3);
  assert(dijkstra(layout) == 11);
}
