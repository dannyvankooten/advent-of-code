from dataclasses import dataclass
from pathlib import Path
import re 

@dataclass 
class Cube():
    status: int 
    x1: int
    x2: int 
    y1: int 
    y2: int 
    z1: int
    z2: int

    def volume(self):
        vol = (self.x2 - self.x1 + 1) * (self.y2 - self.y1 + 1) * (self.z2 - self.z1 + 1)
        return vol if self.status else -vol


def parse_input(input):
    lines = input.split("\n")
    lines = [[int(n) for n in re.sub("y=|x=|z=", "", line).replace('..', ',').replace(' ', ',').replace('on', '1').replace('off', '0').split(',')] for line in lines]
    cubes = [Cube(*l) for l in lines]
    return cubes


def intersect(a: Cube, b: Cube):
    x1 = max(a.x1, b.x1)
    x2 = min(a.x2, b.x2)
    y1 = max(a.y1, b.y1)
    y2 = min(a.y2, b.y2)
    z1 = max(a.z1, b.z1)
    z2 = min(a.z2, b.z2)

    if x1 < x2 and y1 < y2 and z1 < z2:
        # order of calling is important!
        status = b.status
        if a.status and b.status:
            status = 0
        elif a.status == 0 and b.status == 0:
            status = 1
        
        return Cube(status, x1, x2, y1, y2, z1, z2)

    return None


def solve(cubes):
    final = []
    for cube in cubes:
        add = []

        # go through previous cubes and add either positive or negative volume cubes based on intersections
        for prev in final:
            shared = intersect(prev, cube)
            if shared is not None:
                add.append(shared)
      
        if cube.status:
            add.append(cube)

        final += add
    
    return sum([c.volume() for c in final])


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    cubes = parse_input(input)
    pt1 = solve([c for c in cubes if abs(c.x1) <= 50])
    print(pt1, pt1 == 567496)

    pt2 = solve(cubes)
    print(pt2, pt2 == 1355961721298916)
