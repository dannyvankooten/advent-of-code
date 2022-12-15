from pathlib import Path

class Point:
    __slots__ = ("x", "y")
    x: int
    y: int 

    def __init__(self, x: int, y: int):
        self.x = x 
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y 

    def __hash__(self):
        return hash((self.x, self.y))


class Sensor:
    __slots__ = ("x", "y", "size")

    def __init__(self, coords: Point, beacon: Point):
        self.x = x = coords.x 
        self.y = y = coords.y 

        # estimate size of sensor area (taking steps of 1000)
        tx, ty = x, y + 1
        treshold = mh(x, beacon.x, y, beacon.y) 
        while mh(x, tx, y, ty) < treshold:
            ty += 100
        
        # backtrack to correct height 
        while mh(x, tx, y, ty) > treshold:
            ty -= 1 

        self.size = ty - y

    def __repr__(self):
        return f"Sensor (x={self.x}, y={self.y})"
    
    def covers(self, px: int , py: int):
        y_diff = abs(py - self.y) 
        x_size = self.size - y_diff 
        if x_size <= 0:
            return 0 

        x_start = self.x - x_size 
        if x_start > px:
            return 0 

        x_end = self.x + x_size 
        if x_end < px:
            return 0 

        return x_end 


def parse(input: str):
    input = input.replace('Sensor at ', '').replace(': closest beacon is at', '').replace(',', '').replace('x=', '').replace('y=', '')
    
    sensors = []
    beacons = set()
    for line in input.strip().split('\n'):
        sx, sy, bx, by = map(int, line.split())
        sensor = Point(sx, sy) 
        beacon = Point(bx, by) 
        sensors.append(Sensor(sensor, beacon))
        beacons.add(beacon)

    assert(len(sensors) > len(beacons))

    return sensors, list(beacons)

def mh(x1: int, x2: int, y1: int, y2: int):
    return abs(x1 - x2) + abs(y1 - y2)


def count_excluded_positions_on_y(sensors: list[Sensor], beacons: list[Point], search_y: int) -> int:
    count = 0
    ranges = []

    for s in sensors:
        end = s.covers(s.x, search_y)
        if end == 0:
            continue 

        start = s.x - (end - s.x)
        ranges.append((start, end))
  
    # sum non-overlapping ranges
    ranges.sort()
    prev = ranges[0][0] - 1
    for start, end in ranges:
        if start <= prev:
            start = prev + 1
        
        diff = end - start + 1
        if diff > 0:
            count += end - start + 1

        if end > prev:
            prev = end
    
    # exclude beacons on this Y
    for b in beacons:
        if b.y == search_y:
            count -= 1 

    return count

def find_position(sensors: list[Sensor], lim: int) -> int:
    px, py = 0, 0
    free_spot = False 

    while free_spot == False: 
        free_spot = True

        for s in sensors:
            next_x = s.covers(px, py) 
            if next_x == 0:
                continue 

            free_spot = False 

            # go to next (x, y) position
            px = next_x + 1
            if px >= lim:
                px = 0 
                py += 1
              

            break
    return px * 4000000 + py


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    sensors, beacons = parse(input) 

    # pt1
    pt1 = count_excluded_positions_on_y(sensors, beacons, 2000000) 
    print("pt1: ", pt1)
    assert(pt1 == 5240818)

    # pt2
    pt2 = find_position(sensors, 4000000) 
    print("pt2: ", pt2)
    assert(pt2 == 13213086906101)

