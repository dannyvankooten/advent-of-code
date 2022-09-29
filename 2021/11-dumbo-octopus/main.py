input = open("input.txt").read().split("\n")
H = len(input)
W = len(input[0])

class Octopus:
    flashed: False 

    def __init__(self, energy):
        self.energy_level = energy

    def __repr__(self): 
        return f"{self.energy_level}"

grid = [Octopus(int(x)) for line in input for x in line]
d = [
    (0, 1),
    (0, -1),
    (1, 0),
    (-1, 0),
    (1, 1),
    (-1, 1),
    (1, -1),
    (-1, -1),
]

def maybe_flash(x, y):
    flash_count = 0
    o = grid[y*W+x]
    if o.energy_level > 9 and o.flashed == False:
        o.flashed = True
        flash_count += 1    

        # increment neighbors
        for (dx, dy) in d:
            x2 = x + dx 
            y2 = y + dy 
            if 0 <= x2 < W and 0 <= y2 < H:
                n = grid[y2*W+x2]
                if n.flashed == False:
                    grid[y2*W+x2].energy_level += 1
                    flash_count += maybe_flash(x2, y2)  

        # reset energy level to 0
        o.energy_level = 0
        
    return flash_count

flashes = 0
for s in range(0, 1000):
    flashed_this_step = 0

    for o in grid:
        o.flashed = False
        o.energy_level += 1

    for y in range(0, H):
        for x in range(0, W):
            flashed_this_step += maybe_flash(x, y)
            
    flashes += flashed_this_step
    
    if s == 100:
        print("Part 1: ", flashes)
        assert(flashes == 1706)
        
    if flashed_this_step == H*W:
        print("Part 2:", s+1)
        assert(s+1 == 265)
        break


    
