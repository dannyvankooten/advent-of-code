from pathlib import Path
from functools import cmp_to_key

def check(a: list, b: list) -> int:
    for i in range(len(a)):
        # if right side ran out of items, return False
        if i >= len(b):
            return 1 

        left = a[i]
        right = b[i]
        if type(left) == int and type(right) == int:
            if left > right:
                return 1
            elif left < right:
                return -1
        elif type(left) == list and type(right) == list:
            result = check(left, right)
            if result != -2:
                return result 
        elif type(left) == int:
            return check([left], right)
        elif type(right) == int:
            return check(left, [right])

    # hacky way to make this function work with what python expects for cmp 
    # we need to signal to the caller to continue walking down the list 
    # yet if this is the outer-most caller, we also need to signal to cmp that a comes before b
    if len(a) == len(b):
        return -2

    return -1


def pt1(input: str) -> int:
    answer = 0
    for i, pairs in enumerate(input.strip().split('\n\n')):
        left, right = map(eval, pairs.split('\n'))
        if check(left, right) == -1:
            answer += (i + 1)
    return answer


def pt2(input: str) -> int:
    d1 = [[2]] 
    d2 = [[6]]
    packets = list(map(eval, filter(None, input.strip().split('\n')))) + [d1, d2]
    packets.sort(key=cmp_to_key(check))
    return packets.index(d1) * packets.index(d2)

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", pt1(input))
    print("pt2: ", pt2(input))

