# Advent of Code 2024, in PHP.

See https://adventofcode.com/2024 for puzzle text and inputs.

Inputs are read from a `{day}.txt` file in the same directory as the day's solution scripts, where day is `01..25`.

```txt
--- Day 1: Historian Hysteria ---
Part 1: 1506483
Part 2: 23126924
Took 1.5769004821777 ms

--- Day 2: Red-Nosed Reports ---
Part 1: 371
Part 2: 426
Took 5.7039260864258 ms

--- Day 3: Mull It Over ---
Part 1: 174103751
Part 2: 100411201
Took 0.81992149353027 ms

--- Day 4: Ceres Search ---
Part 1: 2414
Part 2: 1871
Took 12.607097625732 ms

--- Day 5: Print Queue ---
Part 1: 5064
Part 2: 5152
Took 3.8139820098877 ms

--- Day 6: Guard Gallivant ---
Part 1: 5409
Part 2: 2022
Took 2585.5920314789 ms

--- Day 7: Bridge Repair ---
Part 1: 303766880536
Part 2: 337041851384440
Took 1099.5209217072 ms

--- Day 8: Resonant Collinearity ---
Part 1: 252
Part 2: 839
Took 1.3210773468018 ms

--- Day 9: Disk Fragmenter ---
Part 1: 6398608069280
Part 2: 6427437134372
Took 3991.8909072876 ms

--- Day 10 ---
Part 1: 746
Part 2: 1541
Took 12.495994567871 ms

--- Day 11 ---
Part 1: 209412
Part 2: 248967696501656
Took 66.925048828125 ms

--- Day 12 ---
Part 1: 140
Part 2: 98
Took 0.34403800964355 ms

--- Day 13 ---
Part 1: 34787
Part 2: 85644161121698
Took 1.1529922485352 ms

--- Day 14 ---
Part 1: 225648864
Part 2: 7847

                           ▓

                          ▓             ▓                                            ▓

                                                                            ▓
                                                                                           ▓
               ▓                               ▓
                            ▓
                                                       ▓                ▓  ▓
                ▓            ▓                ▓                    ▓                   ▓       ▓
           ▓                                          ▓                           ▓ ▓
        ▓                                ▓
                                                                         ▓           ▓         ▓
    ▓



▓         ▓
        ▓                                       ▓                          ▓                       ▓
                                                   ▓
                                                         ▓                      ▓
 ▓

               ▓                                                              ▓
         ▓                                                 ▓                  ▓
                                                 ▓
    ▓                           ▓
            ▓                                         ▓                                         ▓
                                                                          ▓
                                     ▓                                                            ▓
                                                                                     ▓
                                     ▓                          ▓              ▓        ▓   ▓
                                                                                   ▓
 ▓          ▓                              ▓

                                                                  ▓
                                                  ▓
                                                                             ▓
   ▓
        ▓


                                       ▓                                              ▓
                                                                                  ▓
                                                     ▓            ▓
             ▓                                                         ▓                         ▓
                                         ▓              ▓

              ▓                                 ▓
                                                                                                  ▓


      ▓                                                ▓
                                                                                  ▓
                                         ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
                                         ▓                             ▓                      ▓  ▓
                                         ▓                             ▓
                            ▓            ▓                             ▓
                                         ▓                             ▓
                                         ▓              ▓              ▓
                                         ▓             ▓▓▓             ▓
                                  ▓      ▓            ▓▓▓▓▓            ▓
       ▓                     ▓     ▓▓    ▓           ▓▓▓▓▓▓▓           ▓
                                         ▓          ▓▓▓▓▓▓▓▓▓          ▓
                        ▓                ▓            ▓▓▓▓▓            ▓
                 ▓     ▓                 ▓           ▓▓▓▓▓▓▓           ▓
                                         ▓          ▓▓▓▓▓▓▓▓▓          ▓      ▓
                                         ▓         ▓▓▓▓▓▓▓▓▓▓▓         ▓                   ▓▓▓
                    ▓                    ▓        ▓▓▓▓▓▓▓▓▓▓▓▓▓        ▓
                    ▓     ▓              ▓          ▓▓▓▓▓▓▓▓▓          ▓
                                         ▓         ▓▓▓▓▓▓▓▓▓▓▓         ▓
                                         ▓        ▓▓▓▓▓▓▓▓▓▓▓▓▓        ▓    ▓
                                   ▓     ▓       ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       ▓ ▓        ▓     ▓
                                         ▓      ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓      ▓
                                         ▓        ▓▓▓▓▓▓▓▓▓▓▓▓▓        ▓                         ▓
                   ▓               ▓     ▓       ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       ▓
                                         ▓      ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓      ▓  ▓
                                    ▓    ▓     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓     ▓
                   ▓                  ▓  ▓    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓    ▓
                      ▓                  ▓             ▓▓▓             ▓            ▓
                                         ▓             ▓▓▓             ▓
                                     ▓   ▓             ▓▓▓             ▓                  ▓
                                      ▓  ▓                             ▓           ▓
                                         ▓                             ▓
                                         ▓                             ▓     ▓            ▓
                                         ▓                             ▓
           ▓                             ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       ▓

▓                                                             ▓

                              ▓                                 ▓

                                                            ▓
                         ▓
                                    ▓                  ▓           ▓
                                                 ▓                 ▓                      ▓
  ▓                                                                        ▓      ▓
                                                                     ▓   ▓
                   ▓
         ▓                                                                        ▓                 ▓

                                         ▓                   ▓
                              ▓
Took 370.93091011047 ms

--- Day 15 ---
Part 1: 1383666
Part 2: 0
Took 11.682987213135 ms

--- Day 16 ---
Part 1: 89460
Part 2: 504
Took 103.78313064575 ms

--- Day 17 ---
Part 1: 1,0,2,0,5,7,2,1,3
Part 2: 265652340990875
Took 9.641170501709 ms

--- Day 18 ---
Part 1: 360
Part 2: 58,62
Took 4220.006942749 ms

Total time: 7802.27ms
```

## License

Solution implementations are released into the public domain under the [UNLICENSE](/UNLICENSE) license.

Puzzle text and inputs are all the sole property of [Advent of Code](https://adventofcode.com/) and may not be replicated or used by any other person or entity without express written consent of Advent of Code.
