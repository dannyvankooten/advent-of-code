from pathlib import Path
from dataclasses import dataclass

@dataclass()
class Point:
    __slots__ = ("x", "y")
    x: int 
    y: int 


def manhattan_distance(a, b): 
    return abs(a.x - b.x) + abs(a.y - b.y)


def parse(input):
    input = input.replace('Sensor at ', '').replace(': closest beacon is at', '').replace(',', '').replace('x=', '').replace('y=', '')
    
    sensors = []
    for line in input.strip().split('\n'):
        sx, sy, bx, by = map(int, line.split())
        sensor = Point(sx, sy) 
        beacon = Point(bx, by) 
        sensors.append((sensor, beacon))

    return sensors  

def count_excluded_positions_on_y(sensors, search_y):
    count = 0
    ranges = []
    exclusions = set()
    
    for s, b in sensors: 
        dy = -1 if s.y > search_y else 1
        top = Point(s.x, search_y)
        treshold = manhattan_distance(s, b)
        while manhattan_distance(s, top) < treshold:
            top.y += dy 
        
        diff = abs(top.y - search_y) 
        if diff == 0:
            continue 
        start = s.x - diff 
        end = s.x + diff 
        ranges.append((start, end)) 

        if b.y == search_y:
            exclusions.add(b.x)
   
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



    count -= len(exclusions)
    return count

def mh(x1, x2, y1, y2):
    return abs(x1 - x2) + abs(y1 - y2)

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
    
    def covers(self, px, py):
        y_diff = abs(py - self.y) 
        x_size = self.size - y_diff 
        if x_size <= 0:
            return False 

        x_start = self.x - x_size 
        if x_start > px:
            return False 

        x_end = self.x + x_size 
        if x_end < px:
            return False 

        return x_end 


def find_position(sensors, lim):
    px, py = 0, 0
    sensors = [Sensor(s, b) for s, b in sensors]

    free_spot = False 
    while free_spot == False: 
        free_spot = True

        for s in sensors:
            next_x = s.covers(px, py) 
            if next_x != False:
                free_spot = False 
                px = next_x + 1
                if px >= lim:
                    px = 0 
                    py += 1

                # go to next x, y position    
                break
                
    return px * 4000000 + py


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    sensors = parse(input) 

    # pt1
    pt1 = count_excluded_positions_on_y(sensors, 2000000) 
    print("pt1: ", pt1)
    assert(pt1 == 5240818)

    # pt2
    pt2 = find_position(sensors, 4000000) 
    print("pt2: ", pt2)
    assert(pt2 == 13213086906101)

