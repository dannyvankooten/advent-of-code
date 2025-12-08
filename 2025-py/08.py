import sys
import time
import math
from itertools import combinations
from functools import cmp_to_key
from collections import defaultdict

def dist(a, b):
	return math.sqrt((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2 + (a[2] - b[2]) ** 2)

def compare(a, b) -> int:
	da = dist(a[0], a[1])
	db = dist(b[0], b[1])
	if da < db:
		return -1
	elif da > db:
		return 1

	return 0

def work(circuits, a, b):
	left = next(i for i in range(len(circuits)) if a in circuits[i])
	right = next(i for i in range(len(circuits)) if b in circuits[i])

	# merge these two circuits
	if left != right:
		circuits[left] += circuits[right]
		del circuits[right]


time_start = time.time_ns()

# parse
junction_boxes = [tuple(map(int, l.strip().split(','))) for l in sys.stdin.readlines()]

# create all possible connections and sort by euclidean distance
connections =  list(combinations(junction_boxes, 2))
connections = sorted(connections, key=cmp_to_key(compare))
circuits = [[l] for l in junction_boxes]

# now we can make the connections
for (left, right) in connections[:1000]:
	work(circuits, left, right)

# multiply size of three largest circuits
circuits = sorted(circuits, key=len, reverse=True)
pt1 = len(circuits[0]) * len(circuits[1]) * len(circuits[2])

# keep going until a single circuit is left
pt2 = 0
for (left, right) in connections[1000:]:
	work(circuits, left, right)

	if len(circuits) == 1:
		pt2 = left[0] * right[0]
		break

print("--- Day 08 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 135169)
assert(pt2 == 302133440)
