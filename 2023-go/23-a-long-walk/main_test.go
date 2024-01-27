package main

import (
	"os"
	"testing"
)

var a int

func BenchmarkS(b *testing.B) {
	input, _ := os.ReadFile("input.txt")
	grid := parse(input)

	for n := 0; n < b.N; n++ {
		a = solve(grid, false)
	}

	b.ReportAllocs()
}
