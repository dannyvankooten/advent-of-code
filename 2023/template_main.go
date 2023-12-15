package main

import (
	"fmt"
	"log"
	"os"
	"time"
)

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Input: %s\n", input)

	pt1 := 0
	pt2 := 0
	fmt.Printf("--- He Forgot To Set The Puzzle Name Today ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}