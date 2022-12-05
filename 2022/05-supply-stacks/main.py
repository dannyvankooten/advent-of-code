from pathlib import Path
from collections import defaultdict
import re

def solve(input, retain_order=False) -> int:
    [stacks_config, instructions] = input.split('\n\n')
    stacks = defaultdict(lambda: [])

    # parse stacks
    for line in list(reversed(stacks_config.split('\n')))[1:]:
        line = line.replace('    ', ' [ ]').replace('[', '').replace(']', ',').replace(' ', '')
        for stack_idx, crate in enumerate(line.split(',')):
            if crate:
                stacks[stack_idx + 1].append(crate)

    # parse instructions
    rx = re.compile(r"move (\d+) from (\d+) to (\d+)")
    for line in instructions.split('\n'):
        (n, src, dest) = [int(x) for x in rx.match(line).groups()]
        crates = []
        while n > 0:
            crates.append(stacks[src].pop())
            n -= 1

        if retain_order:
            crates = reversed(crates)

        stacks[dest].extend(crates)
       
    answer = []
    for idx in stacks:
        answer.append(stacks[idx][-1])

    return "".join(answer)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input))
    print("pt2: ", solve(input, retain_order=True))

