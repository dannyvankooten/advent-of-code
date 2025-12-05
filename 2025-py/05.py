import sys
import time
import math
from operator import itemgetter
from bisect import bisect_left

time_start = time.time_ns()
pt1 = 0

[ingredient_id_ranges, available_ingredient_ids] = sys.stdin.read().split("\n\n")
ingredient_id_ranges = [list(map(int, r.split('-'))) for r in ingredient_id_ranges.splitlines()]
available_ingredient_ids = list(map(int, available_ingredient_ids.splitlines()))

# sort ranges by start
ingredient_id_ranges = sorted(ingredient_id_ranges, key=itemgetter(0))

for ingredient_id in available_ingredient_ids:
	s = bisect_left(ingredient_id_ranges, ingredient_id, key=itemgetter(0))

	# lol. sorry. shortcut taken here.
	s = int(max(0, s-10))

	for (start, end) in ingredient_id_ranges[s:]:
		# if within range, we good
		if ingredient_id >= start and ingredient_id <= end:
			pt1 += 1
			break


instable = True
while instable:
	instable = False
	for i in range(0, len(ingredient_id_ranges)-1):
		left_end = ingredient_id_ranges[i][1]
		right_start = ingredient_id_ranges[i+1][0]

		if left_end >= right_start:
			ingredient_id_ranges[i+1][0] = left_end+1
			instable = True

	ingredient_id_ranges = sorted(ingredient_id_ranges, key=itemgetter(0))


pt2 = 0
for (start, end) in ingredient_id_ranges:
	pt2 += int(max(0, end - start+1))


print("--- Day 05 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 775)
assert(pt2 == 350684792662845)

