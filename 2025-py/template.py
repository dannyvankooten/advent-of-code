import sys
import time
import math

time_start = time.time_ns()
pt1 = 0
pt2 = 0
lines = [l.strip() for l in sys.stdin.readlines()]

for l in lines:
	pass


print("--- Day %%DAYNAME%% --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

# assert(pt1)
# assert(pt2)
