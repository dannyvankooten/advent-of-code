#include <chrono>
#include <climits>
#include <iostream>
#include <regex>
#include <vector>

using std::stoi;
using std::string;
using std::vector;

struct Point3D {
  int x, y, z;

  int manhattan() const noexcept { return abs(x) + abs(y) + abs(z); }
  bool operator==(const Point3D &o) const noexcept {
    return x == o.x && y == o.y && z == o.z;
  }
  void operator+=(Point3D &o) noexcept {
    x += o.x;
    y += o.y;
    z += o.z;
  }
};

struct Particle {
  Point3D position;
  Point3D velocity;
  Point3D acceleration;

  void update() {
    velocity += acceleration;
    position += velocity;
  }
};

vector<Particle> parse_input() {
  std::string input;
  std::regex rx(
      "^p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, "
      "a=<(-?\\d+),(-?\\d+),(-?\\d+)>$");
  std::smatch matches;
  vector<Particle> particles;
  while (std::getline(std::cin, input)) {
    if (!std::regex_match(input, matches, rx)) {
      std::cerr << "Unexpected line: " << input << std::endl;
      break;
    }

    particles.push_back({
        {stoi(matches[1]), stoi(matches[2]), stoi(matches[3])}, // position
        {stoi(matches[4]), stoi(matches[5]), stoi(matches[6])}, // velocity
        {stoi(matches[7]), stoi(matches[8]), stoi(matches[9])}  // acceleration
    });
  }

  return particles;
}

int main() {
  auto tstart = std::chrono::high_resolution_clock::now();
  vector<Particle> particles = parse_input();

  // part 1
  // point with smallest acceleration manhatten
  // will be point closest to 0,0,0 in long term
  size_t pt1 = 0;
  int min = INT_MAX;
  for (size_t i = 0; i < particles.size(); i++) {
    int mh = particles[i].acceleration.manhattan();
    if (mh < min) {
      min = mh;
      pt1 = i;
    }
  }

  // for part 2
  // try brute-force?
  vector<bool> collided(particles.size());
  vector<size_t> collisions;
  for (size_t t = 0; t < 40; t++) {
    for (Particle &p : particles) {
      p.update();
    }

    // check each particle against all other
    // and remove the ones that  collide
    for (size_t a = 0; a < particles.size(); a++) {
      if (collided[a]) {
        continue;
      }

      for (size_t b = a + 1; b < particles.size(); b++) {
        if (collided[b]) {
          continue;
        }

        if (particles[a].position == particles[b].position) {
          collisions.push_back(a);
          collisions.push_back(b);
        }
      }
    }

    // we mark particle as collided after checking for all collissions
    for (const size_t p : collisions) {
      collided[p] = true;
    }

    collisions.clear();
  }

  auto pt2 = std::count(collided.begin(), collided.end(), 0);

  std::cout << "--- Day 20: Particle Swarm ---\n";
  std::cout << "Part 1: " << pt1 << "\n";
  std::cout << "Part 2: " << pt2 << "\n";
  auto tstop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(tstop - tstart);
  std::cout << "Time: " << (static_cast<double>(duration.count()) / 1000.0)
            << " ms"
            << "\n";
  return EXIT_SUCCESS;
}
