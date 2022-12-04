from pathlib import Path

def parse_section(desc):
    return tuple(map(int, desc.split('-')))

def contains(a, b):
    # a fully contains b
    if a[0] <= b[0] and a[1] >= b[1]:
        return True

    # b fully contains 
    if b[0] <= a[0] and b[1] >= a[1]:
        return True

    return False

def overlap(a, b):
    # if a starts within b
    if a[0] >= b[0] and a[0] <= b[1]:
        return True 
    
    # if b starts within a
    if b[0] >= a[0] and b[0] <= a[1]:
        return True 

    return False

def solve(input) -> int:
    count_contained = 0
    count_overlapped = 0

    for pair in input.split('\n'):
        (a, b) = map(parse_section, pair.split(','))
        count_contained += contains(a,b)
        count_overlapped += overlap(a,b)
       
    return (count_contained, count_overlapped)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    (pt1, pt2) = solve(input)
    print("pt1: ", pt1)
    print("pt2: ", pt2)

