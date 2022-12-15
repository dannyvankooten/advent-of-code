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

class Sensor:
    __slots__ = ("coords", "size")

    def __init__(self, coords: Point, beacon: Point):
        self.coords = coords 

        # calculate size of sensor area
        top = Point(coords.x, coords.y + 1) 
        treshold = manhattan_distance(coords, beacon) 
        while manhattan_distance(coords, top) < treshold:
            top.y += 1 

        self.size = top.y - coords.y

    def __repr__(self):
        return f"Sensor (x={self.coords.x}, y={self.coords.y})"
    
    def covers(self, p: Point):
        y_diff = abs(p.y - self.coords.y) 
        x_size = self.size - y_diff 
        if x_size <= 0:
            return False 

        x_start = self.coords.x - x_size 
        if x_start > p.x:
            return False 

        x_end = self.coords.x + x_size 
        if x_end < p.x:
            return False 

        return x_end 


def find_position(sensors, lim):
    pos = Point(0, 0)
    sensors = [Sensor(s, b) for s, b in sensors]
    free_spot = False 
    while free_spot == False: 
        free_spot = True

        for s in sensors:
            next_x = s.covers(pos) 
            if next_x != False:
                free_spot = False 
                pos.x = next_x + 1
                if pos.x >= lim:
                    pos.x = 0 
                    pos.y += 1

                    if pos.y >= lim:
                        raise Exception("no spot found")

                # go to next x, y position    
                break
                
    return pos.x * 4000000 + pos.y


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

