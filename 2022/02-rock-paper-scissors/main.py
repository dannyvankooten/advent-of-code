from pathlib import Path

ROCK = 0
PAPER = 1
SCISSORS = 2

def normalize(b):
    match b:
        case 'X' | 'A': return ROCK
        case 'Y' | 'B': return PAPER 
        case 'Z' | 'C': return SCISSORS

def outcome(opponent, you):
    if you == opponent:
        return 3 
    if ((you - opponent) % 3 == 1):
        return 6 
    return 0

def score(opponent, you):
    scores = [1, 2, 3]
    return scores[you] + outcome(opponent, you)
    
def part1(input) -> int:
    total_score = 0
    for round in input:
        [p1, p2] = map(normalize, round.split(' '))
        total_score += score(p1, p2)
    return total_score

def part2(input) -> int:
    total_score = 0
    for round in input:
        [p1, expected_outcome] = round.split(' ')
        p1 = normalize(p1)
        match expected_outcome:
            case 'Y': p2 = p1
            case 'X': p2 = (p1 - 1) % 3
            case 'Z': p2 = (p1 + 1) % 3
        total_score += score(p1, p2)   
    return total_score

if __name__ == '__main__':
    lines = Path("input.txt").read_text().split('\n')
    print("pt1: ", part1(lines))
    print("pt2: ", part2(lines))
