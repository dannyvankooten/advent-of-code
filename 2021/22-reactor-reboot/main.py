from dataclasses import dataclass
from itertools import combinations, permutations
from pathlib import Path
from parse import parse


def parse_input(input):
    lines = input.split("\n")
    cubes = []
    for line in lines:
        vars = parse("{status} x={x_start:d}..{x_end:d},y={y_start:d}..{y_end:d},z={z_start:d}..{z_end:d}", line).named
        cubes.append(Cube(vars['x_start'], vars['x_end'], vars['y_start'], vars['y_end'], vars['z_start'], vars['z_end'], 1 if vars['status'] == 'on' else 0))
    return cubes


def part1(cubes):
    grid = [[[0 for _ in range(101)] for _ in range(101)] for _ in range(101)]
    for c in cubes:
        x1 = max(c.x1, -50)
        x2 = min(c.x2, 50)
        y1 = max(c.y1, -50)
        y2 = min(c.y2, 50)
        z1 = max(c.z1, -50)
        z2 = min(c.z2, 50)
        for z in range(z1, z2+1):
            for y in range(y1, y2+1):
                for x in range(x1, x2+1):
                    grid[z+50][y+50][x+50] = c.status

    return sum([xs for zs in grid for ys in zs for xs in ys])

@dataclass 
class Cube():
    x1: int
    x2: int 
    y1: int 
    y2: int 
    z1: int
    z2: int
    status: int 

    def volume(self):
        vol = (self.x2 - self.x1 + 1) * (self.y2 - self.y1 + 1) * (self.z2 - self.z1 + 1)
        return vol if self.status else -vol



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
        
        return Cube(x1, x2, y1, y2, z1, z2, status)

    return None

def part2(cubes):
    final = []
    for cube in cubes:
        add = []
        for prev in final:
            shared = intersect(prev, cube)
            if shared is not None:
                add.append(shared)
      
        if cube.status:
            add.append(cube)

        final = final + add
    
    return sum([c.volume() for c in final])

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    cubes = parse_input(input)
    pt1 = part1(cubes)
    print(pt1, pt1 == 567496)

    pt2 = part2(cubes)
    print(pt2, pt2 == 1355961721298916)
