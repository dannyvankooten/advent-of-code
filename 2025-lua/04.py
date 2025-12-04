import sys
import time
import math

time_start = time.time_ns()
pt1 = 0
pt2 = 0
lines = [list(l.strip()) for l in sys.stdin.readlines()]

neighbors = [
	(-1, -1),
	(0, -1),
	(1, -1),
	(-1, 0),
	(1, 0),
	(-1, 1),
	(0, 1),
	(1, 1),
]
height = len(lines)
width = len(lines[0])

def forklifts(grid, remove_objects=False):
	count = 0
	removed = 1
	while removed > 0:
		removed = 0
		for y in range(0, height):
			for x in range(0, width):
				if lines[y][x] != '@':
					continue

				neighbor_count = 0

				# toilet paper, check neighbors
				for (dx, dy) in neighbors:
					x2 = x + dx
					y2 = y + dy

					if x2 >= 0 and x2 < width and y2 >= 0 and y2 < height and lines[y2][x2] == '@':
						neighbor_count += 1

				if neighbor_count < 4:
					count += 1

					if remove_objects == True:
						lines[y][x] = "."
						removed += 1

	return count

# The forklifts can only access a roll of paper if there are fewer than four rolls of paper in the eight adjacent positions. If you can figure out which rolls of paper the forklifts can access, they'll spend less time looking and more time breaking down the wall to the cafeteria.
pt1 = forklifts(lines, False)
pt2 = forklifts(lines, True)

print("--- Day 04 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 1467)
assert(pt2 == 8484)
