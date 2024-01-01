package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var seen map[[2]int]bool
var options []int

func lookup(grid []string, v [2]int) byte {
	v[0] = v[0] % len(grid)
	if v[0] < 0 {
		v[0] += len(grid)
	}

	v[1] = v[1] % len(grid)
	if v[1] < 0 {
		v[1] += len(grid)
	}

	return grid[v[1]][v[0]]
}

func mod(a int, m int) int {
	a %= m
	if a < 0 {
		a += m
	}
	return a
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
func newtonForwardDivDiff(x int, a0, a1, a2 int) int {
	b0 := a0
	b1 := a1 - a0
	b2 := a2 - a1

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
	new := make([][2]int, 0)
	new = append(new, [2]int{x, y})

	options = make([]int, rem+n*2+1)
	options[0] = 1

	for i := 1; i < rem+2*n+1; i++ {
		toVisit := new
		new = make([][2]int, 0)
		for _, k := range toVisit {
			// visit every neighbor
			for _, d := range [][]int{
				{0, -1}, // N
				{1, 0},  // E
				{0, 1},  // S
				{-1, 0}, // W
			} {
				v := [2]int{
					k[0] + d[0],
					k[1] + d[1],
				}
				if visited[v] {
					continue
				}

				if lookup(grid, v) == '#' {
					continue
				}

				new = append(new, v)
				visited[v] = true
			}
		}

		prev := 0
		if i > 1 {
			prev = options[i-2]
		}
		options[i] = len(new) + prev
	}

	pt1 := options[64]
	pt2 := newtonForwardDivDiff(26501365/n, options[rem+n*0], options[rem+n*1], options[rem+n*2])

	fmt.Printf("--- Day 21: Step Counter ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
