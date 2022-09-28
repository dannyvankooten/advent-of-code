from collections import defaultdict, namedtuple
from itertools import permutations, product 

Point = namedtuple("Point", ["x", "y", "z"])

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
        new_coords = []

        # Create all possible permutations... But why are there 48 (8*6) instead of 24?
        for (cx, cy, cz) in product([1, -1], repeat=3):
            new_coords.append([(x*cx, y*cy, z*cz) for (x, y, z) in self.report])
            new_coords.append([(x*cx, y*cy, z*cz) for (x, z, y) in self.report])
            new_coords.append([(x*cx, y*cy, z*cz) for (y, z, x) in self.report])
            new_coords.append([(x*cx, y*cy, z*cz) for (y, x, z) in self.report])
            new_coords.append([(x*cx, y*cy, z*cz) for (z, x, y) in self.report])
            new_coords.append([(x*cx, y*cy, z*cz) for (z, y, x) in self.report])

        self.report_permutations = new_coords

    def fix_orientation(self, report):
        self.report = report
        self.fixed = True

    def set_position(self, x, y, z):
        self.pos = Point(x, y, z)
    

def solve(lines):
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

    # create permutations
    for s in scanners[1:]:
        s.create_permutations()

    # keep matching beacon positions until every scanner is fixed
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
            

    all = set(scanners[0].report)
    for s in scanners[1:]:
        # adjust report based on absolute position                    
        s.report = map(lambda pos: (pos[0] + s.pos.x, pos[1] + s.pos.y, pos[2] + s.pos.z), s.report)

        # add beacons to set to count unique values
        for coords in s.report:
            all.add(coords)

    print("Part 1: ", len(all))

    max_manhattan = 0
    for s1, s2 in permutations(scanners, 2):
        manhattan_distance = abs(s1.pos.x - s2.pos.x) + abs(s1.pos.y - s2.pos.y) + abs(s1.pos.z - s2.pos.z)
        max_manhattan = max(max_manhattan, manhattan_distance)

    print("Part 2: ", max_manhattan)


if __name__ == '__main__':
    with open(0) as f:
        solve(f.readlines())
