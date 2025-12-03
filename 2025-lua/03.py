import fileinput
import time

time_start = time.time_ns()
lines = [l.strip() for l in fileinput.input()]

def find_max(line, length: int):
	positions = list(range(len(line)-length, len(line)))
	for pos in range(0, length):
		start = 0 if pos == 0 else positions[pos-1]+1
		end = positions[pos]

		for i in range(end, start-1, -1):
			if line[i] >= line[positions[pos]]:
				positions[pos] = i

	return int("".join([line[p] for p in positions]))

pt1 = 0
pt2 = 0

for line in lines:
	pt1 += find_max(line, 2)
	pt2 += find_max(line, 12)

print("--- Day 3: Lobby ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 16887)
assert(pt2 == 167302518850275)
