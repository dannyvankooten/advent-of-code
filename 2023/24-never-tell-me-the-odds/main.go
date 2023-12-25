package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Hailstone struct {
	px, py, pz int
	dx, dy, dz int
}

func parse(input []byte) []Hailstone {
	r := make([]Hailstone, 0)
	for _, line := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		h := Hailstone{}

		fmt.Sscanf(line, "%d, %d, %d @ %d, %d, %d", &h.px, &h.py, &h.pz, &h.dx, &h.dy, &h.dz)
		r = append(r, h)
	}

	return r
}

// generate y=ax+b formula for hailstone
func f(h Hailstone) (float64, float64) {
	a := float64(h.dy) / float64(h.dx)
	b := float64(h.py) + -(a * float64(h.px))
	return a, b
}

// intersection
func intersection(ha Hailstone, hb Hailstone) (float64, float64) {
	// ax + b = ax + b
	a1, b1 := f(ha)
	a2, b2 := f(hb)

	if a1 == a2 {
		return -1, -1
	}

	x := (b2 - b1) / (a1 - a2)

	// determine whether intersection is in future
	if (ha.dx > 0 && x < float64(ha.px)) || (hb.dx > 0 && x < float64(hb.px)) {
		return -1, -1
	}
	if (ha.dx < 0 && x > float64(ha.px)) || (hb.dx < 0 && x > float64(hb.px)) {
		return -1, -1
	}

	y := a1*x + b1
	return x, y

	// a1 * x + b1 = a2 * x + b2
	// a1*x - a2*x = b2 - b1
	// (a1-a2)*x = b2 - b1
	// x = (b2-b1)/(a1-a2)
}

// count all hailstone pairs that intersect at x,y coords inside 2e14 & 4e14
func pt1(hailstones []Hailstone) int {
	count := 0
	for i := range hailstones {
		for j := i + 1; j < len(hailstones); j++ {
			x, y := intersection(hailstones[i], hailstones[j])
			if x >= 2e14 && x <= 4e14 && y >= 2e14 && y <= 4e14 {
				count += 1
			}
		}
	}
	return count
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	hailstones := parse(input)
	pt1 := pt1(hailstones)

	// pt2, see python...
	// didn't want to deal with z3 in go

	fmt.Printf("--- Day 24: Never Tell Me The Odds ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", 0)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
