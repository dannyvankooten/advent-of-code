input = [list(map(int, x)) for x in open('input.txt').read().split("\n")]

grid = input
H = len(grid)
W = len(grid[0])
print(f"heightmap {W}x{H}")

neighbors = [
    (0, 1),     # right
    (0, -1),    # left
    (1, 0),     # top,
    (-1, 0),    # down 
]
risk_sum = 0

for y1 in range(0, H):
    for x1 in range(0, W):
        height = grid[y1][x1]
        
        for dir in neighbors:
            y2 = y1 + dir[0]
            x2 = x1 + dir[1]

            if 0 <= y2 < H and 0 <= x2 < W and height >= grid[y2][x2]:
                break
        else:     
            risk_sum += 1 + height
        

print(risk_sum)