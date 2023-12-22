package main

import (
	"fmt"
	"log"
	"os"
	"slices"
	"strings"
	"time"
)

type Brick struct {
	x1 int
	y1 int
	z1 int
	x2 int
	y2 int
	z2 int
}

func parse(input []byte) []Brick {
	bricks := make([]Brick, 0, 1370)
	for _, line := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		b := Brick{}
		fmt.Sscanf(line, "%d,%d,%d~%d,%d,%d", &b.x1, &b.y1, &b.z1, &b.x2, &b.y2, &b.z2)
		bricks = append(bricks, b)
	}

	return bricks
}

func abs(n int) int {
	if n < 0 {
		return -n
	}

	return n
}

func fall(bricks []Brick) int {
	changes := 0

	for i := range bricks {
		a := &bricks[i]
		fell := false

		for a.z1 > 1 {
			// look at all other bricks
			// bricks are sorted by increasing z-index
			// so start looking at or just below a's z-index
			for j := i - 1; j >= 0; j-- {
				b := &bricks[j]

				// determine if there would be a collission
				// if we were to shift brick a down by 1 row
				if (a.z2-1) >= b.z1 &&
					(a.z1-1) <= b.z2 &&
					a.x2 >= b.x1 &&
					a.x1 <= b.x2 &&
					a.y2 >= b.y1 &&
					a.y1 <= b.y2 {
					goto nextbrick
				}
			}

			if !fell {
				changes += 1
				fell = true
			}

			a.z1 -= 1
			a.z2 -= 1
		}

		// label to skip falling this brick
	nextbrick:
	}
	return changes
}

func solve(bricks []Brick) (int, int) {

	// at this point the grid is stable
	// so what we really want is to count how many bricks
	tmp := make([]Brick, len(bricks))
	pt1 := 0
	pt2 := 0
	for i := range bricks {
		copy(tmp, bricks)
		tmp[i] = Brick{}
		changes := fall(tmp)

		if changes == 0 {
			pt1 += 1
		} else {
			pt2 += changes
		}
	}
	return pt1, pt2
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	// parse bricks, then fall until stable
	bricks := parse(input)
	slices.SortFunc(bricks, func(a, b Brick) int {
		return a.z1 - b.z1
	})
	fall(bricks)

	pt1, pt2 := solve(bricks)

	fmt.Printf("--- Day 22: Sand Slabs ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
