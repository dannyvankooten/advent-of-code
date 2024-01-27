My solutions for [Advent of Code 2020](https://adventofcode.com/2020), in C.

This was my first year of finishing all challenges. Definitely got a lot more comfortable with C, shot myself in the foot a good few times, and learned a thing or two.

Inspired by [Tim Visee](https://timvisee.com/blog/solving-aoc-2020-in-under-a-second/)'s example, I spent some time optimizing the challenges to get them to run in under 1 second. This wasn't too hard given my language choice, but I certainly couldn't have done it without the [Chinese Remainder Theorem](https://en.wikipedia.org/wiki/Chinese_remainder_theorem) for day 13.

Blog post: https://dannyvankooten.com/solving-advent-of-code-2020-under-1-second/

### Running

To run a solution, build the executable and then run it providing a day argument or no argument to run all the days consecutively.

```
make release
aoc2020 10          # run a single day
aoc2020             # run all days
aoc2020 --bench     # run all days, best of 10
```

The solution for day 15 is optimized for Linux and needs about ~60 2MB huge pages available on your machine.

```
echo 60 > /proc/sys/vm/nr_hugepages
```

### Benchmarks

On my Lenovo Yoga Slim 7 laptop (AMD Ryzen 7 4700U).

```
Day 1:	    0.05 ms
Day 2:	    0.03 ms
Day 3:	    0.03 ms
Day 4:	    0.06 ms
Day 5:	    0.05 ms
Day 6:	    0.05 ms
Day 7:	    4.45 ms
Day 8:	    0.18 ms
Day 9:	    0.06 ms
Day 10:	    0.01 ms
Day 11:	    2.58 ms
Day 12:	    0.04 ms
Day 13:	    0.01 ms
Day 14:	    1.65 ms
Day 15:	  319.22 ms
Day 16:	    0.18 ms
Day 17:	    1.99 ms
Day 18:	    0.20 ms
Day 19:	    0.50 ms
Day 20:	    1.09 ms
Day 21:	    0.27 ms
Day 22:	    0.10 ms
Day 23:	  160.27 ms
Day 24:	    3.11 ms
Day 25:	    0.04 ms
Total:	  496.23 ms
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

### Previous editions

- [2019: Rust](https://github.com/dannyvankooten/advent-of-code/tree/main/2019-rust)
