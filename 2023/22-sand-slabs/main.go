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
		mz := 0
		// look at all other bricks
		// bricks are sorted by increasing z1-index
		// so start looking at or just below a's z-index
		for j := i - 1; j >= 0; j-- {
			b := &bricks[j]

			if mz >= a.z1-1 {
				break
			}

			if b.z2 >= a.z1 {
				continue
			}

			if a.x2 >= b.x1 &&
				a.x1 <= b.x2 &&
				a.y2 >= b.y1 &&
				a.y1 <= b.y2 {
				mz = max(b.z2, mz)
			}
		}

		//
		diff := a.z1 - (mz + 1)

		if diff > 0 {
			a.z2 = a.z2 - diff
			a.z1 = mz + 1
			changes += 1
		}

	}
	return changes
}

func countRemainingSupports(below []int, fallen []bool) int {
	count := 0
	for _, b := range below {
		if !fallen[b] {
			count += 1
		}
	}

	return count
}

func count(bricks []Brick, i int, above [][]int, below [][]int, fallen []bool) int {
	// if bricks has nothing above it
	// it can safely be disintegrated
	if len(above[i]) == 0 {
		return 0
	}

	// alternatively, if it has stuff above it
	// but supported by more than one brick
	fallen[i] = true

	// DFS:
	c := 0
	stack := make([]int, 0)
	stack = append(stack, above[i]...)
	for len(stack) > 0 {
		i = stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		if countRemainingSupports(below[i], fallen) == 0 {
			c += 1
			fallen[i] = true
			stack = append(stack, above[i]...)
		}
	}

	return c
}

func solve(bricks []Brick, above [][]int, below [][]int) (int, int) {
	pt1 := 0
	pt2 := 0

	// find # of bricks with nothing above it
	fallen := make([]bool, len(bricks))

	for i := range above {
		clear(fallen)
		c := count(bricks, i, above, below, fallen)
		if c == 0 {
			pt1 += 1
		}
		pt2 += c
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

	above := make([][]int, len(bricks))
	below := make([][]int, len(bricks))
	for i := range bricks {
		// find bricks directly above
		a := &bricks[i]

		for j := range bricks {
			b := &bricks[j]
			if b.z1 != a.z2+1 {
				continue
			}

			if !(a.x2 >= b.x1 &&
				a.x1 <= b.x2 &&
				a.y2 >= b.y1 &&
				a.y1 <= b.y2) {
				continue
			}

			// b is directly above a
			above[i] = append(above[i], j)
			below[j] = append(below[j], i)
		}
	}

	pt1, pt2 := solve(bricks, above, below)

	fmt.Printf("--- Day 22: Sand Slabs ---\n")
	fmt.Printf("Part 1: %d (463)\n", pt1)
	fmt.Printf("Part 2: %d (89727)\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
