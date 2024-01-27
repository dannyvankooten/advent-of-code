from pathlib import Path

def solve(input):
    # create sufficiently sized grid filled with air pockets
    width, height = 500, 180
    grid = [[ '.' ] * width for _ in range(height)]

    # helper to translate x into our grid offset
    translate_x = lambda x: x - (500 - width // 2)

    # add rocks
    abyss_y = 0
    for line in input.split('\n'):
        steps = list(map(lambda s: list(map(int, s.split(','))), line.split(' -> ')))
        for (x, y), (dest_x, dest_y) in zip(steps, steps[1:]):
            while True:
                grid[y][translate_x(x)] = '#'
               
                if x != dest_x:
                    x += 1 if x < dest_x else -1 
                elif y != dest_y:
                    y += 1 if y < dest_y else -1 
                else: 
                    break

                # keep track of max y position so we know where abyss is 
                if y > abyss_y:
                    abyss_y = y
                
    # create floor 
    for x in range(width):
        grid[abyss_y+2][x] = '#'
    
    # let it rain 
    resting_sands = 0
    pt1 = 0
    sand_source = (translate_x(500), 0)
    path = [sand_source]
    while True:
        x, y = path.pop()
        while True:
            if pt1 == 0 and y > abyss_y:
                pt1 = resting_sands

            path.append((x, y))
            if grid[y+1][x] == '.':
                y += 1 
            elif grid[y+1][x-1] == '.':
                y += 1
                x -= 1 
            elif grid[y+1][x+1] == '.':
                y += 1 
                x += 1 
            else:
                break 


        # place sand 
        grid[y][x] = 'o'
        resting_sands += 1
        path.pop()
        
        # stop if we blocked source of sand
        if (x, y) == sand_source:
            break
    
    return pt1, resting_sands


if __name__ == '__main__':
    input = Path("input.txt").read_text().strip()
    pt1, pt2 = solve(input)
    print("pt1: ", pt1)
    print("pt2: ", pt2)

