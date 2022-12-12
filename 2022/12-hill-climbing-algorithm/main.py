from pathlib import Path
from collections import deque 

class Node:
    neighbors: list = []
    is_start: bool = False 
    is_end: bool = False
    distance: int = 999999

    def __init__(self, elevation: str):
        match elevation:
            case 'S':
                self.is_start = True 
                self.elevation = 'a' 
            case 'E':
                self.is_end = True 
                self.elevation = 'z'
            case _: 
                self.elevation = elevation


def parse(input: str, pt: int) -> Node:
    grid = [[Node(c) for c in line] for line in input.strip().split('\n')]
    height, width = len(grid), len(grid[0])
    start = None

    if pt == 1:
        cmp = lambda b: ord(node.elevation) - ord(b.elevation) >= -1
    else:
        cmp = lambda b: ord(b.elevation) - ord(node.elevation) >= -1

    for row, _ in enumerate(grid):
        for col, node in enumerate(grid[row]):
            # add neighbors
            neighbors = []
            if row > 0:
                neighbors.append(grid[row-1][col])
            if col > 0:
                neighbors.append(grid[row][col-1])
            if row < height - 1:
                neighbors.append(grid[row+1][col])
            if col < width - 1:
                neighbors.append(grid[row][col+1])
            node.neighbors = list(filter(cmp, neighbors))

            if node.is_start and pt == 1:
                start = node 

            if node.is_end and pt == 2:
                start = node
    
    return start


def solve(input: str, pt: int) -> int:
    start = parse(input, pt)
    start.distance = 0
    unvisited = deque([start])

    while True:
        cur = unvisited.popleft()

        # part 1: first time we encounter end is always shortest path
        if pt == 1 and cur.is_end:
            return cur.distance

        # part 1: first 'a' elevation we encounter is shortest 
        # because we start from end node this time
        if pt == 2 and cur.elevation == 'a':
            return cur.distance

        distance = cur.distance + 1
        for v in cur.neighbors:
            if distance < v.distance:
                v.distance = distance 
                unvisited.append(v)

    raise Exception("path not found")


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input, 1))
    print("pt2: ", solve(input, 2))


