input = [list(map(int, x)) for x in open('input.txt').read().split("\n")]

grid = input
H = len(grid)
W = len(grid[0])
neighbors = [
    (0, 1),     # right
    (0, -1),    # left
    (1, 0),     # top,
    (-1, 0),    # down 
]

def pt1():
    risk_sum = 0
    for y1 in range(0, H):
        for x1 in range(0, W):
            height = grid[y1][x1]
            
            for (dx, dy) in neighbors:
                y2 = y1 + dy
                x2 = x1 + dx

                if 0 <= y2 < H and 0 <= x2 < W and height >= grid[y2][x2]:
                    break
            else:     
                risk_sum += 1 + height
            
    print(risk_sum)



def add_neighbors_to_bassin(bassin, x, y):
    for (dx, dy) in neighbors:
        y2 = y + dy 
        x2 = x + dx

        if 0 <= y2 < H and 0 <= x2 < W and grid[y2][x2] < 9:
            p = (x2, y2)
            if p not in bassin:
                bassin.append(p)  
                bassin = add_neighbors_to_bassin(bassin, x2, y2)              
        
    return bassin





def pt2():
    bassins = []
    for y1 in range(0, H):
        for x1 in range(0, W):
            height = grid[y1][x1]
            
            for (dx, dy) in neighbors:
                y2 = y1 + dy
                x2 = x1 + dx
                if 0 <= y2 < H and 0 <= x2 < W and height >= grid[y2][x2]:
                    break
            else:     
                # this is a low point
                bassin = add_neighbors_to_bassin([(x1, y1)], x1, y1)
                bassins.append(bassin)

    bassins.sort(key=len, reverse=True)
    a = 1
    for b in bassins[0:3]:
        a *= len(b)
    print(a)



if __name__ == '__main__':
    pt1()
    pt2()