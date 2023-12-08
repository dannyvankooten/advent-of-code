package main

import (
	"fmt"
	"os"
	"strings"
	"time"
)

func pt1(dirs string, m map[string][2]string) int {
	cur := "AAA"
	steps := 0

	for cur != "ZZZ" {
		for _, c := range dirs {
			if c == 'L' {
				cur = m[cur][0]
			} else {
				cur = m[cur][1]
			}
			steps += 1

			if cur == "ZZZ" {
				return steps
			}
		}
	}

	return steps
}

func pt2(dirs string, m map[string][2]string) int {
	curs := make([]string, 0)
	for c, _ := range m {
		if c[2] == 'A' {
			curs = append(curs, c)
		}
	}
	steps := make([]int, len(curs))

	for i, cur := range curs {
		for cur[2] != 'Z' {
			for _, turn := range dirs {
				if turn == 'L' {
					cur = m[cur][0]
				} else {
					cur = m[cur][1]
				}

				steps[i]++

				if cur[2] == 'Z' {
					break
				}
			}
		}
	}

	// find prime factors for each # of steps per starting location
	prod := 1
	for _, s := range steps {
		for _, f := range primeFactors(s) {
			// only multiply if we haven't already added this factor
			// NOTE: Go's map[int] would be slightly faster than this modulo here
			if prod%f != 0 {
				prod *= f
			}
		}
	}

	return prod
}

func parse(filename string) (string, map[string][2]string) {
	input, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}
	lines := strings.Split(string(input), "\n")
	dirs := lines[0]
	m := make(map[string][2]string)
	for _, l := range lines[2:] {
		if l == "" {
			continue
		}

		source := l[0:3]
		left := l[7:10]
		right := l[12:15]
		m[source] = [2]string{left, right}
	}

	return dirs, m
}

func primeFactors(n int) []int {
	pfs := make([]int, 0, 8)
	// Get the number of 2s that divide n
	for n%2 == 0 {
		pfs = append(pfs, 2)
		n = n / 2
	}

	// n must be odd at this point. so we can skip one element
	for i := 3; i*i <= n; i = i + 2 {
		// while i divides n, append i and divide n
		for n%i == 0 {
			pfs = append(pfs, i)
			n = n / i
		}
	}

	// if n is a prime number itself
	if n > 2 {
		pfs = append(pfs, n)
	}

	return pfs
}

func main() {
	timeStart := time.Now()
	dirs, m := parse("input.txt")
	a1 := pt1(dirs, m)
	a2 := pt2(dirs, m)
	fmt.Print("--- Day 8: Haunted Wasteland ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
