My solutions for [Advent of Code 2020](https://adventofcode.com/2020), in C.

This was my first year of finishing all challenges. Definitely got a lot more comfortable with C, shot myself in the foot a good few times, and learned a thing or two.

Inspired by [Tim Visee](https://timvisee.com/blog/solving-aoc-2020-in-under-a-second/)'s example, I spent some time optimizing the challenges to get them to run in under 1 second. This wasn't too hard given my language choice, but I certainly couldn't have done it without the [Chinese Remainder Theorem](https://en.wikipedia.org/wiki/Chinese_remainder_theorem) for day 13.

### Running

To run a solution, build the executable and then run it providing a day argument or no argument to run all the days consecutively.

```
make release        
aoc2020 15          # run a single day
aoc2020             # run all days
aoc2020 --bench     # run all days, best of 10
```

### Benchmarks

On my Lenovo Yoga Slim 7 laptop (AMD Ryzen 7 4700U). 

```
Day 1:       209 μs
Day 2:        22 μs
Day 3:        14 μs
Day 4:        73 μs
Day 5:        60 μs
Day 6:       110 μs
Day 7:      2487 μs
Day 8:       224 μs
Day 9:        54 μs
Day 10:        6 μs
Day 11:    18863 μs
Day 12:       19 μs
Day 13:        3 μs
Day 14:      891 μs
Day 15:   543893 μs
Day 16:      398 μs
Day 17:     2112 μs
Day 18:      168 μs
Day 19:      877 μs
Day 20:    28548 μs
Day 21:      417 μs
Day 22:     7325 μs
Day 23:   290933 μs
Day 24:     6605 μs
Day 25:    45657 μs
Total:    949968 μs (0.95 s)
```


### Resources

- Hexagonal grids, https://www.redblobgames.com/grids/hexagons/
- Chinese Remainder Theorem, https://en.wikipedia.org/wiki/Chinese_remainder_theorem
- Rotation in a Cartesian coordinate system, https://en.wikipedia.org/wiki/Cartesian_coordinate_system#Rotation
- Linear probing, https://en.wikipedia.org/wiki/Linear_probing
- Left recursion, https://en.wikipedia.org/wiki/Left_recursion
- Dynamic programming, https://en.wikipedia.org/wiki/Dynamic_programming
- How to C (as of 2016), https://matt.sh/howto-c

### Tools

- Valgrind, https://valgrind.org/docs/manual/cg-manual.html
- Gprof, https://sourceware.org/binutils/docs/gprof/index.html
- GDB, https://www.gnu.org/software/gdb/
- Cachegrind, https://valgrind.org/docs/manual/cg-manual.html
- Clang-format, https://clang.llvm.org/docs/ClangFormat.html