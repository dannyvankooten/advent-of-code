import sys
import time
import math
import numpy as np
from functools import reduce

time_start = time.time_ns()
input = sys.stdin.read()
lines = [l.strip().split() for l in input.splitlines()]

# part 1
pt1 = 0
lines = np.transpose(np.array(lines))
for l in lines:
	if l[-1] == '*':
		pt1 += reduce(lambda a, b: int(a)*int(b), l[:-1])
	else:
		pt1 += reduce(lambda a, b: int(a)+int(b), l[:-1])

# part 2
pt2 = 0
lines = [list(x) for x in input.splitlines()]

# find columns
width = len(lines[0])
height = len(lines)
result = 0
op = ""

for x in range(0, width):
	d = ""

	# walk down column, assembling digit
	for y in range(0, height):
		d += lines[y][x]

	# if column of all spaces, move to next
	if len(d.strip()) == 0:
		pt2 += result
		result = 0
		# print()
		continue

	# set operator state
	if d[-1] == '+' or d[-1] == '*':
		op = d[-1]
		d = d[:-1]

		if op == "*":
			result = 1

	# now we have full digit and operation
	if op == "*":
		result *= int(d)
	else:
		result += int(d)

# final column
pt2 += result





print("--- Day 06 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 4878670269096)
assert(pt2 == 8674740488592)
