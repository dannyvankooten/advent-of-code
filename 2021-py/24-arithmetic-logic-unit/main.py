from collections import defaultdict
from logging import log
import logging
from pathlib import Path
from random import random

def next(model_number):
    i = len(model_number) - 1
    while i > 3:
        model_number[i] -= 1

        # skip zeroes and reduce next least important number too
        if model_number [i] == 0:
            model_number[i] = 9

            while 5 <= i <= 8 or i == 11:
                i -= 1
        else:
            break

    return model_number


def solve(input):
    program = []
    opmap = {
        'inp': 0, 
        'add': 1,
        'mod': 2,
        'div': 3,
        'mul': 4,
        'eql': 5
    }
    for line in input.split("\n"):
        (op, *args) = line.split(" ")
        program.append([opmap[op], args])

    # Well, brute force doesnt' work... Ain't nobody got time for that!
    # So... Abandoning the Python way completely.
    # Nothing to see here.    

    return None
    
    
def run(program, model_number):
    # reverse model_number as input list so we can pop() from end
    input = list(reversed(model_number))
    memory = {'w': 0, 'x': 0, 'y': 0, 'z': 0}
    opmap = {
        'inp': 0, 
        'add': 1,
        'mod': 2,
        'div': 3,
        'mul': 4,
        'eql': 5
    }

    def read(arg):
        if arg.isalpha():
            return memory[arg]

        return int(arg)

    def inp(a):
        memory[a] = input.pop()
        #logging.debug(f"{args[0]} = {memory[a]}")
    
    def add(a, b):
        #logging.debug(f"{args[0]} = add {memory[a]} {read(b)}", end="")
        memory[a] += read(b)
        #logging.debug(f" = {memory[a]}")
    
    def mod(a, b):
        #logging.debug(f"{args[0]} = mod {memory[a]} {read(b)}", end="")
        memory[a] = memory[a] % read(b)
        #logging.debug(f" = {memory[a]}")

    def div(a, b):
        #logging.debug(f"{args[0]} = div {memory[a]} {read(b)}", end="")
        memory[a] = int(memory[a] / read(b))
        #logging.debug(f"  = {memory[a]}")

    def mul(a, b):
        #logging.debug(f"{args[0]} = mul {memory[a]} {read(b)}", end="")
        memory[a] *= read(b)
        #logging.debug(f"  = {memory[a]}")
    
    def eql(a, b):
        #logging.debug(f"{args[0]} = eql {memory[a]} {read(b)}", end="")
        memory[a] = 1 if memory[a] == read(b) else 0
        #logging.debug(f" = {memory[a]}")

    dispatch = [inp, add, mod, div, mul, eql]
    memory['w'] = input.pop()
    for (op, args) in program[1:]:
        dispatch[op](*args)

    return memory['z']

if __name__ == '__main__':
    print("--- Day 24: Arithmetic Logic Unit ---")
    input = Path("input.txt").read_text()
    pt1 = solve(input)
    print(pt1)