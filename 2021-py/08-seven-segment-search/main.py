displays = open("input.txt").read().split("\n")
#displays = ["acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf"]

def count_overlap(a, b):
    count = 0
    for s1 in a:
        if s1 in b:
            count += 1

    return count

def contains_all(a, b):
    for x in b:
        if x not in a:
            return False 
    
    return True

def find_in_map(m, p):
    for v, pattern in m.items():
        if p == pattern:
            return v
    return None

def part_one():
    count = 0
    for d in displays:
        [signal_patterns, output_patterns] = d.split(" | ")
        signal_patterns = signal_patterns.split(" ")
        output_patterns = output_patterns.split(" ")
        for op in output_patterns:
            if len(op) in [2,3,4,7]:
                count += 1
    return count

def part_two():
    sum = 0

    for d in displays:
        [signal_patterns, output_patterns] = d.split(" | ")
        signal_patterns = [sorted(s) for s in signal_patterns.split(" ")]
        output_patterns = [sorted(s) for s in output_patterns.split(" ")]
        m = {}

        # 1, 4, 7, 8 can be deduced from length
        for p in signal_patterns:
            match len(p):
                case 2:
                    m['1'] = p
                case 4:
                    m['4'] = p
                case 3:
                    m['7'] = p
                case 7:
                    m['8'] = p

        assert len(m) == 4
        
        for p in filter(lambda x: len(x) == 5, signal_patterns):
            if contains_all(p, m['1']):
                m['3'] = p 
            elif count_overlap(p, m['4']) == 3:
                m['5'] = p 
            else:
                m['2'] = p

        assert len(m) == 7

        for p in filter(lambda x: len(x) == 6, signal_patterns):
            if contains_all(p, m['4']):
                m['9'] = p 
            elif contains_all(p, m['5']):
                m['6'] = p
            else:
                m['0'] = p   
        
        assert len(m) == 10, "map does not contain all 10 digits"

        # calculate value from output pattern
        value = 0
        for i, op in enumerate(reversed(output_patterns)):
            value += int(find_in_map(m, op)) * pow(10, i)
    
        sum += value 
    return sum

if __name__ == '__main__':
    pt1 = part_one()
    pt2 = part_two()
    print("--- Day 8: Seven Segment Search ---")
    print("Part 1: ", pt1)
    print("Part 2: ", pt2)