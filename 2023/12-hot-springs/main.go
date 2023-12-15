package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

func isValid2(line []rune, to int, blueprint []int) bool {
	g := -1
	i := 0
	//fmt.Printf("%#v %#v\n", string(line[0:to]), blueprint)
	for i < to {
		if line[i] == '.' {
			i++
			continue
		}

		if line[i] != '#' {
			panic("Unexpected character")
		}

		// new group
		g++

		// No additional groups allowed in blueprint: invalid permutation
		if g >= len(blueprint) {
			return false
		}

		// count contiguous '#' chars
		count := 0
		for i < to && line[i] == '#' {
			i++
			count++
		}
		//fmt.Printf("counted %d in group %d\n", count, g+1)

		// if we have chars left, count should match blueprint exactly
		if (i < to || len(line) == to) && count != blueprint[g] {
			return false
		}

		if count > blueprint[g] {
			return false
		}
	}

	// did we count all groups?
	if to >= len(line) {
		return g == len(blueprint)-1
	}

	return true
}

func permute(line []rune, from int, blueprint []int) int {
	if !isValid2(line, from, blueprint) {
		return 0
	}

	if from == len(line) {
		return 1
	}

	if line[from] != '?' {
		return permute(line, from+1, blueprint)
	}

	// create partial copy of everything to the right of current position
	lineCopy := make([]rune, len(line))
	copy(lineCopy, line)

	line[from] = '.'
	left := permute(line, from+1, blueprint)

	lineCopy[from] = '#'
	right := permute(lineCopy, from+1, blueprint)

	return left + right
}

func main() {
	timeStart := time.Now()
	//b, err := os.ReadFile("input.txt")
	b, err := os.ReadFile("input_test.txt")
	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(string(bytes.TrimSpace(b)), "\n")
	blueprint := make([]int, 0, 512)
	//blueprint2 := make([]int, 0, 512)

	pt1 := 0
	pt2 := 0
	for _, l := range lines {
		spacePos := strings.Index(l, " ")
		a := l[:spacePos]
		b := l[spacePos+1:]

		blueprint = blueprint[:0]
		for _, d := range strings.Split(b, ",") {
			n, err := strconv.Atoi(d)
			if err != nil {
				panic(err)
			}
			blueprint = append(blueprint, n)
		}

		// part 1
		f1 := permute([]rune(a), 0, blueprint)
		pt1 += f1

		// part 2
		a2 := strings.Join([]string{a, a, a, a, a}, "?")
		b2 := make([]int, 0, len(blueprint)*5)
		for i := 0; i < 5; i++ {
			b2 = append(b2, blueprint...)
		}
		f2 := permute([]rune(a2), 0, b2)
		pt2 += f2
		// fmt.Printf("%03d / %03d: %d arrangements \n", lineno, len(lines)-1, f2)
	}

	fmt.Printf("--- Day 12: Hot Springs ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}