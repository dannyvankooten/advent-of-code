from collections import defaultdict
from pathlib import Path

def parse(input):
    neighbors = defaultdict(list)
    for line in input.split("\n"):
        a, b = line.strip().split('-')
        neighbors[a] += [b]
        neighbors[b] += [a]

    return neighbors
    
def solve(neighbors, part, seen : dict, cave : str):
    if cave == 'end': 
        return 1 

    if cave in seen:
        if cave == 'start':
            return 0
        if cave.islower():
            if part == 1: 
                return 0
            else:
                part = 1

    seen[cave] = True

    return sum(solve(neighbors, part, seen.copy(), n) for n in neighbors[cave])


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    neighbors = parse(input)
    print("--- Day 12: Passage Pathing ---")
    print("Part 1: ", solve(neighbors, part=1, seen=dict(), cave='start'))
    print("Part 2: ", solve(neighbors, part=2, seen=dict(), cave='start'))