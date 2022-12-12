from pathlib import Path

def draw(cycle: int, x: int):
    """draw CRT pixel"""
    n = cycle % 40
    if n == 0:
        print()

    if abs(x - n) <= 1:
        print('#', end='')
    else:
        print('.', end='')

    #print(f"cycle: {cycle}\tsprite: {x}\tprinting at position {n}")

def signal_strength(cycle: int, x: int) -> int:
    """Returns signal strength of current cycle + register combo"""
    if cycle == 20 or (cycle - 20) % 40 == 0 and cycle <= 220:
        return x * cycle 

    return 0


def solve(input: list[str]) -> int:
    x = 1
    cycles = 0 
    signal_sum = 0
    
    for line in map(lambda l: l.split(), input):
        n = 2 if line[0] == 'addx' else 1
        for _ in range(0, n):
            draw(cycles, x)
            cycles += 1
            signal_sum += signal_strength(cycles, x)
        if n == 2:
            x += int(line[1])

    return signal_sum

if __name__ == '__main__':
    input = Path("input.txt").read_text().strip().split('\n')

    print("\npt1: ", solve(input))

