from pathlib import Path
from collections import defaultdict

def solve(input, retain_order=False) -> str:
    stacks_config, instructions = input.split('\n\n')
    stacks = defaultdict(lambda: [])
    for line in reversed(stacks_config.split('\n')[:-1]):
        # change line into format like: X]A]O]]C
        #line = line.replace('    ', '  ]').replace('[', '')
        for stack_idx, crate in enumerate(line[1::4]):
            crate = crate.strip()
            if crate:
                stacks[stack_idx + 1].append(crate)

    # parse instructions
    for line in instructions.split('\n'):
        _, n, _, src, _, dest = line.split()
        n, src, dest = int(n), int(src), int(dest)
        crates = stacks[src][-n:]
        if not retain_order:
            crates = reversed(crates)
        del stacks[src][-n:]
        stacks[dest].extend(crates)

    # get top crate from each stack
    return "".join([stacks[s][-1] for s in stacks])

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input))
    print("pt2: ", solve(input, retain_order=True))

