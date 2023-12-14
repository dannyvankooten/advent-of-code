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

func isEqual(a [][]byte, b [][]byte) bool {
	for row := 0; row < len(a); row++ {
		if !bytes.Equal(a[row], b[row]) {
			return false
		}
	}

	return true
}

func copyGrid(a [][]byte) [][]byte {
	rows := make([][]byte, len(a))

	for i := range a {
		rows[i] = make([]byte, len(a[i]))
		copy(rows[i], a[i])
	}
	return rows
}

// Floyd's Tortoise and Hare algorithm
// https://en.wikipedia.org/wiki/Cycle_detection
func floyd(f func([][]byte), x0 [][]byte) (int, [][]byte) {
	hare := copyGrid(x0)
	tortoise := copyGrid(x0)

	f(tortoise)
	f(hare)
	f(hare)

	for !isEqual(tortoise, hare) {
		f(tortoise)
		f(hare)
		f(hare)
	}

	// // move both to start of cycle
	// mu := 0
	// tortoise = copyGrid(x0)
	// for !isEqual(tortoise, hare) {
	// 	mu += 1
	// 	f(tortoise)
	// 	f(hare)
	// }

	// move hare forward until start of next cycle
	// tortoise remains in place
	cycleLength := 1
	f(hare)
	for !isEqual(tortoise, hare) {
		f(hare)
		cycleLength += 1
	}

	// return length of cycle and current state of grid
	return cycleLength, hare
}

func main() {
	timeStart := time.Now()

	b, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := bytes.Split(bytes.TrimSpace(b), []byte("\n"))

	// pt1
	grid1 := copyGrid(grid)
	tilt(grid1, directions[0])
	pt1 := weight(grid1)

	// part 2
	cycle := func(g [][]byte) {
		tilt(g, directions[0])
		tilt(g, directions[1])
		tilt(g, directions[2])
		tilt(g, directions[3])
	}
	cycleLength, state := floyd(cycle, grid)
	cyclesRemaining := 1e9 % cycleLength
	for i := 0; i < cyclesRemaining; i++ {
		cycle(state)
	}

	pt2 := weight(state)
	fmt.Printf("--- Day 14: Parabolic Reflector Dish ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}

// 96464 too high
// 93451 too high