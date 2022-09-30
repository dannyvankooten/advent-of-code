from pathlib import Path
from parse import parse

def parse_input(input):
    lines = input.split("\n")
    instructions = []
    for line in lines:
        vars = parse("{status} x={x_start:d}..{x_end:d},y={y_start:d}..{y_end:d},z={z_start:d}..{z_end:d}", line).named
        instructions.append((1 if vars['status'] == 'on' else 0, max(vars['x_start'], -50), min(vars['x_end'], 50), max(vars['y_start'], -50), min(vars['y_end'], 50), max(vars['z_start'], -50), min(vars['z_end'], 50)))
    return instructions


def solve(input):
    instructions = parse_input(input)

    grid = [[[0 for _ in range(102)] for _ in range(102)] for _ in range(102)]
    for (status, x1, x2, y1, y2, z1, z2) in instructions:
        for z in range(z1, z2+1):
            for y in range(y1, y2+1):
                for x in range(x1, x2+1):
                    grid[z+50][y+50][x+50] = status



    return sum([xs for zs in grid for ys in zs for xs in ys])

if __name__ == '__main__':
    input = Path("input.txt").read_text()

    pt1 = solve(input)
    print(pt1, pt1 == 567496)
