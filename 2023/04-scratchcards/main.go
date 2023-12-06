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
	bytes, err := os.ReadFile("input.txt")
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

	input := strings.TrimSpace(string(bytes))
	for i, c := range strings.Split(input, "\n") {
		if c == "" { continue }
		c = c[strings.Index(c, ": "):]
		sep := strings.Index(c, " | ")
		winning := make([]int, 0, 10)
		for _, n := range strings.Split(c[:sep], " ") {

			d, _ := strconv.Atoi(n)
			winning = append(winning, d)
		}
		hand := make([]int, 0, 25)
		for _, n := range strings.Split(c[sep+3:], " ") {
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
					pt2 += copies[i]
					copies[i+nmatches] += copies[i]

					if nmatches > 2 {
						reward *= 2
					}
					pt1 += reward
					break
				}
			}
		}
	}

    fmt.Print("--- Day 4: Scratchcards ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds()) / 1000)
}
