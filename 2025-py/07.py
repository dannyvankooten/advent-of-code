import sys
import time
import math
import copy

time_start = time.time_ns()
lines = [list(l.strip()) for l in sys.stdin.readlines()]

def beam_down(grid, pos, pt2 = False) -> int:
	(x, y) = pos

	# check for OOB
	if x < 0 or x > len(grid[0]):
		return 0

	# check if we've been here (and how many splits from here to end)
	if isinstance(grid[y][x], int):
		return grid[y][x]

	# walk down until at splitter
	while y < len(grid) and grid[y][x] != '^':
		y += 1

	# if not at splitter, quit
	if y >= len(grid):
		return 0

	splits = 0

	if pt2 or grid[y][x-1] == '.':
		splits = 1
		grid[y][x-1] = beam_down(grid, (x-1, y), pt2)
		splits += grid[y][x-1]

	if pt2 or grid[y][x+1] == '.':
		splits = max(1, splits)
		grid[y][x+1] =  beam_down(grid, (x+1, y), pt2)
		splits += grid[y][x+1]


	return splits


pt1 = beam_down(copy.deepcopy(lines), (lines[0].index('S'), 0), False)
pt2 = 1 + beam_down(copy.deepcopy(lines), (lines[0].index('S'), 0), True)

# i think we can just count the splitters directly adjacent to another splitter? maybe multiply each level?

print("--- Day 07 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 1581)
assert(pt2 == 73007003089792)
