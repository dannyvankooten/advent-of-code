from pathlib import Path

def count_visible(grid, x, y) -> tuple:
    tree_height = grid[y][x] 
    height, width = len(grid), len(grid[0])
    coords = (x, y)

    visible_from_outside = False 
    score = 1
    for dx, dy in [ (0, 1), (1, 0), (-1, 0), (0, -1) ]: 
        x, y = coords
        visible = 0 

        # walk to outer edge
        while 0 < x < width - 1 and 0 < y < height - 1:
            visible += 1

            # stop if we encouter larger or equal tree
            if grid[y + dy][x + dx] >= tree_height:
                break 

            x += dx 
            y += dy 
        
        # if we made it to the edge, tree is visible from outside
        if (x <= 0 or x >= width - 1 or y <= 0 or y
            >= height - 1):
            visible_from_outside = True 
        
        # add current direction line of sight to score
        score *= visible 

    return score, visible_from_outside

def solve(input) -> tuple:
    grid = [[int(c) for c in line] for line in input.strip().split('\n')]
    height, width = len(grid), len(grid[0])

    visible_from_outside = 0
    max_score = 0
    for y in range(0, height):
        for x in range(0, width):
            score, visible = count_visible(grid, x, y)
            if score > max_score:
                max_score = score 

            if visible:
                visible_from_outside += 1

    return (visible_from_outside, max_score)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    pt1, pt2 = solve(input)
    print("pt1: ", pt1)
    print("pt2: ", pt2)

