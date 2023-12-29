package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Point [2]int

var start Point
var end Point
var seen = map[Point]bool{}

var directions map[byte][]Point = map[byte][]Point{
	'.': {{-1, 0}, {1, 0}, {0, -1}, {0, 1}},
	'^': {{-1, 0}},
	'>': {{0, 1}},
	'v': {{1, 0}},
	'<': {{0, -1}},
}

func parse(input []byte) []string {
	grid := strings.Split(strings.TrimSpace(string(input)), "\n")
	start = Point{0, 1}
	end = Point{len(grid) - 1, len(grid) - 2}
	return grid
}

func solve(grid []string, slopes bool) int {
	// parse all points of interest

	points := []Point{start, end}

	for r, row := range grid {
		for c, ch := range row {
			if ch == '#' {
				continue
			}

			neighbors := 0
			for _, d := range []Point{{-1, 0}, {1, 0}, {0, 1}, {0, -1}} {
				nr := r + d[0]
				nc := c + d[1]
				if nr < 0 || nc < 0 || nr >= len(grid) || nc >= len(grid[0]) || grid[nr][nc] == '#' {
					continue
				}

				neighbors += 1
			}

			// if this rc has 3 or more neighbors, it's a junction
			if neighbors >= 3 {
				points = append(points, Point{r, c})
			}
		}
	}

	// create adjacency graph
	graph := make(map[Point]map[Point]int)
	for _, p := range points {
		graph[p] = make(map[Point]int, 0)
	}

	// for every point in pt,  walk until wall or oob
	for _, p := range points {
		stack := make([][]int, 1, 64)
		stack[0] = []int{0, p[0], p[1]}
		seen := map[Point]bool{
			p: true,
		}

		for len(stack) > 0 {
			item := stack[len(stack)-1]
			stack = stack[:len(stack)-1]
			n := item[0]
			r := item[1]
			c := item[2]

			// if we ended on a point, add it to our graph
			_, isPoint := graph[Point{r, c}]
			if n != 0 && isPoint {
				graph[p][Point{r, c}] = n
				continue
			}

			// For part 2, consider all directions
			dirs := directions['.']

			// For part 1, only consider directions given current tile
			// Ie, a sloped tile will steer us in a single direction
			if slopes {
				dirs = directions[grid[r][c]]
			}
			for _, d := range dirs {
				nr := r + d[0]
				nc := c + d[1]
				if nr < 0 || nc < 0 || nr >= len(grid) || nc >= len(grid[0]) || grid[nr][nc] == '#' || seen[[2]int{nr, nc}] {
					continue
				}

				stack = append(stack, []int{n + 1, nr, nc})
				seen[Point{nr, nc}] = true
			}
		}
	}

	return dfs(graph, start)
}

func dfs(graph map[Point]map[Point]int, u Point) int {
	if u == end {
		return 0
	}

	longest := -(1 << 32)

	seen[u] = true
	for n, steps := range graph[u] {
		if seen[n] {
			continue
		}

		dist := dfs(graph, n) + steps
		if dist > longest {
			longest = dist
		}
	}
	delete(seen, u)

	return longest
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	grid := parse(input)

	pt1 := solve(grid, true)
	pt2 := solve(grid, false)

	fmt.Printf("--- Day 23: A Long Walk ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
