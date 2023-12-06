package main

import (
	"fmt"
	"time"
)

func solve(times []int, distances []int) int {
	prod := 1
	count := 0
	tMoving := 0

	for i, t := range times {
		count = 0

		start := t * 20 / 100
		end := t * 80 / 100

		for tCharging := start; tCharging <= end; tCharging++ {
			tMoving = t - tCharging

			if (tCharging * tMoving) > distances[i] {
				count++
			} else if count > 0 {
				break
			}
		}

		prod *= count
	}

	return prod
}

func main() {
	timeStart := time.Now()

	times := []int{61, 70, 90, 66}
	distances := []int{643, 1184, 1362, 1041}

	a1 := solve(times, distances)
	fmt.Printf("---- Day 6: Wait For It ---\npart 1: %d\n", a1)

	a2 := solve([]int{61709066}, []int{643118413621040})
	fmt.Printf("part 2: %d\n", a2)

	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
