package main

import (
	"fmt"
	"log"
	"math"
	"os"
	"strings"
	"time"
)

type Point [2]int

var start Point
var end Point

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

func getPoints(grid []string) []Point {
	points := make([]Point, 2, 256)
	points[0] = start
	points[1] = end
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
	return points
}

type Neighbor struct {
	dist int
	pos  Point
}

func solve(grid []string, slopes bool) int {
	// parse all points of interest
	points := getPoints(grid)

	// create adjacency graph
	stack := make([]Neighbor, 1, 256)
	seen := make([]bool, 141*141)
	ispoi := make([]bool, 141*141)
	graph := make([][]Neighbor, 141*141)
	for _, p := range points {
		ispoi[key(p)] = true
	}

	// for every point in pt,  walk until wall or oob
	for _, p := range points {
		stack = stack[:1]
		stack[0] = Neighbor{0, p}

		clear(seen)
		pk := key(p)
		seen[pk] = true

		for {
			l := len(stack) - 1
			if l < 0 {
				break
			}
			item := stack[l]
			stack = stack[:l]

			// if we ended on a point, add it to our graph
			if item.dist != 0 && ispoi[key(item.pos)] {
				graph[pk] = append(graph[pk], item)
				continue
			}

			// For part 2, consider all directions
			var dirs []Point

			// For part 1, only consider directions given current tile
			// Ie, a sloped tile will steer us in a single direction
			if slopes {
				dirs = directions[grid[item.pos[0]][item.pos[1]]]
			} else {
				dirs = directions['.']
			}
			for _, d := range dirs {
				p := Point{item.pos[0] + d[0], item.pos[1] + d[1]}

				if p[0] < 0 || p[0] >= len(grid) {
					continue
				}

				if p[1] < 0 || p[1] >= len(grid[0]) {
					continue
				}

				if grid[p[0]][p[1]] == '#' || seen[key(p)] {
					continue
				}

				stack = append(stack, Neighbor{dist: item.dist + 1, pos: p})
				seen[key(p)] = true
			}
		}
	}

	return dfs(graph, start)
}

var seen = make([]bool, 141*141)

func key(p Point) int {
	return p[0]*141 + p[1]
}

func dfs(graph [][]Neighbor, u Point) int {
	if u == end {
		return 0
	}

	longest := math.MinInt
	k := key(u)
	seen[k] = true
	for _, v := range graph[k] {
		if seen[key(v.pos)] {
			continue
		}

		longest = max(longest, dfs(graph, v.pos)+v.dist)
	}
	seen[k] = false

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
	fmt.Printf("Part 1: %d (expected: 2502)\n", pt1)
	fmt.Printf("Part 2: %d (expected: 6726)\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
