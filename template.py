import sys 

input_file = "input_test.txt" if len(sys.argv) > 1 and sys.argv[1] == "--test" else "input.txt"
lines = open(input_file).read().split('\n')

print(lines)