from pathlib import Path

def draw(cycle, x):
    n = cycle % 40
    if abs(x - n) <= 1:
        print('#', end='')
    else:
        print('.', end='')

    #print(f"cycle: {cycle}\tsprite: {x}\tprinting at position {n}")
    if cycle > 0 and n == 0:
        print()

def signal_strength(cycle, x) -> int:
    if cycle == 20 or (cycle - 20) % 40 == 0:
        return x * cycle 

    return 0

def solve(input) -> int:
    x = 1
    cycles = 0 
    signal_sum = 0
    for line in input.strip().split('\n'):
        line = line.split()
        if line[0] == 'noop':
            draw(cycles, x)
            cycles += 1
            signal_sum += signal_strength(cycles, x)
        elif line[0] == 'addx':
            for _ in range(0, 2):
                draw(cycles, x)
                cycles += 1
                signal_sum += signal_strength(cycles, x)

            x += int(line[1])

    return signal_sum

if __name__ == '__main__':
    #input = Path("input_test.txt").read_text()
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input))

