package main

import (
	"fmt"
	"math"
	"time"
)

func solve(times []int, distances []int) int {
	// -x^2 + tx - d > 0
	// x = (-t + sqrt(t^2 - 4 * -1 * -d ) / - 2
	// x = (-t - sqrt(t^2 - 4 * -1 * -d ) / - 2
	prod := 1
	for i := 0; i < len(times); i++ {
		t := float64(times[i])
		d := float64(distances[i])
		dis := math.Sqrt(math.Pow(t, 2) - 4*-1*-d)
		x1 := (-t - dis) / -2
		x2 := (-t + dis) / -2
		prod *= int(x1) - int(x2)
	}

	return prod
}

func main() {
	timeStart := time.Now()

	times := []int{61, 70, 90, 66}
	distances := []int{643, 1184, 1362, 1041}

	fmt.Print("---- Day 6: Wait For It ---\n")

	a1 := solve(times, distances)
	fmt.Printf("part 1: %d\n", a1)

	a2 := solve([]int{61709066}, []int{643118413621040})
	fmt.Printf("part 2: %d\n", a2)

	fmt.Printf("%.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
