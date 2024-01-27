from pathlib import Path

def solve(lines, diagonals=False):
    WIDTH = 999
    grid = [0] * WIDTH * WIDTH
    for line in lines:
        start, end = line.split(" -> ")
        start_x, start_y = [int(p) for p in start.split(',')]
        end_x, end_y = [int(p) for p in end.split(',')]
        rcx = 0 if end_x == start_x else 1 if end_x > start_x else -1
        rcy = 0 if end_y == start_y else 1 if end_y > start_y else -1

        if diagonals == False and rcx != 0 and rcy != 0:
            continue

        grid[start_y*WIDTH + start_x] += 1
        while start_x != end_x or start_y != end_y:
            start_x += rcx
            start_y += rcy 
            grid[start_y*WIDTH + start_x] += 1

    return sum(n >= 2 for n in grid)


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    lines = input.split("\n")

    print("--- Day 5: Hydrothermal Venture ---")

    pt1 = solve(lines, diagonals=False)
    print("Part 1: ", pt1, pt1 == 7473)

    pt2 = solve(lines, diagonals=True)
    print("Part 2: ", pt2, pt2 == 24164)