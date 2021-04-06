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
Day 1:	      21 μs
Day 2:	      23 μs
Day 3:	      29 μs
Day 4:	     175 μs
Day 5:	      69 μs
Day 6:	      66 μs
Day 7:	    2259 μs
Day 8:	     201 μs
Day 9:	      30 μs
Day 10:	       3 μs
Day 11:	    1685 μs
Day 12:	      24 μs
Day 13:	       4 μs
Day 14:	    1087 μs
Day 15:	  505663 μs
Day 16:	     287 μs
Day 17:	    1513 μs
Day 18:	     123 μs
Day 19:	     720 μs
Day 20:	     878 μs
Day 21:	     268 μs
Day 22:	     117 μs
Day 23:	  220609 μs
Day 24:	    4806 μs
Day 25:	      73 μs
Total:	  740733 μs (0.74 s)
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