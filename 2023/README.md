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
Time: 1.70ms

--- Day 2: Cube Conundrum ---
Part 1: 2156
Part 2: 66909
Time: 0.33ms

--- Day 3: Gear Ratios ---
Part 1: 537832
Part 2: 81939900
Time: 1.04ms

--- Day 4: Scratchcards ---
Part 1: 25174
Part 2: 6420979
Time: 0.89ms

--- Day 5: If You Give A Seed A Fertilizer ---
Part 1: 313045984
Part 2: 20283860
Time: 0.42ms

---- Day 6: Wait For It ---
Part 1: 293046
Part 2: 35150181
Time: 0.08ms

--- Day 7: Camel Cards ---
Part 1: 251545216
Part 2: 250384185
Time: 2.99ms

--- Day 8: Haunted Wasteland ---
Part 1: 19099
Part 2: 17099847107071
Time: 10.03ms

--- Day 9: Mirage Maintenance ---
Part 1: 1479011877
Part 2: 973
Time: 0.77ms

--- Day 10: Pipe Maze ---
Part 1: 6838
Part 2: 451
Time: 4.61ms

--- Day 11: Cosmic Expansion ---
Part 1: 9686930
Part 2: 630728425490
Time: 1.18ms

--- Day 12: Hot Springs ---
Part 1: 7286
Part 2: 25470469710341
Time: 142.52ms

--- Day 13: Point of Incidence ---
Part 1: 31265
Part 2: 39359
Time: 0.90ms

--- Day 14: Parabolic Reflector Dish ---
Part 1: 106378
Part 2: 90795
Time: 162.82ms

--- Day 15: Lens Library ---
Part 1: 507666
Part 2: 233537
Time: 1.46ms

--- Day 16: The Floor Will Be Lava ---
Part 1: 8034
Part 2: 8225
Time: 27.70ms

--- Day 17: Clumsy Crucible ---
Part 1: 1155
Part 2: 1283
Time: 104.43ms

--- Day 18: Lavaduct Lagoon ---
Part 1: 36807
Part 2: 48797603984357
Time: 0.95ms

--- Day 19: Aplenty ---
Part 1: 323625
Part 2: 127447746739409
Time: 1.90ms

--- Day 20: Pulse Propagation ---
Part 1: 834323022
Part 2: 225386464601017
Time: 141.29ms

--- Day 21: Step Counter ---
Part 1: 3658
Part 2: 608193767979991
Time: 107.69ms

--- Day 22: Sand Slabs ---
Part 1: 463
Part 2: 89727
Time: 466.25ms

--- Day 23: A Long Walk ---
Part 1: 2502
Part 2: 6726
Time: 7869.04ms

--- Day 24: Never Tell Me The Odds ---
Part 1: 19523
Part 2: 0
Time: 3.45ms

--- Day 25: Snowverload ---
Part 1: 559143
Part 2: 559143
Time: 74.01ms

Total time: 9128.45ms
```

## License

Solution implementations are released into the public domain under the [UNLICENSE](/UNLICENSE) license.

Puzzle text and inputs are all the sole property of [Advent of Code](https://adventofcode.com/) and may not be replicated or used by any other person or entity without express written consent of Advent of Code.
