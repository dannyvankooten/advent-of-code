crabs = [int(n) for n in open("input.txt").read().split(',')]

# cheapest position to align on is probably average position
avg = int(sum(crabs) / len(crabs))

cheapest = -1
for pos in range(avg-1, avg+2):
    sum = 0
    for c in crabs:
        diff = abs(c - pos)
        fuel = int((pow(diff, 2) + diff) / 2)
        sum += fuel

    if sum < cheapest or cheapest < 0:
        cheapest = sum

print(cheapest)
        