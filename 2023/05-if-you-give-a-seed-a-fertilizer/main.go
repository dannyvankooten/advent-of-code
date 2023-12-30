package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

func parseNumbers(s string) []int {
	var err error

	svalues := strings.Split(s, " ")
	numbers := make([]int, len(svalues))
	for i, s := range svalues {
		numbers[i], err = strconv.Atoi(s)
		if err != nil {
			panic(err)
		}
	}
	return numbers
}

func transform(source int, rules [][]int) int {
	for _, rule := range rules {
		if source >= rule[1] && source < rule[1]+rule[2] {
			return rule[0] + source - rule[1]
		}
	}

	return source
}

func parseInput(filename string) ([]int, [][][]int) {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(bytes), "\n")
	seeds := parseNumbers(lines[0][strings.Index(lines[0], ": ")+2:])
	s := 0

	translators := make([][][]int, 0, 7)
	translators = append(translators, make([][]int, 0, 7))
	for i := 3; i < len(lines); i++ {
		// on every empty line, start a new map
		if lines[i] == "" {
			i += 1
			s += 1

			if s == 7 {
				break
			}
			translators = append(translators, make([][]int, 0, 7))
			continue
		}

		translators[s] = append(translators[s], parseNumbers(lines[i]))
	}

	return seeds, translators
}

func pt1(seeds []int, translators [][][]int) int {
	// pt 1
	pt1 := 1 << 31
	for _, s := range seeds {
		for _, t := range translators {
			s = transform(s, t)
		}
		if s < pt1 {
			pt1 = s
		}
	}

	return pt1
}

var names = []string{
	"seed-to-soil",
	"soil-to-fertilizer",
	"fertilizer-to-water",
	"water-to-light",
	"light-to-temperature",
	"temperature-to-humidity",
	"humidity-to-location",
}

type Range [2]int

func transformRange(ranges []Range, rules [][]int) []Range {
	// for every range, check if it's inside mapping
	out := make([]Range, 0, len(ranges))

	for i := 0; i < len(ranges); i++ {
		r := ranges[i]
		mapped := false

		for _, rule := range rules {
			ruleStart := rule[1]
			ruleEnd := rule[1] + rule[2] - 1
			ruleOffset := rule[0] - rule[1]

			// if entirely before or after rule, skip rule entirely
			if r[1] < ruleStart || r[0] > ruleEnd {
				continue
			}

			// range is at least partially inside this rule's mapping
			// create a before, inside and after split

			mapped = true

			if r[0] < ruleStart {
				before := Range{r[0], ruleStart - 1}
				ranges = append(ranges, before)
			}
			inside := Range{
				max(r[0], ruleStart) + ruleOffset,
				min(r[1], ruleEnd) + ruleOffset,
			}
			out = append(out, inside)

			if r[1] > ruleEnd {
				after := Range{ruleEnd + 1, r[1]}
				ranges = append(ranges, after)
			}

			// range was mapped, do not process further rules... right?
			break
		}

		// if not mapped to any rule
		// copy range as is
		if !mapped {
			out = append(out, r)
		}

	}

	return out
}

func pt2Ranges(seeds []int, translators [][][]int) int {
	lowest := 1 << 62

	for i := 0; i < len(seeds); i += 2 {
		r := []Range{{seeds[i], seeds[i] + seeds[i+1] - 1}}
		for _, t := range translators {
			r = transformRange(r, t)
		}

		for _, r := range r {
			if r[0] < lowest {
				lowest = r[0]
			}
		}
	}

	return lowest
}

func main() {
	timeStart := time.Now()
	seeds, translators := parseInput("input.txt")

	fmt.Printf("--- Day 5: If You Give A Seed A Fertilizer ---\n")
	fmt.Printf("Part 1: %d\n", pt1(seeds, translators))
	fmt.Printf("Part 2: %d\n", pt2Ranges(seeds, translators))
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
