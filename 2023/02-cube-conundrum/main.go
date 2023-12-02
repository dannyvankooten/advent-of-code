package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

func main() {

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	timeStart := time.Now()
	pt1 := 0
	pt2 := 0

	// The Elf would first like to know which games would have been possible
	// if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?
	for i, l := range strings.Split(string(input), "\n") {
		if l == "" {
			continue
		}

		l = l[8:]
		max := [3]int{0, 0, 0}
		possible := true

		for _, s := range strings.Split(l, ";") {
			for _, h := range strings.Split(s, ",") {
				h = strings.TrimSpace(h)
				parts := strings.Split(h, " ")
				amount ,_ := strconv.Atoi(parts[0])
				color := parts[1]

				if color == "red" {
					if amount > 12 {
						possible = false
					}
					if amount > max[0] {
						max[0] = amount
					}
				}
				if color == "green" {
					if amount > 13 {
						possible = false
					}
					if amount > max[1] {
						max[1] = amount
					}
				}
				if color == "blue" {
					if amount > 14 {
						possible = false
					}
					if amount > max[2] {
						max[2] = amount
					}
				}
			}
		}

		if possible {
			pt1 += i + 1
		}

		pt2 += max[0] * max[1] * max[2]
	}

	fmt.Printf("part 1: %d\n", pt1)
	fmt.Printf("part 2: %d\n", pt2)
	fmt.Printf("time: %.2fms\n", float64(time.Since(timeStart).Microseconds()) / 1000)
}
