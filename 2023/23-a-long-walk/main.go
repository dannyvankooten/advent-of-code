package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var directions = [][2]int{
	{1, 0},  // EAST
	{0, 1},  // SOUTH
	{-1, 0}, // WEST
	{0, -1}, // NORTH
}

type Node struct {
	children []int
	steps    []int
	end      bool
	x, y     int
}

func copyGrid(grid [][]rune) [][]rune {
	c := make([][]rune, len(grid))
	for i := range grid {
		c[i] = make([]rune, len(grid[i]))
		copy(c[i], grid[i])
	}
	return c
}

func key(x int, y int) int {
	return y*150 + x
}

func createGraph(grid [][]rune, nodes map[int]Node, x int, y int) bool {
	availableDirections := directions
	steps := 0

	parentKey := key(x, y)
	parent, _ := nodes[parentKey]

	prevX, prevY := 0, 0

	for {
		// if at end node
		if y == len(grid)-1 && x == len(grid[y])-2 {
			k := key(x, y)
			if _, seen := nodes[k]; !seen {
				nodes[k] = Node{end: true, x: x, y: y}
			}
			parent.children = append(parent.children, k)
			parent.steps = append(parent.steps, steps)
			nodes[parentKey] = parent
			return true
		}

		switch grid[y][x] {
		case '.':
			availableDirections = directions
			break
		case '>':
			availableDirections = directions[0:1]
			break
		case 'v':
			availableDirections = directions[1:2]
			break
		case '<':
			availableDirections = directions[2:3]
			break
		case '^':
			availableDirections = directions[3:4]
			break
		}

		options := make([][2]int, 0, 4)
		for _, d := range availableDirections {
			x2 := x + d[0]
			y2 := y + d[1]
			if x2 < 0 || y2 < 0 || y2 >= len(grid) || x2 >= len(grid[y2]) {
				continue
			}

			// don't walk back in direction we came from
			if x2 == prevX && y2 == prevY {
				continue
			}

			if grid[y2][x2] == '#' {
				continue
			}

			options = append(options, [2]int{x2, y2})
		}

		if len(options) == 0 {
			return false
		} else if len(options) > 1 {
			// at junction, parse recursively
			for _, c := range options {
				k := key(c[0], c[1])
				if _, seen := nodes[k]; !seen {
					nodes[k] = Node{x: c[0], y: c[1]}
					createGraph(grid, nodes, c[0], c[1])
				}
				parent.children = append(parent.children, k)
				parent.steps = append(parent.steps, steps+1)
				nodes[parentKey] = parent
			}

			return len(parent.children) > 0
		} else {
			prevX = x
			prevY = y
			x = options[0][0]
			y = options[0][1]
			steps += 1
		}
	}

}

func pt1(nodes map[int]Node, n Node, steps int, visited map[int]bool) int {
	if n.end {
		return steps
	}

	// mark node as visited
	visited[key(n.x, n.y)] = true

	longest := 0
	for i := range n.children {
		k := n.children[i]
		child := nodes[k]
		if v, ok := visited[k]; ok && v {
			continue
		}

		s := pt1(nodes, child, steps+n.steps[i], visited)
		if s > longest {
			longest = s
		}

		visited[k] = false
	}

	return longest
}

func parse(input []byte) [][]rune {
	grid := make([][]rune, 0, 141)
	for _, l := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		grid = append(grid, []rune(l))
	}
	return grid
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input_test.txt")
	if err != nil {
		log.Fatal(err)
	}

	grid := parse(input)
	nodes := map[int]Node{
		key(1, 0): {x: 1, y: 0},
	}
	createGraph(grid, nodes, 1, 0)
	fmt.Printf("%#v\n", nodes)
	fmt.Printf("Start: %#v\n", nodes[key(1, 0)])

	pt1 := pt1(nodes, nodes[key(1, 0)], 0, make(map[int]bool))
	pt2 := 0

	fmt.Printf("--- Day 23: A Long Walk ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
