package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Pos struct {
	row int
	col int
}

func parseInput() ([][]rune, Pos) {
	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	grid := make([][]rune, 0)
	var start Pos
	for row, line := range strings.Split(string(bytes.TrimSpace(input)), "\n") {
		grid = append(grid, make([]rune, 0))
		for col, c := range line {
			grid[row] = append(grid[row], c)

			if c == 'S' {
				start.row = row
				start.col = col
			}
		}
	}
	return grid, start
}

func findExit(grid [][]rune, p Pos) rune {
	exits := ""

	N := grid[p.row-1][p.col]
	S := grid[p.row+1][p.col]
	E := grid[p.row][p.col+1]
	W := grid[p.row][p.col-1]

	if N == '|' || N == '7' || N == 'F' {
		exits += "N"
	}
	if E == '7' || E == '-' || E == 'J' {
		exits += "E"
	}
	if S == '|' || S == 'L' || S == 'J' {
		exits += "S"
	}
	if W == '-' || W == 'F' || W == 'L' {
		exits += "W"
	}

	m := map[string]rune{
		"NE": 'L',
		"NS": '|',
		"NW": 'J',
		"EW": '-',
		"ES": 'F',
		"SW": '7',
	}
	return m[exits]
}

func pt1(grid [][]rune, start Pos, cur Pos) int {
	distance := 0
	enteredFrom := 'E'

	for {
		distance++
		//fmt.Printf("%c from %c\n", grid[cur.row][cur.col], enteredFrom)

		switch grid[cur.row][cur.col] {
		// |: North or South
		case '|':
			if enteredFrom == 'N' {
				cur.row += 1
			} else {
				cur.row -= 1
				enteredFrom = 'S'
			}
			break

			// - West or East
		case '-':
			if enteredFrom == 'E' {
				cur.col -= 1
			} else {
				cur.col += 1
				enteredFrom = 'W'
			}
			break

			// 7: West or South
		case '7':
			if enteredFrom == 'S' {
				cur.col -= 1
				enteredFrom = 'E'
			} else {
				cur.row += 1
				enteredFrom = 'N'
			}
			break

			// F:South or East
		case 'F':
			if enteredFrom == 'S' {
				cur.col += 1
				enteredFrom = 'W'
			} else {
				cur.row += 1
				enteredFrom = 'N'
			}

			break

			// J: North or West
		case 'J':
			if enteredFrom == 'N' {
				cur.col -= 1
				enteredFrom = 'E'
			} else {
				enteredFrom = 'S'
				cur.row -= 1
			}
			break

		// L: North or East
		case 'L':
			if enteredFrom == 'N' {
				cur.col += 1
				enteredFrom = 'W'
			} else {
				cur.row -= 1
				enteredFrom = 'S'
			}

			break
		}

		if cur.row == start.row && cur.col == start.col {
			break
		}
	}

	return distance / 2
}

func main() {
	timeStart := time.Now()

	grid, start := parseInput()
	grid[start.row][start.col] = findExit(grid, start)

	a1 := pt1(grid, start, start)

	fmt.Print("--- Day 9: Mirage Maintenance ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", 0)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)

}
