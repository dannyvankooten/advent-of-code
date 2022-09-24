from queue import PriorityQueue
import time 
from point import Point

time_start = time.time()
lines = open('input.txt').read().split('\n')
width = len(lines[0])
height = len(lines)
map_tile = [[int(c) for c in l] for l in lines]
q = PriorityQueue()

# init map of zeros
map = [[0 for x in range(width * 5)] for y in range(height * 5)]

print("{:.2f} Filling map from tile template".format(time.time() - time_start))
# create map from tile template
for nx in range(0, 5):
    for ny in range(0, 5):
        for y in range(height):
            for x in range(width):
                val = map_tile[y][x] + nx + ny
                if val > 9:
                    val -= 9

                p = Point(x + nx*height, y + ny*height, val)
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

print("{:.2f} Start pathfinding".format(time.time() - time_start))

while cur != destination:
    # select point with lowest tentative distance
    while cur.visited:
        (prio, cur) = q.get()

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
                    q.put((n.tent_distance, n))
    
    # mark node as visited
    cur.visited = True
        
print("Answer: {}".format(map[height-1][width-1].tent_distance))
