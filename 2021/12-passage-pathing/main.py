from collections import defaultdict
import sys 

def parse(input_file):
    neighbors = defaultdict(list)

    with open(input_file) as f:
        for line in f.readlines():
            a, b = line.strip().split('-')
            neighbors[a] += [b]
            neighbors[b] += [a]

    return neighbors
    
def solve(neighbors, part, seen = [], cave='start'):
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

    return sum(solve(neighbors, part, seen + [cave], n) for n in neighbors[cave])


if __name__ == '__main__':
    for f in sys.argv[1:]:
        neighbors = parse(f)
        print(neighbors)

        print(solve(neighbors, part=1), solve(neighbors, part=2))