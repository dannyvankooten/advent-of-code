import sys
import time
import math

time_start = time.time_ns()
pt1 = 0
pt2 = 0
p = 50

lines = [l.strip() for l in sys.stdin.readlines()]

for l in lines:
	amount = -int(l[1:]) if l[0] == 'L' else int(l[1:])
	p = p + amount
	pt2 = pt2 + abs(p // 100)
	p = p % 100
	if p == 0:
		pt1 += 1


print("--- Day 1: Secret Entrance ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 999)
assert(pt2 == 6099)
