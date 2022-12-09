from pathlib import Path
from dataclasses import dataclass

@dataclass 
class Point:
    x: int
    y: int 

    def __hash__(self):
        return hash(repr(self))

    def step(self, dir: str):
        match dir:
            case 'R':
                self.x += 1 
            case 'L':
               self.x -= 1 
            case 'U':
                self.y += 1
            case 'D':
                self.y -= 1 
    
    def follow(self, other: 'Point'):
        dx = other.x - self.x 
        dy = other.y - self.y 
       
        # don't update position if already touching
        if abs(dx) <= 1 and abs(dy) <= 1:
            return 

        if dx == 0:
            self.y += 1 if dy == 2 else -1 
        elif dy == 0:
            self.x += 1 if dx == 2 else -1
        else:  
            # not on same row or column, move diagonally
            self.y += 1 if dy > 0 else -1 
            self.x += 1 if dx > 0 else -1



def solve(input: str, n: int) -> int:
    knots = [Point(0, 0) for _ in range(0, n)]
    seen = set()

    for line in input.strip().split('\n'):
        dir, steps = line.split()

        for _ in range(0, int(steps)):
            # update head position
            knots[0].step(dir)

            # all other knots follow
            for a, b in zip(knots, knots[1:]):
                b.follow(a) 
          
            seen.add(knots[-1])

    return len(seen)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input, 2))
    print("pt2: ", solve(input, 10))

