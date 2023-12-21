package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var plots = make(map[[2]int]bool)
var seen = make(map[[3]int]bool)

func countSteps(grid []string, x int, y int, depth int, maxDepth int) int {
	// no need to recurse further if we've already been here
	key := [3]int{x, y, depth}
	if _, ok := seen[key]; ok {
		return 0
	}

	if depth == maxDepth {
		plots[[2]int{x, y}] = true
		return 1
	}

	seen[key] = true
	count := 0
	for _, d := range [][]int{
		{0, -1}, // N
		{1, 0},  // E
		{0, 1},  // S
		{-1, 0}, // W
	} {
		x2 := x + d[0]
		y2 := y + d[1]

		if x2 < 0 || y2 < 0 || y2 >= len(grid) || x2 >= len(grid[y2]) {
			continue
		}

		if grid[y2][x2] == '#' {
			continue
		}

		count += countSteps(grid, x2, y2, depth+1, maxDepth)
	}

	return count
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

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := strings.Split(strings.TrimSpace(string(input)), "\n")

	x, y := findStart(grid)
	countSteps(grid, x, y, 0, 64)
	pt1 := len(plots)

	pt2 := 0
	fmt.Printf("--- Day 21: Step Counter ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}

// 1202 too low
// 1203 too low
