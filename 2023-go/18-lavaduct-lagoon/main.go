package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type PlanItem struct {
	direction rune
	length    int
	color     int
}

type Point struct {
	x, y int
}

var dirs = map[int]rune{
	0: 'R',
	1: 'D',
	2: 'L',
	3: 'U',
}

func parse(input []byte, useHex bool) []PlanItem {
	items := make([]PlanItem, 0)
	for _, line := range strings.Split(string(input), "\n") {
		var err error
		parts := strings.Split(line, " ")
		i := PlanItem{}

		i.direction = rune(parts[0][0])
		i.length, err = strconv.Atoi(parts[1])
		if err != nil {
			panic(err)
		}

		var n int64
		hex := parts[2][2 : len(parts[2])-1]
		if useHex {
			// first 5 chars are length
			n, err = strconv.ParseInt(hex[5:], 16, 32)
			if err != nil {
				panic(err)
			}

			// 6th char is direction
			i.direction = dirs[int(n)]
			n, err = strconv.ParseInt(hex[:5], 16, 32)
			if err != nil {
				panic(err)
			}
			i.length = int(n)
		} else {
			n, err = strconv.ParseInt(hex, 16, 32)
			if err != nil {
				panic(err)
			}
			i.color = int(n)
		}

		items = append(items, i)
	}
	return items
}

func area(p []Point) int {
	sum := 0
	i := 0
	j := len(p) - 1
	for i < len(p) {
		sum += (p[i].x * p[j].y) - (p[j].x * p[i].y)
		j = i
		i += 1
	}

	if sum < 0 {
		sum = -sum
	}
	return sum / 2
}

func solve(plan []PlanItem) int {
	vectors := make([]Point, 0)
	x, y := 0, 0
	vectors = append(vectors, Point{x, y})
	linelength := 0
	for _, item := range plan {
		dy, dx := 0, 0
		switch item.direction {
		case 'R':
			dx = 1
			break
		case 'D':
			dy = 1
			break

		case 'L':
			dx = -1
			break

		case 'U':
			dy = -1
			break
		}

		x += dx * (item.length)
		y += dy * (item.length)
		linelength += (item.length)
		vectors = append(vectors, Point{x, y})
	}

	// lol, accidental pick's theorem
	return linelength/2 + area(vectors) + 1
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	plan := parse(input, false)
	a1 := solve(plan)

	plan = parse(input, true)
	a2 := solve(plan)
	fmt.Printf("--- Day 18: Lavaduct Lagoon ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}