from pathlib import Path

def normalize(b):
    match b:
        case 'X' | 'A': return 0
        case 'Y' | 'B': return 1
        case 'Z' | 'C': return 2

def outcome(opponent, you):
    if you == opponent:
        return 3 
    diff = you - opponent 
    if diff == 1 or diff == -2:
        return 6
    return 0

def score(opponent, you):
    scores = [1, 2, 3]
    return scores[you] + outcome(opponent, you)
    
def part1(input) -> int:
    total_score = 0
    for round in input:
        p1, p2 = map(normalize, round.split(' '))
        total_score += score(p1, p2)
    return total_score

def part2(input) -> int:
    total_score = 0
    for round in input:
        p1, expected_outcome = map(normalize, round.split(' '))
        match expected_outcome:
            case 0: p2 = p1 - 1
            case 1: p2 = p1
            case 2: p2 = p1 + 1 

        # slightly faster than p2 % 3
        if p2 < 0:
            p2 = 2
        elif p2 > 2:
            p2 = 0

        total_score += score(p1, p2)   
    return total_score

if __name__ == '__main__':
    lines = Path("input.txt").read_text().split('\n')
    print("pt1: ", part1(lines))
    print("pt2: ", part2(lines))
