from pathlib import Path

input = Path("input.txt").read_text()
numbers = [int(n) for n in input.split("\n")]

prev = 1 << 30
pt1 = 0
for n in numbers:
    if n > prev:
        pt1 += 1
    prev = n


a, b, c = numbers[0:3]
prev = 0
pt2 = 0
for n in numbers[3:]:
    cur = a + b + c

    if cur > prev:
        pt2 += 1

    prev = cur
    a = b
    b = c
    c = n


print("--- Day 1: Sonar Sweep ---")
print("Part 1: ", pt1, pt1 == 1400)
print("Part 2: ", pt2, pt2 == 1429)