from pathlib import Path


def parse(lines):
    algorithm = [1 if c == '#' else 0 for c in lines.pop(0).strip()]
    lines.pop(0)
    image = [ 
        [1 if c == '#' else 0 for c in l.strip()] for l in lines
    ]
    return algorithm, image


def pad(image, n=3, pad_value=0):
    size = len(image) + (n * 2)
    image = ([[pad_value] * size] * n) + ([[pad_value] * n + row + [pad_value] * n for row in image]) + ([[pad_value] * size] * n)
    return image


def read_3x3(image, row: int, col: int, default=0):
    number = 0
    size = len(image)
    for r in range(row-1, row+2):
        for c in range(col-1, col+2):
            if r < 0 or r >= size - 1 or c < 0 or c >= size - 1:
                bit = default
            else:
                bit = image[r][c]
            number = (number << 1) | bit

    return number

# The issue is that values outside the grid flip between '#' and '.'
# Yet we should still grow the grid every step

def solve(input, steps):
    algorithm, image = parse(input)
    assert(len(algorithm) == 512)
    assert(len(image) == len(image[0]))

    for i in range(steps):
        default_value = 0 if i % 2 == 0 else algorithm[0]
        image = pad(image, 1, default_value)
        size = len(image)
        new_image = list(map(list, image))

        for row in range(size):
            for col in range(size):
                replacement_index = read_3x3(image, row, col, default_value)
                new_image[row][col] = algorithm[replacement_index]
        
        image = new_image      

    return sum([col for row in image for col in row])

if __name__ == '__main__':
    print("--- Day 20: Trench Map ---")
    input = Path("input.txt").read_text().strip().split("\n")
    print("part 1: ", solve(list(input), 2))
    print("part 2: ", solve(input, 50))