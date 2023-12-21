package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var seen = make(map[[3]int]bool)
var options []int = make([]int, 65+131*2)

func countStepsInfinite(grid []string, x int, y int, depth int, maxDepth int) {
	if depth == maxDepth {
		return
	}

	n := len(grid)
	for _, d := range [][]int{
		{0, -1}, // N
		{1, 0},  // E
		{0, 1},  // S
		{-1, 0}, // W
	} {
		x2 := x + d[0]
		y2 := y + d[1]

		// modulo like Python
		if grid[((y2%n)+n)%n][((x2%n)+n)%n] == '#' {
			continue
		}

		key := [3]int{x2, y2, depth}
		if _, ok := seen[key]; ok {
			continue
		}
		seen[key] = true

		options[depth] += 1
		countStepsInfinite(grid, x2, y2, depth+1, maxDepth)
	}

}

func findStart(grid []string) (int, int) {
	for y := range grid {
		for x := range grid[y] {
			if grid[y][x] == 'S' {
				return x, y
			}
		}
	}

	return -1, -1
}

func f(x int, a [3]int) int {
	b0 := a[0]
	b1 := a[1] - a[0]
	b2 := a[2] - a[1]

	return b0 + (b1 * x) + (x*(x-1)/2)*(b2-b1)
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := strings.Split(strings.TrimSpace(string(input)), "\n")

	x, y := findStart(grid)
	countStepsInfinite(grid, x, y, 0, 65+131*2)
	pt1 := options[63]
	pt2 := f(26501365/131, [3]int{options[64], options[64+131], options[64+131*2]})

	fmt.Printf("--- Day 21: Step Counter ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
