from pathlib import Path


lines = Path("input.txt").read_text().strip().split("\n")
pairs = {
    '(': ')',
    '{': '}',
    '<': '>',
    '[': ']',
}
def pt1():
    points = {
        ')': 3,
        ']': 57,
        '}': 1197,
        '>': 25137,
    }
    score = 0
    for i, l in enumerate(lines):
        expect = []
        for c in list(l):
            match c:
                case '{' | '[' | '(' | '<':
                    expect.append(pairs[c])
                case '}' | ']' | ')' | '>':
                    if expect[-1] != c:
                        score += points[c]
                        break
                    expect.pop(-1)

    return score

def autocomplete_score(line):
    expect = []
    points = {
        ')': 1,
        ']': 2,
        '}': 3,
        '>': 4,
    }
    score = 0
    for c in list(line):
        match c:
            case '{' | '[' | '(' | '<':
                expect.append(pairs[c])
            case '}' | ']' | ')' | '>':
                if expect[-1] != c:
                    return None
                expect.pop(-1)

    for c in reversed(expect):
        score = (score * 5) + points[c]
    return score

def pt2():
    scores = []
    for line in lines:
        scores.append(autocomplete_score(line))
    
    scores = list(filter(lambda s: s is not None, scores))
    scores.sort()
    middle_score = scores[int(len(scores) / 2)]
    return middle_score


if __name__ == '__main__':
    print("--- Day 10: Syntax Scoring ---")
    print("Part 1: ", pt1())
    print("Part 2: ", pt2())