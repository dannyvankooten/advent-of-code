from math import floor, ceil
from pathlib import Path 
import sys 
import itertools
from ast import literal_eval

def explode(snailfish):
    i = 0 
    depth = 0
    for i in range(len(snailfish)):
        c = snailfish[i]

        if c == '[':
            depth += 1
        elif c == ']':
            depth -= 1

        if depth == 5:
            pair_start = i
            while snailfish[i+1] != ']':
                i += 1
            pair_end = i + 1

            left, right = map(int, snailfish[pair_start+1:pair_end].split(","))

            # replace pair with 0
            snailfish = snailfish[:pair_start] + "0" + snailfish[pair_end+1:]
            pair_end = pair_start + 1
          
            # add left value to first regular number to the left 
            for j in range(pair_start-1, 0, -1):
                if snailfish[j].isdigit() == False:
                    continue 

                # rewind to start of digit
                while snailfish[j].isdigit():
                    j -= 1

                num, start, end = parse_digit(snailfish[j:])
                if num != None:
                    if num+left >= 10:
                        pair_end += 1
                    
                    snailfish = snailfish[:j+start] + str(num + left) + snailfish[j+end:]
                    break

            # add right value to first regular number to the right
            # skip forward to next number
            num, start, e = parse_digit(snailfish[pair_end:])
            if num != None:
                snailfish = snailfish[:pair_end+start] + str(num + right) + snailfish[pair_end+e:]

            
            return snailfish, True


    return snailfish, False


def parse_digit(s):
    start = 0
    while start < len(s) and s[start].isdigit() == False:
        start += 1

    if (start == len(s)):
        return None, None, None

    number = 0
    end = start
    while end < len(s) and s[end].isdigit():
        number = number * 10 + int(s[end])
        end += 1

    return number, start, end


def split(snailfish): 
    i = 0  

    for i in range(len(snailfish)):
        num, start, end = parse_digit(snailfish[i:])
        if num != None and num >= 10:
            left = floor(num / 2)
            right = ceil(num / 2)
            snailfish = snailfish[0:i+start] + "[" + str(left) + "," + str(right) + "]" + snailfish[i+end:]
            return snailfish, True

    return snailfish, False 



def reduce(str):
    while True:
        str, changed = explode(str)
        if changed:
            continue 

        str, changed = split(str)
        if changed:
            continue 

        break
        
    return str

def add(left: str, right: str):
    return reduce("[" + left + "," + right + "]")


def magnitude(x):
    if isinstance(x, int):
        return x
    return 3 * magnitude(x[0]) + 2 * magnitude(x[1])


if __name__ == '__main__':
    print("--- Day 18: Snailfish ---")
    input = Path("input.txt").read_text()
    lines = input.split("\n")
    
    n = lines.pop(0).strip()
    for l in lines:
        l = l.strip()
        n = add(n, l)
    
    print("Part 1: ", magnitude(literal_eval(n)))

    lines = input.split("\n")
    print("Part 2: ", max(magnitude(literal_eval(add(a, b))) for a, b in itertools.permutations(lines, 2)),)
        