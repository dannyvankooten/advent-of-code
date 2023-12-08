package main

import (
	"fmt"
	"os"
	"strings"
	"time"
)

func pt1(dirs string, m map[string][]string) int {
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

	return -1
}

func parse(filename string) (string, map[string][]string) {
	input, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}
	lines := strings.Split(string(input), "\n")
	dirs := lines[0]
	m := make(map[string][]string)
	for _, l := range lines[2:] {
		if l == "" {
			continue
		}

		source := l[0:3]
		left := l[7:10]
		right := l[12:15]
		m[source] = []string{left, right}
	}
	return dirs, m
}
func main() {
	timeStart := time.Now()
	dirs, m := parse("input.txt")
	a1 := pt1(dirs, m)

	a2 := 0
	fmt.Print("--- Day 8: Haunted Wasteland ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
