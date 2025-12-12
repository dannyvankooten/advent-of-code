import sys
import time
import math
import re
from collections import deque
from z3 import *

time_start = time.time_ns()
pt1 = 0
pt2 = 0
lines = [l.strip() for l in sys.stdin.readlines()]



def solve_pt1(target, buttons):
    num_buttons = len(buttons)
    num_lights = len(target)

    # x[j] in {0,1}: whether to press button j
    x = [Int(f"x_{j}") for j in range(num_buttons)]

    opt = Optimize()

    for v in x:
        opt.add(Or(v == 0, v == 1))

    # One equation per light: sum toggles mod 2 = target
    for light in range(num_lights):
        opt.add(
            sum(x[j] for j, b in enumerate(buttons) if light in b) % 2
            == (1 if target[light] else 0)
        )

    # Minimize number of presses
    opt.minimize(sum(x))

    assert opt.check() == sat
    m = opt.model()

    presses = [m[v].as_long() for v in x]
    return sum(presses)


def solve_pt2(buttons, target):
    # buttons: list of lists, each inner list = counters this button increments
    # target: list of target counts for each counter

    num_buttons = len(buttons)
    num_counters = len(target)

    # One integer variable per button
    presses = [Int(f"x_{i}") for i in range(num_buttons)]

    opt = Optimize()

    for p in presses:
        opt.add(p >= 0)

    # Build linear equations: sum(button[j] affects counter i) x_j == target[i]
    for counter_idx in range(num_counters):
        opt.add(
            sum(
                presses[button_idx]
                for button_idx, b in enumerate(buttons)
                if counter_idx in b
            ) == target[counter_idx]
        )

    # Minimize total presses
    opt.minimize(sum(presses))

    assert opt.check() == sat
    model = opt.model()
    return sum(model[p].as_long() for p in presses)



for l in lines:
	# [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
	groups = l.split(" ")
	target_state = list([True if x == "#" else False for x in groups[0][1:-1]])
	buttons = [list(map(int, x[1:-1].split(","))) for x in groups[1:-1]]
	target_joltages = list(map(int, groups[-1][1:-1].split(",")))
	pt1 += solve_pt1(target_state, buttons)
	pt2 += solve_pt2(buttons, target_joltages)




print("--- Day 10 --")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took", (time.time_ns() - time_start) // 1_000_000, "ms")

assert(pt1 == 509)
assert(pt2 == 20083)
