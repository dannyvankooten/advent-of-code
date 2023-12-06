package main

import (
	"fmt"
	"os"
	"strings"
	"time"
	"unicode"
)

func main() {
	timeStart := time.Now()
	input, err := os.ReadFile("input.txt")
	if err != nil {
		panic(err)
	}

	grid := make([][]rune, 0, 140)
	for _, line := range strings.Split(string(input), "\n") {
		if line == "" {
			continue
		}
		grid = append(grid, []rune(line))
	}

    fmt.Print("--- Day 3: Gear Ratios ---\n")
	fmt.Printf("Part 1: %d\n", pt1(grid))
	fmt.Printf("Part 2: %d\n", pt2(grid))
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds()) / 1000.0)
}

func pt1(grid [][]rune) int {
	// loop over grid, check all neighbors
	dxvals := []int{-1, 0, 1}
	dyvals := []int{-1, 0, 1}
	sum := 0
	number := 0
	symbolNeighbor := false

	for y := 0; y < len(grid); y++ {
		// if prevous line ended in a number, add to sum
		if symbolNeighbor && number > 0 {
			sum += number
		}
		number = 0
		symbolNeighbor = false

		for x := 0; x < len(grid[y]); x++ {
			if unicode.IsDigit(grid[y][x]) {
				number = (number * 10) + int(grid[y][x] - '0')

				// check neighbors
				for _, dy := range dyvals {
					for _, dx := range dxvals {
						y2 := y + dy
						x2 := x + dx

						if x2 >= 0 && x2 < len(grid[y]) && y2 >= 0 && y2 < len(grid) && (x2 != x || y2 != y) {
							if grid[y2][x2] != '.' && ! unicode.IsDigit(grid[y2][x2]){
								symbolNeighbor = true
							}
						}
					}
				}
			} else {
				// add to sum
				if symbolNeighbor {
					sum += number
				}

				number = 0
				symbolNeighbor = false
			}
		}
	}

    return sum
}

func parse(grid [][]rune, x int, y int) int {
	for x > 0 && unicode.IsDigit(grid[y][x-1]) {
		x--
	}

	n := 0
	for x < len(grid[y]) && unicode.IsDigit(grid[y][x]) {
		n = (n * 10) + int(grid[y][x] - '0')
		x++
	}

	return n
}

func pt2(grid [][]rune) int {
	// loop over grid, check all neighbors
	dxvals := []int{-1, 0, 1}
	dyvals := []int{-1, 0, 1}
	sum := 0

	ratios := make([]int, 0)

	for y := 0; y < len(grid); y++ {
		for x := 0; x < len(grid[y]); x++ {
			if grid[y][x] != '*' {
				continue
			}

			// found a gear, go over neighbors
			ratios = ratios[:0]
			for _, dy := range dyvals {
				for _, dx := range dxvals {
					y2 := y + dy
					x2 := x + dx

					if x2 >= 0 && x2 < len(grid[y]) && y2 >= 0 && y2 < len(grid)  {
						if unicode.IsDigit(grid[y2][x2]) {
							v := parse(grid, x2, y2)

							uniq := true
							for _, v2 := range ratios {
								if v2 == v {
									uniq = false
									break
								}
							}
							if uniq {
								ratios = append(ratios, v)

								if len(ratios) > 2 {
									goto next
								}
							}
						}
					}
				}
			}

			if len(ratios) == 2 {
				sum += ratios[0] * ratios[1]
			}

			next:
		}
	}

	return sum
}
