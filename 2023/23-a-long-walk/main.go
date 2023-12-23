package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var directions = [][2]int{
	{1, 0},  // EAST
	{0, 1},  // SOUTH
	{-1, 0}, // WEST
	{0, -1}, // NORTH
}

func copyGrid(grid [][]rune) [][]rune {
	c := make([][]rune, len(grid))
	for i := range grid {
		c[i] = make([]rune, len(grid[i]))
		copy(c[i], grid[i])
	}
	return c
}

func pt1(grid [][]rune, x int, y int, steps int) int {
	var availableDirections [][2]int

	for {
		switch grid[y][x] {
		case '.':
			availableDirections = directions
			break
		case '>':
			availableDirections = directions[0:1]
			break
		case 'v':
			availableDirections = directions[1:2]
			break
		case '<':
			availableDirections = directions[2:3]
			break
		case '^':
			availableDirections = directions[3:4]
			break
		}

		// mark tile as visited
		grid[y][x] = 'O'

		options := make([][2]int, 0, 4)
		for _, d := range availableDirections {
			x2 := x + d[0]
			y2 := y + d[1]
			if x2 < 0 || y2 < 0 || y2 >= len(grid) || x2 >= len(grid[y2]) {
				continue
			}

			if grid[y2][x2] == '#' || grid[y2][x2] == 'O' {
				continue
			}

			options = append(options, [2]int{x2, y2})
		}

		if len(options) == 0 {
			return steps
		} else if len(options) > 1 {
			longest := 0
			for _, coords := range options {
				s := pt1(copyGrid(grid), coords[0], coords[1], steps+1)
				if s > longest {
					longest = s
				}
			}

			return longest
		}

		x = options[0][0]
		y = options[0][1]
		steps += 1
	}

}

func parse(input []byte) [][]rune {
	grid := make([][]rune, 0)
	for _, l := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		grid = append(grid, []rune(l))
	}
	return grid
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	grid := parse(input)

	pt1 := pt1(grid, 1, 0, 0)

	pt2 := 0
	fmt.Printf("--- Day 23: A Long Walk ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
