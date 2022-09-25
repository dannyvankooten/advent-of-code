import sys 

def parse(input):
    return input.split('\n')

def solve(input):
    # TODO: Write puzzle code here
    return 0

if __name__ == '__main__':
    for path in sys.argv[1:]:
        with open(path) as f:
            input = f.read().strip()
            data = parse(input)
            print(solve(data))
