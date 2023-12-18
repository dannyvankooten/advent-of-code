# Advent of Code 2023 in Golang.

See https://adventofcode.com/2023 for puzzle text and inputs.

Solutions for this year are mostly written in [Go](https://go.dev/), with the occassional port to C when I was curious about potential performance improvements.

Inputs are read from a `input.txt` file in the respective directory for each day's solution.

To run all solutions for all days using bash (requires a Go compiler):

```bash
for d in */; do cd $d && go run main.go && echo "" && cd ..; done
(for d in */; do cd $d && go run main.go && cd ..; done) | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'
```

Results:

```txt
--- Day 1: Trebuchet?! ---
Part 1: 53080
Part 2: 53268
Time: 1.21ms

--- Day 2: Cube Conundrum ---
Part 1: 2156
Part 2: 66909
Time: 0.14ms

--- Day 3: Gear Ratios ---
Part 1: 537832
Part 2: 81939900
Time: 0.27ms

--- Day 4: Scratchcards ---
Part 1: 25174
Part 2: 6420979
Time: 0.69ms

--- Day 5: If You Give A Seed A Fertilizer ---
Part 1: 313045984
Part 2: 20283860
Time: 2549.96ms

---- Day 6: Wait For It ---
Part 1: 293046
Part 2: 35150181
Time: 0.04ms

--- Day 7: Camel Cards ---
Part 1: 251545216
Part 2: 250384185
Time: 2.05ms

--- Day 8: Haunted Wasteland ---
Part 1: 19099
Part 2: 17099847107071
Time: 3.06ms

--- Day 9: Mirage Maintenance ---
Part 1: 1479011877
Part 2: 973
Time: 0.46ms

--- Day 10: Pipe Maze ---
Part 1: 6838
Part 2: 451
Time: 2.65ms

--- Day 11: Cosmic Expansion ---
Part 1: 9686930
Part 2: 630728425490
Time: 0.39ms

--- Day 12: Hot Springs ---
Part 1: 21
Part 2: 525152
Time: 1537.28ms

--- Day 13: Point of Incidence ---
Part 1: 31265
Part 2: 39359
Time: 0.81ms

--- Day 14: Parabolic Reflector Dish ---
Part 1: 106378
Part 2: 90795
Time: 97.69ms

--- Day 15: Lens Library ---
Part 1: 507666
Part 2: 233537
Time: 0.38ms

--- Day 17: Clumsy Crucible ---
Part 1: 1155
Part 2: 1283
Time: 41.54ms

Total time: 4197.08ms
```

## License

Solution implementations are released into the public domain under the [UNLICENSE](/UNLICENSE) license.

Puzzle text and inputs are all the sole property of [Advent of Code](https://adventofcode.com/) and may not be replicated or used by any other person or entity without express written consent of Advent of Code.