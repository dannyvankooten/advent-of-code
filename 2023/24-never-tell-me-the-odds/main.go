package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Hailstone struct {
	px, py, pz float64
	dx, dy, dz float64
}

func parse(input []byte) []Hailstone {
	r := make([]Hailstone, 0)
	for _, line := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		h := Hailstone{}

		fmt.Sscanf(line, "%f, %f, %f @ %f, %f, %f", &h.px, &h.py, &h.pz, &h.dx, &h.dy, &h.dz)
		r = append(r, h)
	}

	return r
}

// generate y=ax+b formula for hailstone
func f(h Hailstone) (float64, float64) {
	a := (h.dy) / (h.dx)
	b := (h.py) + -(a * (h.px))
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
	if (ha.dx > 0 && x < ha.px) || (hb.dx > 0 && x < hb.px) {
		return -1, -1
	}
	if (ha.dx < 0 && x > ha.px) || (hb.dx < 0 && x > hb.px) {
		return -1, -1
	}

	y := a1*x + b1
	return x, y

	// a1 * x + b1 = a2 * x + b2
	// a1*x - a2*x = b2 - b1
	// (a1-a2)*x = b2 - b1
	// x = (b2-b1)/(a1-a2)

	// x + 1 = -0.5x + 22.5
	// x + 0.5x = 21.5
	// 1.5x = 21.5
	// x = (21.5 / 1.5)
}

func pt1(hailstones []Hailstone) int {
	count := 0
	for i := range hailstones {
		for j := i + 1; j < len(hailstones); j++ {
			x, y := intersection(hailstones[i], hailstones[j])
			// fmt.Printf("%v & %v = %.2f, %.2f\n", hailstones[i], hailstones[j], x, y)
			if x >= 200000000000000 && x <= 400000000000000 && y >= 200000000000000 && y <= 400000000000000 {
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
	pt2 := 0
	fmt.Printf("--- Day 24: Never Tell Me The Odds ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}

// 13368 too low
// 13396 too low
// 22815 too high
// 19523
