from pathlib import Path
from re import X

def print_grid(grid):
    for row in grid:
        print("".join(row))
    print()

def solve(input) -> int:
    grid = [[state for state in line] for line in input.split("\n") ]

    for step in range(1, 2**16):
        moved = 0
        
        # move '>' cucumbers
        new_grid = [list(row) for row in grid]
        for y, row in enumerate(grid):
            for x, col in enumerate(row):
                if col != '>':
                    continue 

                next = (x + 1) % len(row)
                if row[next] == '.':
                    new_grid[y][x] = '.'
                    new_grid[y][next] = '>'
                    moved += 1
        

        # move down cucumbers
        grid = [list(row) for row in new_grid]
        for y, row in enumerate(grid):
            for x, col in enumerate(row):
                if col != 'v':
                    continue 

                next = (y + 1) % len(grid)
                if grid[next][x] == '.':
                    new_grid[y][x], new_grid[next][x] = new_grid[next][x], new_grid[y][x]
                    moved += 1

        if moved == 0:
            return step

        grid = new_grid 

    return 0

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    
    pt1 = solve(input)
    print(pt1)
