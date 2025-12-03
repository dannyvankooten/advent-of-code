import fileinput
import time

time_start = time.time_ns()
lines = list(fileinput.input())

pt1 = 0
for line in lines:
	line = line.strip()

	positions = list(range(len(line)-2, len(line)))
	for pos in range(0, 2):
		prev = 0 if pos == 0 else positions[pos-1] + 1
		for i in range(prev, positions[pos]+1):
			if line[i] > line[positions[pos]]:
				positions[pos] = i


	v = 0
	for p in positions:
		v = v*10 + int(line[p])
	pt1 += v


pt2 = 0
for line in lines:
	line = line.strip()

	positions = list(range(len(line)-12, len(line)))
	for pos in range(0, 12):
		prev = 0 if pos == 0 else positions[pos-1] + 1
		for i in range(positions[pos], prev-1,-1):
			if line[i] >= line[positions[pos]]:
				positions[pos] = i

	v = 0
	for p in positions:
		v = v*10 + int(line[p])
	pt2 += v


print("--- Day 3: Lobby ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_00, "ms")

