package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"time"
)

const OBJ_ROCK = byte('O')
const OBJ_EMPTY = byte('.')

var directions = [][]int{
	{0, -1}, // N
	{-1, 0}, // W
	{0, 1},  // S
	{1, 0},  // E
}

func tilt(grid [][]byte, dir []int) {
	width := len(grid[0])
	height := len(grid)

	startCol := 0
	colStep := 1
	if dir[0] == 1 {
		startCol = width - 1
		colStep = -1
	}
	startRow := 0
	rowStep := 1
	if dir[1] == 1 {
		startRow = height - 1
		rowStep = -1
	}

	for row := startRow; row < height && row >= 0; row += rowStep {
		for col := startCol; col < width && col >= 0; col += colStep {
			if grid[row][col] != OBJ_ROCK {
				continue
			}

			// walk into direction until obstacle
			c := col
			r := row
			for c+dir[0] >= 0 && c+dir[0] < width && r+dir[1] >= 0 && r+dir[1] < height && grid[r+dir[1]][c+dir[0]] == OBJ_EMPTY {
				c += dir[0]
				r += dir[1]
			}

			// place rock at new position
			if r != row || c != col {
				grid[r][c] = OBJ_ROCK
				grid[row][col] = OBJ_EMPTY
			}
		}
	}
}

func weight(grid [][]byte) int {
	width := len(grid[0])
	height := len(grid)
	weight := 0

	for row := 0; row < height; row++ {
		for col := 0; col < width; col++ {
			if grid[row][col] != OBJ_ROCK {
				continue
			}

			weight += height - row
		}
	}

	return weight
}

func main() {
	timeStart := time.Now()

	b, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := bytes.Split(bytes.TrimSpace(b), []byte("\n"))

	// pt1
	tilt(grid, directions[0])
	pt1 := weight(grid)

	grid = bytes.Split(bytes.TrimSpace(b), []byte("\n"))
	// seen := make(map[int]int)
	for i := 0; i < (18*10)+10; i++ {
		tilt(grid, directions[0])
		tilt(grid, directions[1])
		tilt(grid, directions[2])
		tilt(grid, directions[3])

		// w := weight(grid)
		// if prev, ok := seen[w]; ok {
		// 	fmt.Printf("%d: %d (previously at %d, diff = %d)\n", i, weight(grid), prev, i-prev)
		// } else {
		// 	fmt.Printf("%d: %d\n", i, weight(grid))
		// }
		// seen[w] = i
	}

	pt2 := weight(grid)
	fmt.Printf("--- Day 14: Parabolic Reflector Dish ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}

// 96464 too high
// 93451 too high