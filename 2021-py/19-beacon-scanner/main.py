from collections import defaultdict, namedtuple
from itertools import permutations, product
from pathlib import Path
from time import perf_counter, time 

start = perf_counter()
Point = namedtuple("Point", ["x", "y", "z"])

def matrix_rotations(values: list[(int, int, int)]):
    rotations = [
        # north
        [( 1, 0, 0), ( 0, 1, 0), ( 0, 0, 1)],
        [( 0, 0, 1), ( 0, 1, 0), (-1, 0, 0)],
        [(-1, 0, 0), ( 0, 1, 0), ( 0, 0,-1)],
        [( 0, 0,-1), ( 0, 1, 0), ( 1, 0, 0)],

        # east
        [( 0, 1, 0), (-1, 0, 0), ( 0, 0, 1)],
        [( 0, 1, 0), ( 0, 0,-1), (-1, 0, 0)],
        [( 0, 1, 0), ( 1, 0, 0), ( 0, 0,-1)],
        [( 0, 1, 0), ( 0, 0, 1), ( 1, 0, 0)],

        # south
        [(-1, 0, 0), ( 0,-1, 0), ( 0, 0, 1)],
        [( 0, 0,-1), ( 0,-1, 0), (-1, 0, 0)],
        [( 1, 0, 0), ( 0,-1, 0), ( 0, 0,-1)],
        [( 0, 0, 1), ( 0,-1, 0), ( 1, 0, 0)],

        # west
        [( 0,-1, 0), ( 1, 0, 0), ( 0, 0, 1)],
        [( 0,-1, 0), ( 0, 0, 1), (-1, 0, 0)],
        [( 0,-1, 0), (-1, 0, 0), ( 0, 0,-1)],
        [( 0,-1, 0), ( 0, 0,-1), ( 1, 0, 0)],

        # up 
        [( 1, 0, 0), ( 0, 0,-1), ( 0, 1, 0)],
        [( 0, 0,-1), (-1, 0, 0), ( 0, 1, 0)],
        [(-1, 0, 0), ( 0, 0, 1), ( 0, 1, 0)],
        [( 0, 0, 1), ( 1, 0, 0), ( 0, 1, 0)],

        # down 
        [( 1, 0, 0), ( 0, 0, 1), ( 0,-1, 0)],
        [( 0, 0, 1), (-1, 0, 0), ( 0,-1, 0)],
        [(-1, 0, 0), ( 0, 0,-1), ( 0,-1, 0)],
        [( 0, 0, -1), ( 1, 0, 0), ( 0,-1, 0)]
    ]

    rotated_values = [
        [(x * a[0] + y * a[1] + z * a[2], x * b[0] + y * b[1] + z * b[2], x * c[0] + y * c[1] + z * c[2]) 
            for x, y, z in values]
                for ((a,b,c)) in rotations
    ]
   
    return rotated_values

class Scanner():
    def __init__(self, i, report):
        self.index = i
        self.report = report 
        self.pos = Point(None, None, None)
        self.fixed = False

        if i == 0:
            self.fixed = True
            self.pos = Point(0, 0, 0)

    def create_permutations(self):
        self.report_permutations = matrix_rotations(self.report)

    def fix_orientation(self, report):
        self.report = report
        self.fixed = True

    def set_position(self, x, y, z):
        self.pos = Point(x, y, z)
    

def solve(input):
    lines = input.strip().split("\n")
    scanner = None
    scanners = []
    for l in map(lambda l: l.strip(), lines):
        if l == "":
            continue

        if l.startswith("--- scanner "):
            scanner = Scanner(len(scanners), [])
            scanners.append(scanner)
            continue

        (x, y, z) = [int(n) for n in l.split(",")]
        scanner.report.append((x, y, z))

    # create report permutations for every possible orientation
    for s in scanners[1:]:
        s.create_permutations()    

    # keep matching beacon positions until every scanner is fixed at an absolute position
    while len(list(filter(lambda s: s.fixed is False, scanners[1:]))) > 0:
        for s1 in filter(lambda s: s.fixed, scanners):
            for s2 in filter(lambda s: not s.fixed, scanners):
                for report in s2.report_permutations:
                    positions = defaultdict(int)
                    for (x1, y1, z1) in s1.report:
                        for (x2, y2, z2) in report:
                            positions[((x2-x1), (y2-y1), (z2-z1))] += 1
                                          
                    positions = {k: v for k, v in positions.items() if v >= 12}
                    if positions:
                        s2.fix_orientation(report)
                        (x, y, z) = list(positions.keys()).pop()
                        s2.set_position((x - s1.pos.x) * -1, (y - s1.pos.y) * -1, (z - s1.pos.z) * -1)
                        break

    beacons = set(scanners[0].report)
    for s in scanners[1:]:        
        for (x, y, z) in s.report:
            beacons.add((x + s.pos.x, y + s.pos.y, z + s.pos.z))
    print("Part 1: ", len(beacons))

    max_manhattan = 0
    for s1, s2 in permutations(scanners, 2):
        manhattan_distance = abs(s1.pos.x - s2.pos.x) + abs(s1.pos.y - s2.pos.y) + abs(s1.pos.z - s2.pos.z)
        max_manhattan = max(max_manhattan, manhattan_distance)
    print("Part 2: ", max_manhattan)


if __name__ == '__main__':
    print("--- Day 19: Beacon Scanner ---")
    input = Path("input.txt").read_text()
    solve(input)