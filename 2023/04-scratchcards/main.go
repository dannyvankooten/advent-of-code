package main

import (
	"fmt"
	"os"
	"strings"
	"time"
)


func main() {
	timeStart := time.Now()
	input, err := os.ReadFile("input.txt")
	if err != nil {
		panic(err)
	}

	ncards := 203
	pt1 := 0
	copies := make([]int, ncards)
	for i := 0; i < ncards; i++ {
		copies[i] = 1
	}

	for i, c := range strings.Split(string(input), "\n") {
		if c == "" { continue }
		c = c[strings.Index(c, ":")+1:]
		c = strings.TrimSpace(c)
		c = strings.ReplaceAll(c, "  ", " ")

		parts := strings.Split(c, " | ")
		if len(parts) != 2 {
			fmt.Printf("invalid line: %s\n", c)
			continue
		}


		winning := " " + parts[0] + " "
		hand := strings.Split(parts[1], " ")

		for c := 0; c < copies[i]; c++ {
			reward := 1
			nmatches := 0

			for _, n := range hand {
				if strings.Index(winning, " "+n+" ") > -1 {
					if nmatches > 1 {
						reward *= 2
					}

					nmatches++

					if c == 0 {
						pt1 += reward
					}
				}
			}

			for m := 0; m < nmatches; m++ {
				copies[i+m+1]++
			}
		}
	}

	fmt.Printf("part 1: %d\n", pt1)

	pt2 := 0
	for i := 0; i < ncards; i++ {
		pt2 += copies[i]
	}
	fmt.Printf("part 2: %d\n", pt2)



	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds()) / 1000)
}
