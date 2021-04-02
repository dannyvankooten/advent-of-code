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
Day 1:     0 ms
Day 2:     0 ms
Day 3:     0 ms
Day 4:     0 ms
Day 5:     0 ms
Day 6:     0 ms
Day 7:     2 ms
Day 8:     0 ms
Day 9:     0 ms
Day 10:    0 ms
Day 11:   11 ms
Day 12:    0 ms
Day 13:    0 ms
Day 14:    1 ms
Day 15:  550 ms
Day 16:    0 ms
Day 17:    1 ms
Day 18:    0 ms
Day 19:    0 ms
Day 20:   29 ms
Day 21:    0 ms
Day 22:   10 ms
Day 23:  300 ms
Day 24:    7 ms
Day 25:   47 ms
Total:   958 ms
```


### Resources

- Hexagonal grids, https://www.redblobgames.com/grids/hexagons/
- Chinese Remainder Theorem, https://en.wikipedia.org/wiki/Chinese_remainder_theorem
- Rotation in a Cartesian coordinate system, https://en.wikipedia.org/wiki/Cartesian_coordinate_system#Rotation
- Linear probing, https://en.wikipedia.org/wiki/Linear_probing
- Left recursion, https://en.wikipedia.org/wiki/Left_recursion
- Dynamic programming, https://en.wikipedia.org/wiki/Dynamic_programming

### Tools

- Valgrind, https://valgrind.org/docs/manual/cg-manual.html
- Gprof, https://sourceware.org/binutils/docs/gprof/index.html
- GDB, https://www.gnu.org/software/gdb/
- Cachegrind, https://valgrind.org/docs/manual/cg-manual.html
