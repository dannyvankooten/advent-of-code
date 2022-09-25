from dataclasses import dataclass
import sys 
import parse 
from dataclasses import dataclass 

def parse_input(input):
    pattern = "target area: x={x1:d}..{x2:d}, y={y1:d}..{y2:d}"
    match = parse.search(pattern, input)
    return match.named

@dataclass
class Velocity:
    x: int 
    y: int 

@dataclass
class Probe:
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

    x_dir = 1 if target['x1'] > 0 else -1
    distinct_velocity_values = 0

    for x in range(x_dir, target['x2'] * 2, x_dir):
        for y in range(-3 * abs(target['y2']), abs(target['y2'] * 3)):
            probe = Probe(0, 0, Velocity(x, y))
            for step in range(target['x1'] * 5):
                probe.move()

                if target['x1'] <= probe.x <= target['x2'] and target['y1'] <= probe.y <= target['y2']:
                    distinct_velocity_values += 1
                    break
                elif x_dir > 0 and probe.x > target['x2'] or x_dir < 0 and probe.x < target['x1']:
                    # overshot
                    break 

    return distinct_velocity_values

if __name__ == '__main__':
    for path in sys.argv[1:]:
        with open(path) as f:
            input = f.read().strip()
            data = parse_input(input)
            print(data)
            print(solve(data))
