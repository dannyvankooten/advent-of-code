
from pathlib import Path

def part_one():
    lines = Path("input.txt").read_text().strip().split("\n")
    length = len(lines[0])
    pos = 0
    gamma = 0
    epsilon = 0
    
    for pos in range(length):
        bits = [s[pos] for s in lines]
        bit_pos = length - pos - 1
        
        if bits.count('1') >= bits.count('0'):
            gamma |= 1 << bit_pos
            epsilon |= 0 << bit_pos
        else:
            gamma |= 0 << bit_pos
            epsilon |= 1 << bit_pos

    return gamma * epsilon
    

def search(lines, most_common=True):
    length = len(lines[0])

    for pos in range(length):
        bits = [s[pos] for s in lines]
        
        # read top bitcount
        bit_criteria = 1 if bits.count('1') >= bits.count('0') else 0
        if not most_common:
            bit_criteria = 1 - bit_criteria

        # remove lines not matching bit criteria
        lines = [line for line in lines if int(line[pos]) == bit_criteria]
        if len(lines) == 1:
            return int(lines[0], 2)

    return None

def part_two():
    lines = Path("input.txt").read_text().strip().split("\n")
    oxygen_gen_rating = search(lines)
    co2_rating = search(lines, False)
    return oxygen_gen_rating * co2_rating
   
pt1 = part_one()
print(pt1, pt1 == 4160394)

pt2 = part_two()  
print(pt2, pt2 == 4125600)