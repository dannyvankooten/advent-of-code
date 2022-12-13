from pathlib import Path 

def solve(input, n):
    for i in range(0, len(input) -n):
        if len(set(input[i:i+n])) == n:
            return i+n
    return None


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    print("pt1: ", solve(input, 4))
