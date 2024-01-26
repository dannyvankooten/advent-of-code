# Advent of Code 2015 in C

See https://adventofcode.com/2015/ for puzzle texts and inputs.

Execute [run.sh](run.sh) to run all solutions for all days (using bash + awk). 
Needs a C compiler toolchain like gcc or Clang.

Inputs are read from a `input.txt` file in the respective directory for each day's solution.

```bash
for d in */; do cd $d && cc -Ofast -march=native main.c -lcrypto && ./a.out && echo "" && cd ..; done
(for d in */; do cd $d && cc -Ofast -march=native main.c -lcrypto && ./a.out && cd ..; done) | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'
```

Results:

```txt 
--- Day 1: Not Quite Lisp ---
Part 1: 232
Part 2: 1783
Time: 0.17 ms

--- Day 2: I Was Told There Would Be No Math ---
Part 1: 1588178
Part 2: 3783758
Time: 0.07 ms

--- Day 3: Perfectly Spherical Houses in a Vacuum ---
Part 1: 2592
Part 2: 2360
Time: 0.16 ms

--- Day 4: The Ideal Stocking Stuffer ---
Part 1: 282749
Part 2: 9962624
Time: 1828.19 ms

--- Day 5: Doesn't He Have Intern-Elves For This? ---
Part 1: 258
Part 2: 53
Time: 0.47 ms

--- Day 6: Probably a Fire Hazard ---
Part 1: 400410
Part 2: 15343601
Time: 58.48 ms

--- Day 7: Some Assembly Required ---
Part 1: 46065 ✔
Part 2: 14134 ✔
Time: 0.23 ms

--- Day 8: Matchsticks ---
Part 1: 1342 ✔
Part 2: 2074 ✔
Time: 0.04 ms

--- Day 9: All in a Single Night ---
Part 1: 141
Part 2: 736
Time: 0.13 ms

--- Day 10: Elves Look, Elves Say ---
Part 1: 492982 ✔
Part 2: 6989950 ✔
Time: 48.52 ms

--- Day 11: Corporate Policy ---
Part 1: cqjxxyzz
Part 2: cqkaabcc
Time: 14.11 ms

--- Day 12: JSAbacusFramework.io ---
Part 1: 119433
Part 2: 68466
Time: 0.34 ms

--- Day 13: Knights of the Dinner Table ---
Part 1: 618
Part 2: 601
Time: 2.06 ms

--- Day 14: Reindeer Olympics ---
Part 1: 2660 ✔
Part 2: 1256 ✔
Time: 0.12 ms

--- Day 15: Science for Hungry People ---
Part 1: 18965440 ✔
Part 2: 15862900 ✔
Time: 0.17 ms

--- Day 16: Aunt Sue ---
Part 1: 213 ✔
Part 2: 323 ✔
Time: 0.15 ms

--- Day 17: No Such Thing as Too Much ---
Part 1: 1638
Part 2: 17
Time: 5.08 ms

--- Day 18: Like a GIF For Your Yard ---
Part 1: 814
Part 2: 924
Time: 41.03 ms

--- Day 19: Medicine for Rudolph ---
Part 1: 509
Part 2: 195
Time: 1.50 ms

--- Day 20: Infinite Elves and Infinite Houses ---
Part 1: 786240
Part 2: 831600
Time: 318.88 ms

--- Day 21: RPG Simulator 20XX ---
Part 1: 111
Part 2: 188
Time: 0.13 ms

--- Day 22: Wizard Simulator 20XX ---
Part 1: 0
Part 2: 0
Time: 0.07 ms

--- Day 23: Opening the Turing Lock ---
Part 1: 307
Part 2: 160
Time: 0.07 ms

--- Day 24: It Hangs in the Balance ---
Part 1: 10723906903
Part 2: 74850409
Time: 36.95 ms

--- Day 25: Let It Snow ---
Part 1: 9132360
Time: 98.70 ms

Total time: 2455.820 ms
```

## License

Solution implementations are released into the public domain under the [UNLICENSE](/UNLICENSE) license.

Puzzle text and inputs are all the sole property of [Advent of Code](https://adventofcode.com/) and may not be replicated or used by any other person or entity without express written consent of Advent of Code.
