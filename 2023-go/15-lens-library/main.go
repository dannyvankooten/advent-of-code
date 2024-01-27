package main

import (
	"fmt"
	"log"
	"os"
	"slices"
	"strconv"
	"strings"
	"time"
)

func hash(s string) int {
	sum := 0
	for _, c := range s {
		sum += int(c)
		sum *= 17
		sum %= 256
	}
	return sum
}

func pt1(input []byte) int {
	sum := 0
	for _, step := range strings.Split(string(input), ",") {
		sum += hash(step)
	}
	return sum
}

type Lens struct {
	Label       string
	FocalLength int
}

// remove removes the given element at index i from the slice
// remaining elements are shifted to the left
func remove[E any](slice []E, s int) []E {
	return append(slice[:s], slice[s+1:]...)
}

// parseStep parses the given step into a box ID, add/remove bool and Lens struct
func parseStep(s string) (int, bool, Lens) {
	labelEnd := strings.Index(s, "=")
	addLens := true
	if labelEnd == -1 {
		labelEnd = strings.Index(s, "-")
		addLens = false
	}

	// only box additions have a focal length
	focalLength := 0
	if addLens {
		var err error
		if focalLength, err = strconv.Atoi(s[labelEnd+1:]); err != nil {
			panic(err)
		}
	}

	lens := Lens{
		Label:       s[:labelEnd],
		FocalLength: focalLength,
	}
	boxId := hash(lens.Label)
	return boxId, addLens, lens
}

func pt2(input []byte) int {
	boxes := make([][]Lens, 256)
	for _, step := range strings.Split(string(input), ",") {
		boxId, addLens, lens := parseStep(step)
		existing := slices.IndexFunc(boxes[boxId], func(l Lens) bool {
			return l.Label == lens.Label
		})

		if addLens {
			if existing > -1 {
				// replace exiting lens with same label
				boxes[boxId][existing] = lens
			} else {
				// add to end of box
				boxes[boxId] = append(boxes[boxId], lens)
			}
		} else {
			// remove from box if exists
			if existing > -1 {
				boxes[boxId] = remove(boxes[boxId], existing)
			}
		}
	}

	a := 0
	for boxId := 0; boxId < 256; boxId++ {
		for slot, lens := range boxes[boxId] {
			a += (boxId + 1) * (slot + 1) * lens.FocalLength
		}
	}

	return a
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	pt1 := pt1(input)
	pt2 := pt2(input)
	fmt.Printf("--- Day 15: Lens Library ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}