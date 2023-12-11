package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Galaxy struct {
	r int // row
	c int // col
}

func parse(filename string, expand int) []Galaxy {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(strings.TrimSpace(string(bytes)), "\n")
	galaxies := make([]Galaxy, 0, 512)
	rowOffset := 0

	// 1st pass: iterate row-wise and create all galaxies with proper row offset for expanded universe
	for r, row := range lines {
		rowHasGalaxy := false

		for c, col := range row {
			if col == '#' {
				galaxies = append(galaxies, Galaxy{r + rowOffset, c})
				rowHasGalaxy = true
			}
		}

		if false == rowHasGalaxy {
			rowOffset += expand
		}
	}

	// 2nd pass: iterate col-wise and add column offsets to account for expansions
	width := len(lines[0])
	for c := 0; c < width; c++ {
		hasGalaxy := false
		for _, g := range galaxies {
			if g.c == c {
				hasGalaxy = true
				break
			}
		}
		if hasGalaxy == false {
			// shift every galaxy to the right of this column by expand amounts
			for i, _ := range galaxies {
				if galaxies[i].c > c {
					galaxies[i].c += expand
				}
			}

			width += expand
			c += expand
		}
	}
	return galaxies
}

func manhattan(a Galaxy, b Galaxy) int {
	r := a.r - b.r
	c := a.c - b.c
	if r < 0 {
		r *= -1
	}
	if c < 0 {
		c *= -1
	}
	return r + c
}

func sumDistances(galaxies []Galaxy) int {
	distance := 0
	for g1, a := range galaxies {
		for _, b := range galaxies[g1+1:] {
			distance += manhattan(a, b)
		}
	}
	return distance
}

func main() {
	timeStart := time.Now()
	pt1 := sumDistances(parse("input.txt", 1))
	pt2 := sumDistances(parse("input.txt", 1000000-1))

	fmt.Printf("%s\n", "--- Day 11: Cosmic Expansion ---")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)

}
