lines = open('input.txt').read().split('\n')
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
                val = map_tile[y][x]
                val += nx 
                if val > 9:
                    val -= 9

                val += ny
                if val > 9:
                    val -= 9

                map[y + ny*height][x + nx*height] = val

# set correct map height 
height *= 5
width *= 5

class Point():
    def __init__(self, x,  y, risk_factor):
        self.x = x
        self.y = y
        self.visited = False
        self.tent_distance = float('inf')
        self.risk_factor = risk_factor
        self.neighbors = []

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))

    def __lt__(self, other):
        return self.tent_distance < other.tent_distance

    def __repr__(self):
        return "({}, {}): {}".format(self.x, self.y, self.tent_distance)

# convert each map value to a point so we can store shit
# keep track of unvisited points in set
unvisited = []
for y in range(height):
    for x in range(width):
        map[y][x] = p = Point(x, y, map[y][x])
        unvisited.append(p)

# list of dx, dy values for direct neighbors
# store direct neighbors on each point in map
neighbors = [
    (1, 0),
    (0, 1),
    (-1, 0),
    (0, -1),
]
for y in range(height):
    for x in range(width):
        for (dy, dx) in neighbors:
            # check against oob
            (nx, ny) = (x + dx, y + dy)
            if 0 <= nx < width and 0 <= ny < height:
                neighbor = map[ny][nx]
                map[y][x].neighbors.append(neighbor)



# set tentative distance of 1st node to 0
map[0][0].tent_distance = 0
cur = map[0][0]
destination = map[height-1][width-1]

print("STart pathfinding")

while destination.visited == False:
    # select point with lowest tentative distance
    if cur.visited:
        cur = unvisited.pop(0)

    if cur == destination:
        print("Destination is smallest among unvisited!")
        break

    # set tentative distance of direct neighbors
    updated = False
    for n in filter(lambda n: n.visited == False, cur.neighbors):
        # update tentative distance of neighbor if smaller
        distance_to_neighbor = cur.tent_distance + n.risk_factor
        if distance_to_neighbor < n.tent_distance:
            n.tent_distance = distance_to_neighbor
            updated = True

    cur.visited = True
    if updated:
        unvisited.sort()
        
print("Answer: {}".format(map[height-1][width-1].tent_distance))
