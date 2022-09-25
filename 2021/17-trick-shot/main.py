from dataclasses import dataclass
import sys 
import parse 
from dataclasses import dataclass 
import math 
def parse_input(input):
    pattern = "target area: x={x1:d}..{x2:d}, y={y1:d}..{y2:d}"
    match = parse.search(pattern, input)
    return match.named

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


def solve(target):
    distinct_velocity_values = 0
    x_limit = max(abs(target['x1']), abs(target['x2'])) + 1
    y_limit = max(abs(target['y1']), abs(target['y2'])) + 1
    step_limit = 2 * y_limit

    for x in range(-x_limit, x_limit):
        for y in range(-y_limit, y_limit):
            probe = Probe(0, 0, Velocity(x, y))
            for _ in range(step_limit):
                probe.move()

                if target['x1'] <= probe.x <= target['x2'] and target['y1'] <= probe.y <= target['y2']:
                    distinct_velocity_values += 1
                    break
                elif abs(probe.x) > max(abs(target['x1']), abs(target['x2'])):
                    # overshot
                    break
                elif probe.y < target['y1']:
                    # gravity baby
                    break

    return distinct_velocity_values

if __name__ == '__main__':
    for path in sys.argv[1:]:
        with open(path) as f:
            input = f.read().strip()
            data = parse_input(input)
            print(solve(data))
