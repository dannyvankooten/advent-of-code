package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"unicode"
)

func main() {
	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	grid := make([]string, 0, 0)
	for _, line := range strings.Split(string(input), "\n") {
		if line == "" {
			continue
		}
		grid = append(grid, line)
	}


	pt1(grid)
	pt2(grid)
}

func pt1(grid []string) {
	// loop over grid, check all neighbors
	dxvals := []int{-1, 0, 1}
	dyvals := []int{-1, 0, 1}
	sum := 0
	number := 0
	inNumber := false
	symbolNeighbor := false

	for y := 0; y < len(grid); y++ {

		// add to sum
		if inNumber && symbolNeighbor {
			sum += number
		}

		number = 0
		inNumber = false
		symbolNeighbor = false

		for x := 0; x < len(grid[y]); x++ {
			if unicode.IsDigit(rune(grid[y][x])) {
				number = (number * 10) + int(grid[y][x] - '0')
				inNumber = true

				// check neighbors
				for _, dy := range dyvals {
					for _, dx := range dxvals {
						y2 := y + dy
						x2 := x + dx

						if x2 >= 0 && x2 < len(grid[y]) && y2 >= 0 && y2 < len(grid) {
							if grid[y2][x2] != '.' && ! unicode.IsDigit(rune(grid[y2][x2])){
								symbolNeighbor = true
							}
						}
					}
				}
			} else {
				// add to sum
				if inNumber && symbolNeighbor {
					sum += number
				}

				number = 0
				inNumber = false
				symbolNeighbor = false
			}
		}
	}

	fmt.Printf("part 1: %d\n", sum)
}

func parse(grid []string, x int, y int) int {
	for x > 0 && unicode.IsDigit(rune(grid[y][x-1])) {
		x--
	}

	n := 0
	for x < len(grid[y]) && unicode.IsDigit(rune(grid[y][x])) {
		n = (n * 10) + int(grid[y][x] - '0')
		x++
	}

	return n
}

func pt2(grid []string) {
	// loop over grid, check all neighbors
	dxvals := []int{-1, 0, 1}
	dyvals := []int{-1, 0, 1}
	sum := 0

	for y := 0; y < len(grid); y++ {
		for x := 0; x < len(grid[y]); x++ {
			if grid[y][x] != '*' {
				continue
			}

			ratios := make([]int, 0)
			// found a gear, go over neighbors
			for _, dy := range dyvals {
				for _, dx := range dxvals {
					y2 := y + dy
					x2 := x + dx

					if x2 >= 0 && x2 < len(grid[y]) && y2 >= 0 && y2 < len(grid)  {
						if unicode.IsDigit(rune(grid[y2][x2])) {
							v := parse(grid, x2, y2)

							uniq := true
							for _, v2 := range ratios {
								if v2 == v {
									uniq = false
								}
							}
							if uniq {
								ratios = append(ratios, v)
							}
						}
					}
				}
			}

			if len(ratios) == 2 {
				sum += ratios[0] * ratios[1]
			}
		}
	}

	fmt.Printf("part 2: %d\n", sum)
}
