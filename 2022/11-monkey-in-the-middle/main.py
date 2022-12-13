from pathlib import Path
from dataclasses import dataclass 
from typing import Callable 
from copy import deepcopy 

@dataclass
class Monkey:
    __slots__ = ("items", "operation", "test", "targets")
    items: list[int]
    operation: Callable[[int], int]
    test: int
    targets: tuple[int, int]


def solve(monkeys: list, pt1: bool) -> int:
    # for part 2, find the factor we can safely divide by (least common multiple)
    # if numbers are not all prime, we can find a smaller number here
    # but this works and is easier, so it's okay.
    divisor = 1
    for m in monkeys:
        divisor *= m.test 

    # each monkey starts at activity level 0
    activities = [0 for _ in monkeys] 
    n = 20 if pt1 else 10000
    for _ in range(0, n):
        for i, m in enumerate(monkeys):
            for old in m.items:
                # update worry level of this item
                new = m.operation(old)

                # divide by 3 or lcm
                if pt1:
                    new //= 3
                else:
                    new %= divisor 

                # pass to target monkey
                test = (new % m.test) == 0
                target_monkey = m.targets[test]
                monkeys[target_monkey].items.append(new)

            activities[i] += len(m.items)
            m.items.clear()

    activities.sort()
    return activities[-1] * activities[-2]


def parse(input) -> list :
    monkeys = []
    for m in input.strip().split('\n\n'):
        lines = list(map(lambda l: l.strip(), m.split('\n')))
        items = list(map(int,lines[1][len("Starting items: "):].split(', ')))
        operation = eval("lambda old: " + lines[2][len("Operation: new = "):])
        test = int(lines[3][len("Test: divisible by "):])
        if_true = int(lines[4][len("If true: throw to monkey "):])
        if_false = int(lines[5][len("If false: throw to monkey "):])
        monkeys.append(Monkey(items, operation, test, (if_false, if_true)))
    return monkeys 


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    monkeys = parse(input)
    print("pt1: ", solve(deepcopy(monkeys), True))
    print("pt2: ", solve(monkeys, False))

