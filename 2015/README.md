# Advent of Code 2015

For this year I really enjoyed writing the solutions in C (again).

To compile and run all solutions for all days using bash:

```bash
for d in */; do cd $d && cc -Ofast -march=native main.c -lcrypto && ./a.out && echo "" && cd ..; done
(for d in */; do cd $d && cc -Ofast -march=native main.c -lcrypto && ./a.out && cd ..; done) | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'
```

```txt 
--- Day 1: Not Quite Lisp ---
Part 1: 232
Part 2: 1783
Time: 0.16ms

--- Day 2: I Was Told There Would Be No Math ---
Part 1: 1588178
Part 2: 3783758
Time: 0.21ms

--- Day 3: Perfectly Spherical Houses in a Vacuum ---
Part 1: 2592
Part 2: 2360
Time: 0.18ms

--- Day 4: The Ideal Stocking Stuffer ---
Part 1: 282749
Part 2: 9962624
Time: 1891.65ms

--- Day 5: Doesn't He Have Intern-Elves For This? ---
Part 1: 258
Part 2: 53
Time: 0.39ms

--- Day 6: Probably a Fire Hazard ---
Part 1: 400410
Part 2: 15343601
Time: 55.82ms

--- Day 7: Some Assembly Required ---
Part 1: 46065
Part 2: 14134
Time: 0.68ms

--- Day 8: Matchsticks ---
Part 1: 1342
Part 2: 2074
Time: 0.05ms

--- Day 9: All in a Single Night ---
Part 1: 264
Time: 0.06ms

--- Day 10: Elves Look, Elves Say ---
Part 1: 492982
Part 2: 6989950
Time: 46.09ms

--- Day 11: Corporate Policy ---
Part 1: cqjxxyzz
Part 2: cqkaabcc
Time: 38.94ms

--- Day 12: JSAbacusFramework.io ---
Part 1: 119433
Part 2: 67653
Time: 0.11ms

--- Day 14: Reindeer Olympics ---
Part 1: 2660
Part 2: 1256
Time: 0.15ms

--- Day 15: Science for Hungry People ---
Part 1: 18965440
Part 2: 15862900
Time: 45.52ms

--- Day 16: Aunt Sue ---
Part 1: 213
Part 2: 323
Time: 0.17ms

--- Day 17: No Such Thing as Too Much ---
Part 1: 1638
Part 2: 17
Time: 2.49ms

Total time: 1887.79ms
```

## License

Solution implementations are released into the public domain under the [UNLICENSE](/UNLICENSE) license.

Puzzle text and inputs are all the sole property of [Advent of Code](https://adventofcode.com/) and may not be replicated or used by any other person or entity without express written consent of Advent of Code.
