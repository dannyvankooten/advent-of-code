import sys
import time
import math
import numpy as np
from functools import reduce

time_start = time.time_ns()
lines = [l.strip().split() for l in sys.stdin.readlines()]

pt1 = 0
lines = np.transpose(np.array(lines))
for l in lines:
	if l[-1] == '*':
		pt1 += reduce(lambda a, b: int(a)*int(b), l[:-1])
	else:
		pt1 += reduce(lambda a, b: int(a)+int(b), l[:-1])

pt2 = 0

print("--- Day 06 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 4878670269096)
# assert(pt2)
