from pathlib import Path
from heapq import heappop, heappush
from functools import total_ordering

@total_ordering
class Node:
    def __init__(self, elevation: str):
        self.neighbors = []
        self.is_start = False 
        self.is_end = False 
        self.distance = 99999

        match elevation:
            case 'S':
                self.is_start = True 
                self.elevation = 'a' 
            case 'E':
                self.is_end = True 
                self.elevation = 'z'
            case _: 
                self.elevation = elevation

    def add_neighbor(self, other: 'Node'):
        if ord(self.elevation) - ord(other.elevation) >= -1:
            self.neighbors.append(other)

    def __lt__(self, other):
        return self.distance < other.distance

def reset(grid: list[list[Node]]):
    for row in grid:
        for node in row:
            node.distance = 99999


def parse(input: str) -> list[list[Node]]:
    grid = [[Node(c) for c in line] for line in input.strip().split('\n')]
    height, width = len(grid), len(grid[0])

    # add neighbors for each node
    for row, _ in enumerate(grid):
        for col, node in enumerate(grid[row]):
            if row > 0:
                node.add_neighbor(grid[row-1][col])
            if col > 0:
                node.add_neighbor(grid[row][col-1])
            if row < height - 1:
                node.add_neighbor(grid[row+1][col])
            if col < width - 1:
                node.add_neighbor(grid[row][col+1])


    return grid


def solve(grid: list[list[Node]], fixed_start=True) -> int:
    # find start node(s)
    start_nodes = []
    for row in grid:
        for node in row:
            if fixed_start and node.is_start:
                start_nodes.append(node)
            if not fixed_start and node.elevation == 'a':
                start_nodes.append(node)

    # dijkstra
    distances_to_end = []
    for start in start_nodes:
        reset(grid)
        start.distance = 0
        unvisited = [start]
        cur = Node('xaa') 

        while not cur.is_end:
            # some paths lead into a dead end
            if len(unvisited) == 0:
                break

            cur = heappop(unvisited)
            distance = cur.distance + 1
            for v in cur.neighbors:
                if distance < v.distance:
                    v.distance = distance 
                    heappush(unvisited, v)

        # if we reached the end, add total distance to list
        if cur.is_end:
            distances_to_end.append(cur.distance) 

    return min(distances_to_end)


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    grid = parse(input)
    print("pt1: ", solve(grid, True))
    print("pt2: ", solve(grid, False))


