package main

import (
	"bytes"
	"fmt"
	"hash/fnv"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

func hash(cfg []byte, nums []int) uint64 {
	h := fnv.New64a()
	h.Write(cfg)

	for n := range nums {
		h.Write([]byte{byte(nums[n])})
	}

	return h.Sum64()
}

var cache map[uint64]int = make(map[uint64]int, 512)

func count(cfg []byte, nums []int) int {
	if len(cfg) == 0 {
		if len(nums) == 0 {
			return 1
		} else {
			return 0
		}
	}

	if len(nums) == 0 {
		if bytes.ContainsRune(cfg, '#') {
			return 0
		} else {
			return 1
		}
	}

	key := hash(cfg, nums)
	if v, ok := cache[key]; ok {
		return v
	}

	result := 0

	if cfg[0] == '.' || cfg[0] == '?' {
		result += count(cfg[1:], nums)
	}
	if cfg[0] == '#' || cfg[0] == '?' {
		if nums[0] <= len(cfg) &&
			!bytes.ContainsRune(cfg[:nums[0]], '.') &&
			(nums[0] == len(cfg) || cfg[nums[0]] != '#') {

			e := nums[0]
			if nums[0] < len(cfg) {
				e += 1
			}
			result += count(cfg[e:], nums[1:])
		}
	}

	cache[key] = result

	return result
}

func main() {
	timeStart := time.Now()
	b, err := os.ReadFile("input.txt")
	// b, err := os.ReadFile("input_test.txt")
	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(string(bytes.TrimSpace(b)), "\n")
	blueprint := make([]int, 0, 256)

	pt1 := 0
	pt2 := 0
	for _, l := range lines {
		spacePos := strings.Index(l, " ")
		a := []byte(l[:spacePos])
		b := l[spacePos+1:]

		blueprint = blueprint[:0]
		for _, d := range strings.Split(b, ",") {
			n, err := strconv.Atoi(d)
			if err != nil {
				panic(err)
			}
			blueprint = append(blueprint, n)
		}

		// part 1
		f1 := count(a, blueprint)
		pt1 += f1

		// // part 2
		a2 := bytes.Join([][]byte{a, a, a, a, a}, []byte("?"))
		b1 := blueprint
		for i := 0; i < 4; i++ {
			blueprint = append(blueprint, b1...)
		}
		f2 := count(a2, blueprint)
		pt2 += f2
		// fmt.Printf("%03d / %03d: %d arrangements \n", lineno, len(lines)-1, f2)
	}

	fmt.Printf("--- Day 12: Hot Springs ---\n")
	fmt.Printf("Part 1: %d\n", pt1)
	fmt.Printf("Part 2: %d\n", pt2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
