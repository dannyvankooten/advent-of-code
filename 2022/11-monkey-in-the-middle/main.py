from pathlib import Path
from dataclasses import dataclass 
from collections import defaultdict 

from pprint import pprint 

@dataclass
class Monkey:
    items: list[int]
    operation: str
    test: int
    target: tuple[int, int]
     

def solve(monkeys: list, pt: int) -> int:
    pprint(monkeys)
  
    # for part 2, find the factor we can safely divide by 
    divisor = 1
    for m in monkeys:
        divisor *= m.test 

    # each monkey starts at activity level 0
    activities = [0 for _ in monkeys] 
    n = 20 if pt == 1 else 10000
    for _ in range(0, n):
        for i, m in enumerate(monkeys):
            for old in m.items:
                activities[i] += 1

                # update worry level of this item
                new = eval(m.operation)
                if pt == 1:
                    new //= 3
                else:
                    new %= divisor 
                test = (new % m.test) == 0
                target_monkey = m.target[test]
                monkeys[target_monkey].items.append(new)

            m.items.clear()

    activities.sort()

    return activities[-1] * activities[-2]


def parse(input) -> list :
    monkeys = []
    for m in input.strip().split('\n\n'):
        lines = list(map(lambda l: l.strip(), m.split('\n')))
        items = list(map(int,lines[1][len("Starting items: "):].split(', ')))
        operation = lines[2][len("Operation: new = "):]
        test = int(lines[3][len("Test: divisible by "):])
        if_true = int(lines[4][len("If true: throw to monkey "):])
        if_false = int(lines[5][len("If false: throw to monkey "):])
        target = (if_false, if_true)
        monkeys.append(Monkey(items, operation, test, target))
    return monkeys 


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    monkeys = parse(input)
    print("pt1: ", solve(monkeys, 1))

    monkeys = parse(input)
    print("pt2: ", solve(monkeys, 2))

