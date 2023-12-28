package main

import (
	"fmt"
	"log"
	"math/rand"
	"os"
	"slices"
	"strings"
	"time"

	"github.com/gammazero/deque"
)

type Graph map[string][]string

func parse(input []byte) Graph {
	g := Graph{}

	for _, line := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		pos := strings.Index(line, ": ")
		left := line[:pos]
		right := strings.Split(line[pos+2:], " ")

		if _, ok := g[left]; !ok {
			g[left] = right
		} else {
			g[left] = append(g[left], right...)
		}

		for _, r := range right {
			if _, ok := g[r]; !ok {
				g[r] = []string{left}
			} else {
				g[r] = append(g[r], left)
			}
		}
	}
	return g
}

func calculateGroups(g Graph) int {
	// recursively add each component
	// + all its connection to a group
	seen := make(map[string]bool, len(g))
	size := 0
	q := deque.New[string](128)

	for left := range g {
		q.PushBack(left)
		seen[left] = true
		size += 1
		break
	}

	for q.Len() != 0 {
		u := q.PopBack()
		for _, v := range g[u] {
			if _, ok := seen[v]; ok {
				continue
			}

			q.PushBack(v)
			seen[v] = true
			size += 1
		}
	}

	return size * (len(g) - size)
}

func add_weights(seen map[[2]string]int, path []string) {
	for i := 0; i < len(path)-1; i += 1 {
		key := [2]string{path[i], path[i+1]}
		if _, ok := seen[key]; !ok {
			seen[key] = 1
		} else {
			seen[key] += 1
		}

		// key = [2]string{path[i+1], path[i]}
		// if _, ok := seen[key]; !ok {
		// 	seen[key] = 1
		// } else {
		// 	seen[key] += 1
		// }
	}

}

func bfs(g Graph, seen map[[2]string]int, from, to string) {
	q := deque.New[[]string](1024)
	visited := make(map[string]bool)

	// add initial node
	q.PushBack([]string{from})
	visited[from] = true

	// bfs other nodes
	for q.Len() != 0 {
		path := q.PopFront()
		u := path[len(path)-1]

		if u == to {
			add_weights(seen, path)
			return
		}

		for _, v := range g[u] {
			if visited[v] {
				continue
			}

			visited[v] = true
			q.PushBack(append(path, v))
		}
	}

}

func getHottestN(seen map[[2]string]int, n int) [][2]string {
	values := make([]int, n)
	edges := make([][2]string, n)

	for edge, count := range seen {
		for i := range values {
			if count > values[i] {
				values[i] = count
				edges[i] = edge
				break
			}
		}
	}

	return edges
}

func (g Graph) removeEdge(u string, v string) {
	g[u] = slices.DeleteFunc(g[u], func(e string) bool {
		return e == v
	})
	g[v] = slices.DeleteFunc(g[v], func(e string) bool {
		return e == u
	})
}

func getRandomNodePairs(g Graph, n int) [][2]string {
	nodes := make([]string, len(g))
	i := 0
	for k := range g {
		nodes[i] = k
		i += 1
	}

	intmap := make(map[int]bool, n)
	pairs := make([][2]string, n)
	for i := 0; i < n; i += 1 {
		var a, b int
		for a == b || intmap[(a<<16)+b] == true {
			a = rand.Intn(len(nodes))
			b = rand.Intn(len(nodes))
		}

		intmap[(a<<16)+b] = true
		pairs[i] = [2]string{nodes[a], nodes[b]}
	}

	return pairs
}

func solve(g Graph) int {

	// bfs path for N random node pairs
	// keep track of nodes seen along the way
	seen := make(map[[2]string]int)
	for _, edge := range getRandomNodePairs(g, 64) {
		bfs(g, seen, edge[0], edge[1])
	}

	// remove 3 hottest edges from graph
	for _, edge := range getHottestN(seen, 3) {
		g.removeEdge(edge[0], edge[1])
	}
	prod := calculateGroups(g)
	if prod == 0 {
		return solve(g)
	}

	return prod
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	g := parse(input)
	pt1 := solve(g)
	fmt.Printf("--- Day 25: Snowverload ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt1)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
