import time
import sys
import math

time_start = time.time_ns()

id_ranges = [list(map(int, v.split('-'))) for v in sys.stdin.read().split(',')]

pt1 = 0
for [start, end] in id_ranges:
	for v in range(start, end+1):
		full_id = str(v)
		half_id = full_id[:int(len(full_id)/2)]
		if half_id*2 == full_id:
			pt1 += v


pt2 = 0
for [start, end] in id_ranges:
	for v in range(start, end+1):
		full_id = str(v)
		for repeats in range(2, len(full_id)+1):

			if len(full_id) % repeats != 0:
				continue

			partial_id = full_id[:int(len(full_id)/repeats)]
			if partial_id*repeats == full_id:
				pt2 += v
				break

print("--- Day 2: Gift Shop ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 24747430309)
assert(pt2 == 30962646823)
