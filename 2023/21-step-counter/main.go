package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var seen map[[3]int]bool
var options []int

func lookup(grid []string, x int, y int) byte {
	x = x % len(grid)
	if x < 0 {
		x += len(grid)
	}

	y = y % len(grid)
	if y < 0 {
		y += len(grid)
	}

	return grid[y][x]
}

func mod(a int, m int) int {
	a %= m
	if a < 0 {
		a += m
	}
	return a
}

func countSteps(grid []string, x int, y int, depth int, maxDepth int) {
	if depth > maxDepth {
		return
	}

	for _, d := range [][]int{
		{0, -1}, // N
		{1, 0},  // E
		{0, 1},  // S
		{-1, 0}, // W
	} {
		x2 := x + d[0]
		y2 := y + d[1]
		if lookup(grid, x2, y2) == '#' {
			continue
		}

		key := [3]int{x2, y2, depth}
		if _, ok := seen[key]; ok {
			continue
		}
		seen[key] = true

		options[depth] += 1
		countSteps(grid, x2, y2, depth+1, maxDepth)
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

// see https://en.wikipedia.org/wiki/Newton_polynomial
func newtonForwardDivDiff(x int, a [3]int) int {
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
	n := len(grid)
	rem := 26501365 % n
	x, y := findStart(grid)

	visited := map[[2]int]bool{
		{x, y}: true,
	}
	new := map[[2]int]bool{
		{x, y}: true,
	}

	options = make([]int, rem+n*2+1)
	options[0] = 1

	for i := 1; i < rem+2*n+1; i++ {
		toVisit := new
		new = make(map[[2]int]bool)
		for k := range toVisit {
			// visit every neighbor
			for _, d := range [][]int{
				{0, -1}, // N
				{1, 0},  // E
				{0, 1},  // S
				{-1, 0}, // W
			} {
				x2 := k[0] + d[0]
				y2 := k[1] + d[1]
				k2 := [2]int{x2, y2}

				if _, ok := visited[k2]; ok {
					continue
				}

				if lookup(grid, x2, y2) == '#' {
					continue
				}

				new[k2] = true
			}
		}

		visited = toVisit

		prev := 0
		if i > 1 {
			prev = options[i-2]
		}
		options[i] = len(new) + prev
	}

	pt1 := options[64]
	pt2 := newtonForwardDivDiff(26501365/n, [3]int{options[rem+n*0], options[rem+n*1], options[rem+n*2]})

	fmt.Printf("--- Day 21: Step Counter ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
