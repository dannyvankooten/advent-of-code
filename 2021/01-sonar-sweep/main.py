from pathlib import Path

input = Path("input.txt").read_text()
numbers = [int(n) for n in input.split("\n")]
a, b, c = numbers[0:3]
prev = 0
sum = 0

for n in numbers[3:]:
    cur = a + b + c

    if cur > prev:
        sum += 1

    prev = cur
    a = b
    b = c
    c = n


print(sum)