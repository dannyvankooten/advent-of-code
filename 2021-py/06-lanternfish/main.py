from pathlib import Path

fish = [int(n) for n in Path("input.txt").read_text().split(",")]

# create list of pregnancies so we know how many fish are bown on each cycle day
pregnancies = [0] * 7
for f in fish:
    pregnancies[f] += 1

# babies are fish that have yet to mature
babies = [0] * 7

num_fish = len(fish)
for d in range(0, 256):
    day_in_cycle = d % 7

    # a new fish is born
    num_fish += pregnancies[day_in_cycle]

    # this fish will start producing babies after 9 days
    babies[(d+2) % 7] += pregnancies[day_in_cycle]

    pregnancies[day_in_cycle] += babies[day_in_cycle]
    babies[day_in_cycle] = 0
   

print("--- Day 6: Lanternfish ---")
print(num_fish, num_fish == 1595779846729)