input = open("input.txt").read()
lines = input.split("\n")
print(lines)

W = 999
H = 999
grid = [0] * W * H

def print_grid():
    for y in range(0, H):
        for x in range(0, W):
            print(grid[y * W + x], end="")
        print("")

for line in lines:
    [start, end] = line.split(" -> ")
    [start_x, start_y] = [int(p) for p in start.split(',')]
    [end_x, end_y] = [int(p) for p in end.split(',')]

    rcx = 0 if end_x == start_x else 1 if end_x > start_x else -1
    rcy = 0 if end_y == start_y else 1 if end_y > start_y else -1
    print(f"Walking line from ({start_x}, {start_y}) to ({end_x}, {end_y})")
    print(f"rcx = {rcx}, rcy = {rcy})")

    # if rcx != 0 and rcy != 0:
    #     print("Skipping diagonal line")
    #     continue

    grid[start_y*W + start_x] += 1
    while start_x != end_x or start_y != end_y:
        start_x += rcx
        start_y += rcy 

        grid[start_y*W + start_x] += 1

print_grid()

count = 0
for n in grid:
    if n >= 2:
        count += 1   
print(count)
