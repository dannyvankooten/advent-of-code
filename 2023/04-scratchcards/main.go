package main

import (
	"fmt"
	"os"
	"strconv"
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
	pt2 := ncards

	// start with single copy of each card
	copies := make([]int, ncards)
	for i := 0; i < ncards; i++ {
		copies[i] = 1
	}

	for i, c := range strings.Split(string(input), "\n") {
		if c == "" { continue }
		c = c[strings.Index(c, ":")+1:]
		c = strings.TrimSpace(c)
		parts := strings.Split(c, " | ")
		if len(parts) != 2 {
			fmt.Printf("invalid line: %s\n", c)
			continue
		}

		winning := make([]int, 0, 10)
		for _, n := range strings.Split(parts[0], " ") {
			n = strings.TrimSpace(n)
			if n == "" { continue }
			d, _ := strconv.Atoi(n)
			winning = append(winning, d)
		}
		hand := make([]int, 0, 25)
		for _, n := range strings.Split(parts[1], " ") {
			n = strings.TrimSpace(n)
			if n == "" { continue }
			d, _ := strconv.Atoi(n)
			hand = append(hand, d)
		}

		reward := 1
		nmatches := 0
		for _, n := range hand {
			for _, v := range winning {
				if n == v {
					nmatches++

					if nmatches > 2 {
						reward *= 2
					}

					pt1 += reward
					break
				}
			}
		}

		// create copies
		for c := 0; c < copies[i]; c++ {
			for m := 0; m < nmatches; m++ {
				copies[i+m+1]++
				pt2++
			}
		}
	}

	fmt.Printf("part 1: %d\n", pt1)
	fmt.Printf("part 2: %d\n", pt2)
	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds()) / 1000)
}
