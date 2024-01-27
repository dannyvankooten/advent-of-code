from pathlib import Path

def solve(input):
    elves = [sum([int(x) for x in elf.split('\n')]) for elf in input.split('\n\n')]
    elves.sort()
    return elves

if __name__ == '__main__':
    input = Path("input.txt").read_text().strip()
    elves = solve(input)
    print("part 1", elves[-1])
    print("part 2", sum(elves[-3:]))
