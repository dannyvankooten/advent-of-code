package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

const (
	E = iota
	S
	W
	N
)

var directions = []Pos{
	E: {1, 0},  // E
	S: {0, 1},  // S
	W: {-1, 0}, // W
	N: {0, -1}, //N
}

type Pos struct {
	x int
	y int
}

func beam(grid []string, energy [][]int, p Pos, dir int, depth int) {
	if depth > 28 {
		return
	}

	for {
		// bail if we just escaped the grid
		if p.y < 0 || p.y >= len(grid) {
			return
		}

		if p.x < 0 || p.x >= len(grid[p.y]) {
			return
		}

		// energize current tile
		energy[p.y][p.x] = 1

		// change dir?
		switch grid[p.y][p.x] {
		case byte('/'):
			// go north if traveling eastwards
			change := []int{
				E: N,
				W: S,
				S: W,
				N: E,
			}
			dir = change[dir]
			break

		case byte('\\'):
			change := []int{
				E: S,
				S: E,
				W: N,
				N: W,
			}
			dir = change[dir]
			break

		case byte('|'):
			// if going to East or West split
			if dir == E || dir == W {
				beam(grid, energy, p, N, depth+1)
				beam(grid, energy, p, S, depth+1)
				return
			}
			break

		case byte('-'):
			if dir == N || dir == S {
				beam(grid, energy, p, E, depth+1)
				beam(grid, energy, p, W, depth+1)
				return
			}
			break
		}

		// take a step in the current direction
		p.x += directions[dir].x
		p.y += directions[dir].y
	}
}

func makeEnergy(grid []string) [][]int {
	energy := make([][]int, len(grid))
	for row := range grid {
		energy[row] = make([]int, len(grid[row]))
	}
	return energy
}

func sumEnergy(energy [][]int) int {
	e := 0
	for _, row := range energy {
		for _, col := range row {
			if col > 0 {
				e += 1
			}
		}
	}
	return e
}

func clearEnergy(energy [][]int) {
	for row := range energy {
		for col := range energy[row] {
			energy[row][col] = 0
		}
	}
}

func pt2(grid []string) int {
	best := 0
	energy := makeEnergy(grid)

	// start at every row in first and last column
	for row := 0; row < len(grid); row++ {
		clearEnergy(energy)
		beam(grid, energy, Pos{0, row}, E, 0)
		e := sumEnergy(energy)
		if e > best {
			best = e
		}

		clearEnergy(energy)
		energy = makeEnergy(grid)
		beam(grid, energy, Pos{len(grid[0]) - 1, row}, W, 0)
		e = sumEnergy(energy)
		if e > best {
			best = e
		}
	}

	// start at every column in top row and bottom row
	for col := 0; col < len(grid[0]); col++ {

		// top row going south
		clearEnergy(energy)
		beam(grid, energy, Pos{col, 0}, S, 0)
		e := sumEnergy(energy)
		if e > best {
			best = e
		}

		// bottom row going north
		clearEnergy(energy)
		energy = makeEnergy(grid)
		beam(grid, energy, Pos{col, len(grid) - 1}, N, 0)
		e = sumEnergy(energy)
		if e > best {
			best = e
		}
	}

	return best
}

func pt1(grid []string) int {
	energy := makeEnergy(grid)
	beam(grid, energy, Pos{0, 0}, E, 0)
	return sumEnergy(energy)
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := strings.Split(string(bytes.TrimSpace(input)), "\n")

	a1 := pt1(grid)
	a2 := pt2(grid)
	fmt.Printf("--- Day 16: The Floor Will Be Lava ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}