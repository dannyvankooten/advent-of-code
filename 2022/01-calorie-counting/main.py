from pathlib import Path

def solve(input) -> int:
    elves = []
    elf_sum = 0
    for line in input.split('\n'):
        line = line.strip()
        
        if line == "":
            elves.append(elf_sum)
            elf_sum = 0
        else:
            elf_sum += int(line)

    return sorted(elves)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    elves = solve(input)
    print("part 1", elves[-1])
    print("part 2", sum(elves[-3:]))
