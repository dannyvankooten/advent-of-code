import sys
import time
import math
import re
from collections import deque

time_start = time.time_ns()
pt1 = 0
pt2 = 0
lines = [l.strip() for l in sys.stdin.readlines()]


def solve_pt1(target_state, buttons):
	queue = deque([(list("." * len(target_state)), 0)])
	while queue:
		(state, pushes) = queue.popleft()

		for b in buttons:
			new_state = list(state)
			for x in b:
				new_state[x] = "." if new_state[x] == "#" else "#"

			if new_state == target_state:
				return pushes+1

			queue.append((new_state, pushes+1))

	raise Exception


def solve_pt2(target_state, buttons):
	initial_state = [0] * len(target_state)
	queue = deque([(initial_state, 0)])
	while queue:
		(state, pushes) = queue.popleft()

		for b in buttons:
			new_state = list(state)
			for x in b:
				new_state[x] += 1

			if new_state == target_state:
				return pushes+1

			queue.append((new_state, pushes+1))

	raise Exception

for l in lines:
	# [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
	groups = l.split(" ")
	target_state = list(groups[0][1:-1])
	buttons = [list(map(int, x[1:-1].split(","))) for x in groups[1:-1]]
	target_joltage = list(map(int, groups[-1][1:-1].split(",")))

	pt1 += solve_pt1(target_state, buttons)

	# this is way too slow for part 2...
	# i think we can somehow see the button groups as factors and then factorize or lcm?
	# pt2 += solve_pt2(target_joltage, buttons)






print("--- Day 10 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

# assert(pt1)
# assert(pt2)
