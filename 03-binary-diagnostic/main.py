
def part_one():
    with open("input.txt") as file:
        pos = 0
        gamma = 0
        epsilon = 0

        length = len(file.readline().rstrip())
        print(f"Binary numbers have length {length}")
        file.seek(0)

        for pos in range(0, length):
            ones = 0
            zeros = 0

            for line in file:
                if line[pos] == '1':
                    ones += 1
                else:
                    zeros += 1

            file.seek(0)

            bit_pos = length - pos - 1
            
            if ones > zeros:
                gamma |= 1 << bit_pos
                epsilon |= 0 << bit_pos
            else:
                gamma |= 0 << bit_pos
                epsilon |= 1 << bit_pos

        print(gamma, epsilon, gamma * epsilon)


def search(lines, most_common=True):
    length = len(lines[0].rstrip())
    for pos in range(0, length):
        # count bits in position
        bitcount = [0, 0]
        for i, line in enumerate(lines):
            bit = int(line[pos])
            bitcount[bit] += 1

        print(bitcount)
        
        # read top bitcount
        bit_criteria = 1 if bitcount[1] >= bitcount[0] else 0
        if not most_common:
            bit_criteria = 1 - bit_criteria

        # remove lines not matching bit criteria
        new_lines = []
        for i, line in enumerate(lines):
            bit = int(line[pos])
            if bit == bit_criteria:
                new_lines.append(line)

        lines = new_lines

        if len(lines) == 1:
            return int(lines.pop(), 2)

    return None

def part_two():
    lines = open("input.txt").read().split("\n")
    oxygen_gen_rating = search(lines)
    co2_rating = search(lines, False)
    print(oxygen_gen_rating * co2_rating)
   
    


part_one()
part_two()  