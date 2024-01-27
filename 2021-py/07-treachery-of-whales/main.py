crabs = [int(n) for n in open("input.txt").read().split(',')]

# cheapest position to align on is probably average position
avg = int(sum(crabs) / len(crabs))

cheapest = 1 << 64
for pos in range(avg-1, avg+2):
    sum = 0
    for c in crabs:
        diff = abs(c - pos)
        sum += int((diff ** 2 + diff) / 2)

    cheapest = min(sum, cheapest)
   
print("--- Day 7: The Treachery of Whales ---")
print(cheapest)
        