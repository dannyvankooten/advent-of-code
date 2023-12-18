package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"time"
)

func parse(input []byte) [][]int {
	input = bytes.TrimSpace(input)
	lines := bytes.Split(input, []byte("\n"))
	grid := make([][]int, len(lines))
	for i := range lines {
		grid[i] = make([]int, len(lines[i]))

		for j, ch := range lines[i] {
			n := int(ch) - '0'
			grid[i][j] = n
		}
	}

	return grid
}

const (
	PLANE_VERTICAL = iota
	PLANE_HORIZONTAL
	PLANE_UNDECIDED // special plane for start position
)

type Graph struct {
	vertices []Vertex
	width    int
	height   int
}

type Point struct {
	x int
	y int
}

type Vertex struct {
	position  Point
	direction int

	visited  bool
	heatloss int

	calculatedHeatloss int
	total              int
}

func (g *Graph) getEdges(u *Vertex, minSteps int, maxSteps int) []*Vertex {
	e := make([]*Vertex, 0, 4)

	if u.direction == PLANE_HORIZONTAL || u.direction == PLANE_UNDECIDED {
		for heatloss, dy := 0, 1; dy <= maxSteps; dy++ {
			v := g.getVertexByCoords(u.position.x, u.position.y+dy, PLANE_VERTICAL)
			if v != nil {
				heatloss += v.heatloss
				if dy >= minSteps {
					v.calculatedHeatloss = heatloss
					e = append(e, v)
				}
			}
		}
		for heatloss, dy := 0, 1; dy <= maxSteps; dy++ {
			v := g.getVertexByCoords(u.position.x, u.position.y-dy, PLANE_VERTICAL)
			if v != nil {
				heatloss += v.heatloss
				if dy >= minSteps {
					v.calculatedHeatloss = heatloss
					e = append(e, v)
				}
			}
		}
	}

	if u.direction == PLANE_VERTICAL || u.direction == PLANE_UNDECIDED {
		for heatloss, dx := 0, 1; dx <= maxSteps; dx++ {
			v := g.getVertexByCoords(u.position.x+dx, u.position.y, PLANE_HORIZONTAL)
			if v != nil {
				heatloss += v.heatloss
				if dx >= minSteps {
					v.calculatedHeatloss = heatloss
					e = append(e, v)
				}
			}
		}
		for heatloss, dx := 0, 1; dx <= maxSteps; dx++ {
			v := g.getVertexByCoords(u.position.x-dx, u.position.y, PLANE_HORIZONTAL)
			if v != nil {
				heatloss += v.heatloss
				if dx >= minSteps {
					v.calculatedHeatloss = heatloss
					e = append(e, v)
				}
			}
		}
	}

	return e
}

func (g *Graph) getVertexByCoords(x int, y int, plane int) *Vertex {
	if x < 0 || y < 0 || y >= g.height || x >= g.width {
		return nil
	}

	// since we have 2 planes per original grid node
	// index must be at (y * width * 2) + (x * 2) + plane
	return &g.vertices[y*2*g.width+x*2+plane]
}

func NewGraph(grid [][]int) Graph {
	graph := Graph{}
	vertices := make([]Vertex, 0, len(grid)*len(grid)*2)
	graph.height = len(grid)
	for y := range grid {
		graph.width = len(grid[y])
		for x := range grid[y] {
			vertices = append(vertices, Vertex{
				position:  Point{x, y},
				direction: PLANE_VERTICAL,
				total:     1 << 30,
				heatloss:  grid[y][x],
			})
			vertices = append(vertices, Vertex{
				position:  Point{x, y},
				direction: PLANE_HORIZONTAL,
				total:     1 << 30,
				heatloss:  grid[y][x],
			})
		}
	}
	graph.vertices = vertices
	return graph
}

func Dijkstra(grid [][]int, minSteps int, maxSteps int) int {
	graph := NewGraph(grid)
	vertices := graph.vertices

	// source vertex, 0 heatloss
	vertices[0].total = 0
	vertices[0].direction = PLANE_UNDECIDED

	// O(n) function to get next vertex to consider
	// TODO: we should use a priority queue based on https://pkg.go.dev/container/heap here
	next := func() *Vertex {
		var lowest *Vertex
		for i := range vertices {
			if vertices[i].visited == false && (lowest == nil || vertices[i].total < lowest.total) {
				lowest = &vertices[i]
			}

		}
		return lowest
	}

	var u *Vertex
	var e = &vertices[len(vertices)-1]
	for {
		u = next()

		// stop as soon as we're at end node
		if u.position.x == e.position.x && u.position.y == e.position.y {
			break
		}

		// mark vertice as visited
		u.visited = true

		// consider visitable neighbors
		for _, e := range graph.getEdges(u, minSteps, maxSteps) {
			if u.total+e.calculatedHeatloss < e.total {
				e.total = u.total + e.calculatedHeatloss
			}
		}
	}

	return u.total
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	grid := parse(input)

	a1 := Dijkstra(grid, 1, 3)
	a2 := Dijkstra(grid, 4, 10)
	fmt.Printf("--- Day 17: Clumsy Crucible ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}