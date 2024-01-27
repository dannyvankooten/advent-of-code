from dataclasses import dataclass
from pathlib import Path
import sys 
from dataclasses import dataclass 
import math 

def parse_input(input):
    input = input[len("target area: "):]
    parts = input.split(', ')
    parts[0] = parts[0][2:]
    parts[1] = parts[1][2:]

    x = [int(n) for n in parts[0].split('..')]
    y = [int(n) for n in parts[1].split('..')]
    return x, y

@dataclass
class Velocity:
    __slots__ = ['x', 'y']
    x: int 
    y: int 

@dataclass
class Probe:
    __slots__ = ['x', 'y', 'velocity']
    x: int 
    y: int 
    velocity: Velocity

    def move(self):
        self.x += self.velocity.x
        self.y += self.velocity.y

        if self.velocity.x < 0:
            self.velocity.x = min(0, self.velocity.x + 1)
        else:
            self.velocity.x = max(0, self.velocity.x - 1)

        self.velocity.y -= 1


def solve(xt, yt):
    distinct_velocity_values = 0
    x_limit = max(abs(xt[0]), abs(xt[1])) + 1
    y_limit = max(abs(yt[0]), abs(yt[1])) + 1
    step_limit = 2 * y_limit

    for x in range(-x_limit, x_limit):
        for y in range(-y_limit, y_limit):
            probe = Probe(0, 0, Velocity(x, y))
            for _ in range(step_limit):
                probe.move()

                if xt[0] <= probe.x <= xt[1] and yt[0] <= probe.y <= yt[1]:
                    distinct_velocity_values += 1
                    break
                elif abs(probe.x) > max(abs(xt[0]), abs(xt[1])):
                    # overshot
                    break
                elif probe.y < yt[0]:
                    # gravity baby
                    break

    return distinct_velocity_values

if __name__ == '__main__':
    print("--- Day 17: Trick Shot ---")
    input = Path("input.txt").read_text().strip()
    x, y  = parse_input(input)

    pt2 = solve(x, y)
    print("Part 2: ", pt2, pt2 == 3229)