package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

func parseNumbers(s string) []int {
	svalues := strings.Split(s, " ")
	numbers := make([]int, len(svalues))
	for i, s := range svalues {
		numbers[i], _ = strconv.Atoi(s)
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

func reverse(dest int, rules [][]int) int {
	var source int
	for _, rule := range rules {
		source = dest + rule[1] - rule[0]
		if source >= rule[1] && source < rule[1]+rule[2] {
			return source
		}
	}

	return dest
}

func parseInput() ([]int, [][][]int) {
	bytes, err := os.ReadFile("input.txt")
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(bytes), "\n")
	seeds := parseNumbers(lines[0][strings.Index(lines[0], ": ")+2:])
	s := 0

	translators := make([][][]int, 0, 8)
	translators = append(translators, make([][]int, 0, 8))
	for i := 3; i < len(lines); i++ {
		// on every empty line, start a new map
		if lines[i] == "" {
			i += 1
			s += 1
			translators = append(translators, make([][]int, 0, 8))
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

func pt2(seeds []int, translators [][][]int) int {
	var s int
	var l int
	for {
		s = l
		for i := len(translators) - 1; i >= 0; i-- {
			s = reverse(s, translators[i])
		}

		// check if s is valid seed number
		for i := 0; i < len(seeds); i += 2 {
			if s > seeds[i] && s < seeds[i]+seeds[i+1] {
				return l
			}
		}

		l++
	}
}

func main() {
	timeStart := time.Now()
	seeds, translators := parseInput()

	fmt.Printf("part 1: %d\n", pt1(seeds, translators))

	// part 2: reverse search for valid seed number
	fmt.Printf("part 2: %d\n", pt2(seeds, translators))

	// time
	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
