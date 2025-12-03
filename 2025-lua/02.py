import time
import sys
import math

time_start = time.time_ns()

id_ranges = [list(map(int, v.split('-'))) for v in sys.stdin.read().split(',')]
pt1 = 0
for [start, end] in id_ranges:
	for v in range(start, end+1):
		digits = int(math.log10(v)) + 1

		# uneven number of digits can never repeat twice
		# skip to next log10 range
		if digits & 1:
			v *= 10

		half_length = digits / 2
		first_half = int(v // 10**half_length)
		first_half_repeated = int(first_half * (10**half_length) + first_half)
		if first_half_repeated == v:
			pt1 += v

pt2 = 0

print("--- Day 2: Gift Shop ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 24747430309)
assert(pt2 == 30962646823)
