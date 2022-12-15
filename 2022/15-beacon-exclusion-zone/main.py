from pathlib import Path
from dataclasses import dataclass
from re import I 

@dataclass(unsafe_hash=True)
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

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    sensors = parse(input) 
    # pt1
    pt1 = count_excluded_positions_on_y(sensors, 2000000) 
    print("pt1: ", pt1)
    assert(pt1 == 5240818)

    # pt2

