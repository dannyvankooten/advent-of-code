from collections import defaultdict
from pathlib import Path

input = Path('input.txt').read_text().split("\n")

# parse template
template = input.pop(0).strip()

# parse rules
input.pop(0);
rules = [l.split(" -> ") for l in input]


def apply_steps(n):
    global template 

    pairs = defaultdict(int)
    char_count = defaultdict(int)
    char_count[template[0]] = 1
    for i in range(len(template) - 1):
        key = template[i] + template[i+1]
        pairs[key] += 1
        char_count[template[i+1]] += 1

    # steps
    for _ in range(0, n):
        new_pairs = pairs.copy()

        for (pair, char_to_insert) in rules:
            left_side = pair[0] + char_to_insert
            right_side = char_to_insert + pair[1]

            new_pairs.setdefault(pair, 0)
            new_pairs.setdefault(left_side, 0)
            new_pairs.setdefault(right_side, 0)
            if pair in pairs and pairs[pair] > 0: 
                value = pairs[pair]

                # first, increment letter count
                if char_to_insert not in char_count:
                    char_count[char_to_insert] = value 
                else:
                    char_count[char_to_insert] += value 

                # then, update pairs
                new_pairs[pair] -= value
                new_pairs[pair[0] + char_to_insert] += value
                new_pairs[char_to_insert + pair[1]] += value 

        pairs = new_pairs  

    # count elements in template
    most_common = max(char_count.values())
    least_common = min(char_count.values())
    return most_common - least_common

# print answer
pt1 = apply_steps(10)
print("Part 1: ", pt1)
assert(pt1 == 2947)

pt2 = apply_steps(40)
print("Part 2: ", pt2)
assert(pt2 == 3232426226464)