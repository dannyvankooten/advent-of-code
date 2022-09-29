from copy import deepcopy
from pathlib import Path
import sys
from typing import Counter 

def parse(lines):
    algorithm = lines.pop(0).strip()
    lines.pop(0)
    image = [ 
        [c for c in l.strip()] for l in lines
    ]
    return algorithm, image

def pad(image, n=3, pad_value='.'):
    size = len(image) + (n * 2)
    image = ([[pad_value] * size] * n) + ([[pad_value] * n + row + [pad_value] * n for row in image]) + ([[pad_value] * size] * n)
    assert(len(image) == len(image[0]) == size)
    return image

def encoding_to_binary(enc: list[str]):
    assert(len(enc) == 9)
    number = 0
    for el in enc:
        bit = 1 if el == '#' else 0
        number = (number << 1) | bit
   
    return number

def value_at(image, row, col, default='.'):
    try:
        value = image[row][col]
    except IndexError:
        value = default
    return value

def read_3x3(image, row, col, default='.'):
    encoding = [value_at(image, yt, xt, default) for yt in range(row-1, row+2) for xt in range(col-1, col+2)]
    return encoding

# The issue is that values outside the grid flip between '#' and '.'
# Yet we should still grow the grid every step

def solve(input, steps):
    algorithm, image = parse(input)
    assert(len(algorithm) == 512)
    assert(len(image) == len(image[0]))
    image = pad(image, 50, '.')
    
    for i in range(steps):
        default_value = '.' if i % 2 == 0 else algorithm[0]
        size = len(image)
        new_image = list(map(list, image))

        for row in range(size):
            for col in range(size):
                encoding = read_3x3(image, row, col, default_value)
                n = encoding_to_binary(encoding)
                replacement = algorithm[n]
                new_image[row][col] = replacement
        
        image = list(map(list, new_image))        

   
    return Counter([col for row in image for col in row])['#']

if __name__ == '__main__':
    input = Path("input.txt").read_text().strip().split("\n")
    
    print("part 1: ", solve(list(input), 2))
    print("part 2: ", solve(input, 50))
