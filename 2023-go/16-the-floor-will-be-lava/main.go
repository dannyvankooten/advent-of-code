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

var MIRROR_A = []int{
	E: N,
	W: S,
	S: W,
	N: E,
}

var MIRROR_B = []int{
	E: S,
	S: E,
	W: N,
	N: W,
}

func (c *Grid) Beam(p Pos, dir int) {
	width := len(c.tiles[0])
	height := len(c.tiles)

	for {
		// create single int key from coords + direction
		// grid in input is 110x110 so we really only need 21 bits
		key := (p.y * 110 * 10) + (p.x * 10) + dir
		if c.seen[key] == true {
			return
		}

		// energize current tile
		c.energized[p.y][p.x] = true
		c.seen[key] = true

		// change dir?
		switch c.tiles[p.y][p.x] {
		case byte('/'):
			// go north if traveling eastwards
			dir = MIRROR_A[dir]
			break

		case byte('\\'):
			dir = MIRROR_B[dir]
			break

		case byte('|'):
			// if going to East or West split
			if dir == E || dir == W {
				c.Beam(p, N)
				dir = S
			}
			break

		case byte('-'):
			if dir == N || dir == S {
				c.Beam(p, E)
				dir = W
			}
			break
		}

		p.x += directions[dir].x
		p.y += directions[dir].y

		// bail if we just escaped the grid
		if p.y < 0 || p.y >= height {
			return
		}

		if p.x < 0 || p.x >= width {
			return
		}

	}
}

func makeEnergy(grid []string) [][]bool {
	energy := make([][]bool, len(grid))
	for row := range grid {
		energy[row] = make([]bool, len(grid[row]))
	}
	return energy
}

func (c *Grid) CountEnergizedTiles() int {
	e := 0
	for _, row := range c.energized {
		for _, col := range row {
			if col {
				e += 1
			}
		}
	}
	return e
}

func (c *Grid) ClearEnergy() {
	for row := range c.energized {
		for col := range c.energized[row] {
			c.energized[row][col] = false
		}
	}
}

func pt2(c *Grid) int {
	best := 0
	height := len(c.tiles)
	width := len(c.tiles[0])

	// start at every row in first and last column
	for row := 0; row < len(c.tiles); row++ {
		energy := c.calculateEnergy(Pos{0, row}, E)
		if energy > best {
			best = energy
		}

		energy = c.calculateEnergy(Pos{width - 1, row}, W)
		if energy > best {
			best = energy
		}
	}

	// start at every column in top row and bottom row
	for col := 0; col < len(c.tiles[0]); col++ {
		// top row going south
		energy := c.calculateEnergy(Pos{col, 0}, S)
		if energy > best {
			best = energy
		}

		// bottom row going north
		energy = c.calculateEnergy(Pos{col, height - 1}, N)
		if energy > best {
			best = energy
		}
	}

	return best
}

type Grid struct {
	tiles     []string
	energized [][]bool
	seen      []bool
}

func (c *Grid) Reset() {
	clear(c.seen)
	c.ClearEnergy()
}

func (c *Grid) calculateEnergy(startPos Pos, startDir int) int {
	c.Reset()
	c.Beam(startPos, startDir)
	return c.CountEnergizedTiles()
}

func NewGrid(grid []string) *Grid {
	energyMatrix := makeEnergy(grid)
	seen := make([]bool, 110*110*10)
	return &Grid{grid, energyMatrix, seen}
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := NewGrid(strings.Split(string(bytes.TrimSpace(input)), "\n"))
	a1 := grid.calculateEnergy(Pos{0, 0}, E)
	a2 := pt2(grid)
	fmt.Printf("--- Day 16: The Floor Will Be Lava ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}