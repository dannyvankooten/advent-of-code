import sys
import time

time_start = time.time_ns()
lines = [list(l.strip()) for l in sys.stdin.readlines()]

neighbors = [(-1, -1), (0, -1), (1, -1), (-1, 0), (1, 0), (-1, 1), (0, 1), (1, 1)]
height = len(lines)
width = len(lines[0])

def forklifts(grid, remove_objects=False) -> int:
	count = 0

	while True:
		stable = True
		for y in range(0, height):
			for x in range(0, width):
				if lines[y][x] != '@':
					continue

				neighbor_count = 0
				for (dx, dy) in neighbors:
					x2, y2 = x + dx, y + dy
					if width > x2 >= 0 and height > y2 >= 0 and lines[y2][x2] == '@':
						neighbor_count += 1

				if neighbor_count < 4:
					count += 1

					if remove_objects == True:
						lines[y][x] = "."
						stable = False

		if stable:
			break

	return count

pt1 = forklifts(lines, False)
pt2 = forklifts(lines, True)

print("--- Day 04 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 1467)
assert(pt2 == 8484)
