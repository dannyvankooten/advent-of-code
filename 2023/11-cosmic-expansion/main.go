package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Galaxy struct {
	r int64 // row
	c int64 // col
}

func parse(filename string, expand int64) []Galaxy {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(strings.TrimSpace(string(bytes)), "\n")
	width := int64(len(lines[0]))
	galaxies := make([]Galaxy, 0, 32)
	rowOffset := int64(0)

	for r, row := range lines {
		rowHasGalaxy := false

		for c, col := range row {
			if col == '#' {
				g := Galaxy{int64(r) + rowOffset, int64(c)}
				galaxies = append(galaxies, g)
				rowHasGalaxy = true
			}
		}

		if false == rowHasGalaxy {
			rowOffset += expand
		}
	}

	for c := int64(0); c < width; c++ {
		rowHasGalaxy := false
		for _, g := range galaxies {
			if g.c == c {
				rowHasGalaxy = true
				break
			}
		}
		if rowHasGalaxy == false {
			// shift every galaxy to the right of this column
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

func sumDistances(galaxies []Galaxy) int64 {
	distance := int64(0)
	for g1, a := range galaxies {
		for _, b := range galaxies[g1+1:] {
			r := a.r - b.r
			c := a.c - b.c
			if r < 0 {
				r *= -1
			}
			if c < 0 {
				c *= -1
			}
			distance += r + c
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
