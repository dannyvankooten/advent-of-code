import re

input = open('input.txt').read().split('\n')

# parse template
template = input.pop(0).strip()

# skip empty line
input.pop(0);

# parse rules
rules = []
for l in input:
    (pair, el) = l.split(" -> ")
    rules.append((pair, el))

print("Parsing ready")
pairs = {}
char_count = { template[0]: 1 }
for i in range(len(template) - 1):
    key = template[i] + template[i+1]

    if key not in pairs:
        pairs[key] = 1
    else:
        pairs[key] += 1
   
    try:
        char_count[template[i+1]] += 1
    except KeyError:
        char_count[template[i+1]] = 1

template = list(template)

# steps
steps = 40
for n in range(0, steps):
    new_pairs = pairs.copy()

    for (pair, char_to_insert) in rules:
        left_side = pair[0] + char_to_insert
        right_side = char_to_insert + pair[1]

        new_pairs.setdefault(pair, 0)
        new_pairs.setdefault(left_side, 0)
        new_pairs.setdefault(right_side, 0)
        if pair in pairs: 
            if pairs[pair] > 0:
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

# print answer
print("Answer: ", most_common - least_common) 
