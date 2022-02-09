lines = open('input.txt').read().split('\n')
points = {
    ')': 3,
    ']': 57,
    '}': 1197,
    '>': 25137,
}

score = 0
for l in lines:

    open = []
    for c in list(l):
        match c:
            case '{':
                open.append('{')
            case '[':
                open.append('[')

            case '(':
                open.append('(')

            case '<':
                open.append('<')

            case '}':
                if open[-1] != '{':
                    score += points[c]
                    break
                open.pop(-1)

            case ']':
                if open[-1] != '[':
                    score += points[c]
                    break
                open.pop(-1)

            case ')':
                if open[-1] != '(':
                    score += points[c]
                    break
                open.pop(-1)

            case '>':
                if open[-1] != '<':
                    score += points[c]
                    break
                open.pop(-1)

print(score)
