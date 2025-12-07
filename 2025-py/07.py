import sys
import time
import math

time_start = time.time_ns()
pt1 = 0
pt2 = 0
lines = [list(l.strip()) for l in sys.stdin.readlines()]
grid = lines

def beam_down(grid, pos) -> int:
	(x, y) = pos

	if x < 0 or x > len(grid[0]) or grid[y][x] == '|':
		return 0

	# walk down until at splitter
	while y < len(grid) and grid[y][x] != '^':
		grid[y][x] = '|'
		y += 1

	# if not at splitter, quit
	if y >= len(grid) or grid[y][x] != '^':
		return 0

	splits = 0
	if grid[y][x-1] == '.':
		if splits == 0:
			splits = 1
		splits += beam_down(grid, (x-1, y))

	if grid[y][x+1] == '.':
		if splits == 0:
			splits = 1
		splits += beam_down(grid, (x+1, y))

	return splits



sx = lines[0].index('S')
sy = 0

print("starting at ", sx, sy)
pt1 = beam_down(grid, (sx, sy))


print("--- Day 07 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

# assert(pt1)
# assert(pt2)
