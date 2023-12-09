package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
	"time"
)

func parse(filename string) [][]int {
	input, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}

	sequences := make([][]int, 0)
	for _, l := range strings.Split(string(input), "\n") {
		if l == "" {
			continue
		}

		sequence := make([]int, 0)
		for _, str := range strings.Split(l, " ") {
			n, _ := strconv.Atoi(str)
			sequence = append(sequence, n)
		}

		sequences = append(sequences, sequence)
	}

	return sequences
}

func sumInts(ns []int) int {
	s := 0
	for _, n := range ns {
		s += n
	}

	return s
}

func pt1(seqs [][]int) int {
	pt1 := 0

	for _, s := range seqs {
		sub := make([][]int, 0)
		sub = append(sub, s)

		for {
			diffs := make([]int, len(s)-1)
			sumDiffs := 0
			for i := 0; i < len(s)-1; i++ {
				diffs[i] = s[i+1] - s[i]
				sumDiffs += diffs[i]
			}

			if sumDiffs == 0 {
				break
			}

			sub = append(sub, diffs)
			s = diffs
		}

		// walk subs
		for _, d := range sub {
			pt1 += d[len(d)-1]
		}

	}

	return pt1
}

func pt2(seqs [][]int) int {
	for i := 0; i < len(seqs); i++ {
		slices.Reverse(seqs[i])
	}
	return pt1(seqs)
}

func main() {
	timeStart := time.Now()
	seqs := parse("input.txt")

	a1 := pt1(seqs)
	a2 := pt2(seqs)
	fmt.Print("--- Day 9: Mirage Maintenance ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
