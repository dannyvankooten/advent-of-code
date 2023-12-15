from heapq import heappop, heappush
from pathlib import Path
import time 
from point import INT_MAX, Point

time_start = time.perf_counter()
lines = Path('input.txt').read_text().split('\n')
width = len(lines[0])
height = len(lines)
map_tile = [[int(c) for c in l] for l in lines]

# init map of zeros
map = [[0 for x in range(width * 5)] for y in range(height * 5)]

# create map from tile template
for nx in range(0, 5):
    for ny in range(0, 5):
        for y in range(height):
            for x in range(width):
                risk_factor = map_tile[y][x] + nx + ny
                if risk_factor > 9:
                    risk_factor -= 9

                p = Point(x + nx*height, y + ny*height, risk_factor, INT_MAX, False)
                map[p.y][p.x] = p

# set correct map height 
height *= 5
width *= 5

# list of dx, dy values for direct neighbors
neighbors = [
    (1, 0),
    (0, 1),
    (-1, 0),
    (0, -1),
]

# set tentative distance of 1st node to 0
cur = map[0][0]
cur.tent_distance = 0
destination = map[height-1][width-1]
unvisited = []

while cur != destination:
    # select point with lowest tentative distance
    while cur.visited:
        cur = heappop(unvisited)

    # set tentative distance of direct neighbors
    for (dy, dx) in neighbors:
        nx, ny = cur.x + dx, cur.y + dy
        if 0 <= nx < width and 0 <= ny < height:
            n = map[ny][nx]
            if n.visited == False:
                # update tentative distance of neighbor if smaller
                distance_to_neighbor = cur.tent_distance + n.risk_factor
                if distance_to_neighbor < n.tent_distance:
                    n.tent_distance = distance_to_neighbor
                    heappush(unvisited, n)
    
    # mark node as visited
    cur.visited = True
        
print("--- Day 15: Chiton ---")
print("Part 2: ", map[height-1][width-1].tent_distance)