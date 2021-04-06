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
Day 1:	      34 μs
Day 2:	      28 μs
Day 3:	      27 μs
Day 4:	     243 μs
Day 5:	     218 μs
Day 6:	     215 μs
Day 7:	    2305 μs
Day 8:	     205 μs
Day 9:	      32 μs
Day 10:	       5 μs
Day 11:	   15973 μs
Day 12:	      27 μs
Day 13:	       4 μs
Day 14:	    1045 μs
Day 15:	  508025 μs
Day 16:	     294 μs
Day 17:	    1525 μs
Day 18:	     117 μs
Day 19:	     733 μs
Day 20:	     903 μs
Day 21:	     264 μs
Day 22:	     117 μs
Day 23:	  236326 μs
Day 24:	    4851 μs
Day 25:	      76 μs
Total:	  773592 μs (0.77 s)
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