package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type Part map[rune]int

type Workflow []Rule

type Rule struct {
	category    rune
	operator    rune
	right       int
	consequence string
}

func parse(input []byte) (map[string]Workflow, []Part) {
	sections := strings.Split(string(input), "\n\n")

	// parse workflows
	workflows := make(map[string]Workflow)
	for _, line := range strings.Split(sections[0], "\n") {
		pos := strings.Index(line, "{")
		name := line[0:pos]
		line = line[pos+1 : len(line)-1]

		w := make(Workflow, 0, 4)
		for _, rule := range strings.Split(line, ",") {
			r := Rule{}
			pos = strings.Index(rule, ":")
			if pos > -1 {
				r.category = rune(rule[0])
				r.operator = rune(rule[1])
				r.right, _ = strconv.Atoi(rule[2:pos])
				r.consequence = rule[pos+1:]
			} else {
				r.consequence = rule
			}
			w = append(w, r)
		}

		workflows[name] = w
	}

	// parse parts
	parts := make([]Part, 0)
	for _, line := range strings.Split(sections[1], "\n") {
		var x, m, a, s int
		fmt.Sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s)
		parts = append(parts, Part{
			'x': x,
			'm': m,
			'a': a,
			's': s,
		})
	}

	return workflows, parts
}

func applyWorkflow(workflows map[string]Workflow, workflow string, part Part) bool {
	if workflow == "R" {
		return false
	} else if workflow == "A" {
		return true
	}

	for _, r := range workflows[workflow] {
		var result bool

		switch r.operator {
		case '>':
			result = part[r.category] > r.right
			break
		case '<':
			result = part[r.category] < r.right
			break
		default:
			result = true
			break
		}

		// keep processing rules if expression evaluated to false
		if !result {
			continue
		}

		return applyWorkflow(workflows, r.consequence, part)
	}

	return false
}

func p1(workflows map[string]Workflow, parts []Part) int {
	sum := 0
	for _, p := range parts {
		if applyWorkflow(workflows, "in", p) {
			for _, v := range p {
				sum += v
			}
		}
	}
	return sum
}

func cloneMap(original map[rune][2]int) map[rune][2]int {
	c := make(map[rune][2]int, 4)
	for k, v := range original {
		c[k] = v
	}
	return c
}

func count(workflows map[string]Workflow, workflow string, values map[rune][2]int) int {
	if workflow == "R" {
		return 0
	} else if workflow == "A" {
		product := 1
		for _, v := range values {
			product *= (v[1] - v[0] + 1)
		}
		return product
	}

	total := 0
	for _, r := range workflows[workflow] {
		// low high for this category
		v := values[r.category]

		// for every rule, the category this rule applies to splits
		// in a true and a false range
		// one below and one above the condition
		var tv [2]int
		var fv [2]int
		if r.operator == '<' {
			tv = [2]int{v[0], r.right - 1}
			fv = [2]int{r.right, v[1]}
		} else if r.operator == '>' {
			tv = [2]int{r.right + 1, v[1]}
			fv = [2]int{v[0], r.right}
		} else {
			total += count(workflows, r.consequence, values)
			continue
		}

		// for true range, create a clone and keep counting
		// but start at consequence instead
		if tv[0] <= tv[1] {
			v2 := cloneMap(values)
			v2[r.category] = tv
			total += count(workflows, r.consequence, v2)
		}

		// for false range, keep processing rest of rules
		// unless low is already higher than high
		// then we can just break
		if fv[0] > fv[1] {
			break
		}

		values[r.category] = fv
	}

	return total
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	workflows, parts := parse(input)

	pt1 := p1(workflows, parts)
	pt2 := count(workflows, "in", map[rune][2]int{
		'x': {1, 4000},
		'm': {1, 4000},
		'a': {1, 4000},
		's': {1, 4000},
	})

	fmt.Printf("--- Day 19: Aplenty ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}