import fileinput
import time

time_start = time.time_ns()
lines = [l.strip() for l in fileinput.input()]

pt1 = 0
pt2 = 0

for line in lines:
	pass 

print("--- Day %%DAYNAME%% ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

