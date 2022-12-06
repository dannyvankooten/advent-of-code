from pathlib import Path

def solve(input, n) -> int:
    for i in range(0, len(input) - n):
        if len(set(input[i:i+n])) == n:
            return i + n

    raise Exception("No start packet") 

if __name__ == '__main__':
    input = Path("input.txt").read_text()
    assert(solve("bvwbjplbgvbhsrlpgdmjqwftvncz", 4) == 5)
    assert(solve("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 4) == 11)

    print("pt1: ", solve(input, 4))
    print("pt2: ", solve(input, 14))

