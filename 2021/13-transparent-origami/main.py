from collections import Counter
from pathlib import Path
import re

input = Path("input.txt").read_text().strip().split("\n")
max_width = 1350
max_height = 900
width = 0
height = 0

paper = [[ 0 for _ in range(max_width )] for _ in range(max_height)]


def print_paper():
    print("")
    for row in paper[0:height+1]:
        for col in row[0:width+1]:
            print('#' if col else ' ', end='')
        print("")

folds = 0
for line in input:
    if line.startswith("fold along "):
        # determine axes to fold over
        # for x axes, every '#' larger than this x should move to new position
        # for y axes, every '#' larger than this y should move to new position
        # new position is = width-x or height - y
        (ax, value) = line[len("fold along "):].split("=")
        value = int(value)
        match ax:
            case 'y':
                for y in range(value, height+1):
                    for x in range(0, width+1):
                        if paper[y][x]:
                            paper[(y - (y - value ) * 2)][x] = 1
                
                height = value - 1
            case 'x':
                for y in range(0, height+1):
                    for x in range(value, width+1):
                        if paper[y][x]:
                            paper[y][(x - (x - value ) * 2)] = 1

                width = value - 1
    else:
        if line == "": continue

        (col, row) = [int (v) for v in line.strip().split(',')]
        paper[row][col] = 1
        width = max(width, col)
        height = max(height, row)


print("--- Day 13: Transparent Origami ---")
dot_count = sum([col for row in paper[0:height+1] for col in row[0:width+1] ])
pt2 = print_paper()