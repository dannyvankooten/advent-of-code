package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

func parseNumbers(s string) []int {
	numbers := make([]int, 0)
	for _, s := range strings.Split(s, " ") {
		n, _ := strconv.Atoi(s)
		numbers = append(numbers, n)
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

func main() {
	timeStart := time.Now()
	bytes, err := os.ReadFile("input.txt")
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(bytes), "\n")
	seeds := parseNumbers(lines[0][strings.Index(lines[0], ": ")+2:])
	s := 0
	translators := make([][][]int, 0, 0)
	translators = append(translators, make([][]int, 0, 0))
	for i := 3; i < len(lines); i++ {
		// on every empty line, start a new map
		if lines[i] == "" {
			i += 1
			s += 1
			translators = append(translators, make([][]int, 0, 0))
			continue
		}

		translators[s] = append(translators[s], parseNumbers(lines[i]))
	}

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
	fmt.Printf("part 1: %d\n", pt1)

	// pt 2
	pt2 := 1 << 31
	fmt.Printf("%d seed pairs\n", len(seeds))
	for i := 0; i < len(seeds); i += 2 {
		for j := 0; j < seeds[i+1]; j++ {
			s := seeds[i] + j
			for _, t := range translators {
				s = transform(s, t)
			}
			if s < pt2 {
				pt2 = s
			}
		}

	}
	fmt.Printf("part 2: %d\n", pt2)

	// time
	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
