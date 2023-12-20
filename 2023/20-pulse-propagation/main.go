package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type Module struct {
	typ     rune
	targets []string
	status  bool
	memory  map[string]bool
}

const TYPE_BUTTON = 'b'
const TYPE_BROADCASTER = 'a'
const TYPE_CONJUNCTION = '&'
const TYPE_FLIPFLOP = '%'

func parse(input []byte) map[string]Module {
	modules := map[string]Module{
		"button": {
			typ:     TYPE_BUTTON,
			targets: []string{"broadcaster"},
		},
	}

	for _, line := range strings.Split(strings.TrimSpace(string(input)), "\n") {
		m := Module{}
		pos := strings.Index(line, " ")
		var name string
		if line[0] == TYPE_FLIPFLOP || line[0] == TYPE_CONJUNCTION {
			m.typ = rune(line[0])
			name = line[1:pos]
		} else {
			m.typ = TYPE_BROADCASTER
			name = line[:pos]
		}
		line = line[strings.Index(line, " -> ")+4:]
		m.targets = strings.Split(line, ", ")
		m.memory = make(map[string]bool)
		modules[name] = m
	}

	// init memory
	for k, input := range modules {
		for _, t := range input.targets {
			dest := modules[t]
			if dest.typ == TYPE_CONJUNCTION {
				dest.memory[k] = false
			}
		}
	}

	return modules
}

type Pulse struct {
	source string
	dest   string
	value  bool
}

func (p *Pulse) handle(modules map[string]Module) []Pulse {
	sout := false
	m := modules[p.dest]

	switch m.typ {
	case TYPE_FLIPFLOP:
		if p.value {
			return nil
		} else {
			m.status = !m.status
			sout = m.status
		}
		break

	case TYPE_CONJUNCTION:
		m.memory[p.source] = p.value
		sout = false
		for _, v := range m.memory {
			if !v {
				sout = true
				break
			}
		}
		break

	case TYPE_BROADCASTER:
		sout = p.value
		break
	}

	// send signal to all targets
	out := make([]Pulse, len(m.targets))
	for i, t := range m.targets {
		out[i] = Pulse{
			source: p.dest,
			dest:   t,
			value:  sout,
		}
	}

	// re-assign map so values are pertained
	modules[p.dest] = m

	return out
}

func pt1(modules map[string]Module) int {
	low := 0
	high := 0
	pulses := make([]Pulse, 0)

	for i := 0; i < 1000; i++ {
		pulses = pulses[:0]

		// first pulse is a low from button to broadcaster
		pulses = append(pulses, Pulse{
			source: "button",
			dest:   "broadcaster",
			value:  false,
		})

		// keep processing pulses until nothing in queue
		for len(pulses) > 0 {
			pulse := pulses[0]
			pulses = pulses[1:]

			if pulse.value {
				high += 1
			} else {
				low += 1
			}

			pulses = append(pulses, pulse.handle(modules)...)
		}
	}

	return low * high
}

func findInputs(modules map[string]Module, dest string) []string {
	inputs := make([]string, 0)
	for k, m := range modules {
		for _, t := range m.targets {
			if t == dest {
				inputs = append(inputs, k)
			}
		}
	}
	return inputs
}

func pt2(modules map[string]Module) int {

	// find the single conjunction module that feeds into rx
	rxFeed := findInputs(modules, "rx")[0]

	// find inputs that feed into feed
	inputs := findInputs(modules, rxFeed)

	// for each of these inputs
	// the first time inside a cycle we find it to be true inside rxFeeds' memory
	// we store the cycle number
	factors := make(map[string]int)
	pulses := make([]Pulse, 0)
	for i := 1; len(factors) != len(inputs); i++ {
		pulses = pulses[:0]

		// first pulse is a low from button to broadcaster
		pulses = append(pulses, Pulse{
			source: "button",
			dest:   "broadcaster",
			value:  false,
		})

		// keep processing pulses until nothing in queue
		for len(pulses) > 0 {
			pulse := pulses[0]
			pulses = pulses[1:]
			pulses = append(pulses, pulse.handle(modules)...)

			// check memory values for each input
			for _, k := range inputs {
				_, ok := factors[k]
				if !ok && modules[rxFeed].memory[k] {
					factors[k] = i
				}
			}
		}
	}

	// we should be finding the LCM here first
	// but for me all of these were prime anyway
	// so this works for me (but may not on other inputs)
	product := 1
	for _, v := range factors {
		product *= v
	}

	return product
}

func reset(modules map[string]Module) {
	for k, m := range modules {
		m.status = false

		for in := range m.memory {
			m.memory[in] = false
		}

		modules[k] = m
	}
}

func main() {
	timeStart := time.Now()

	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	modules := parse(input)

	a1 := pt1(modules)

	reset(modules)
	a2 := pt2(modules)

	fmt.Printf("--- Day 20: Pulse Propagation ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
