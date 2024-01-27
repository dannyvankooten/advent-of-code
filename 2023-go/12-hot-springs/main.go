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

func parseLine(l string) ([]byte, []int) {
	spacePos := strings.Index(l, " ")
	cfg := []byte(l[:spacePos])

	// parse comma separated list of ints without allocating
	b := l[spacePos+1:]
	nums := make([]int, 0, 16)
	s, after, _ := strings.Cut(b, ",")
	for len(s) > 0 {
		n, err := strconv.Atoi(s)
		if err != nil {
			panic(err)
		}
		nums = append(nums, n)
		s, after, _ = strings.Cut(after, ",")
	}

	return cfg, nums
}

func pt2(cfg []byte, nums []int) int {
	cfg = bytes.Join([][]byte{cfg, cfg, cfg, cfg, cfg}, []byte("?"))
	n := nums
	for i := 0; i < 4; i++ {
		nums = append(nums, n...)
	}
	return count(cfg, nums)
}

func main() {
	timeStart := time.Now()
	b, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(string(bytes.TrimSpace(b)), "\n")

	ans1 := 0
	ans2 := 0
	for _, l := range lines {
		cfg, blueprint := parseLine(l)
		ans1 += count(cfg, blueprint)
		ans2 += pt2(cfg, blueprint)
	}

	fmt.Printf("--- Day 12: Hot Springs ---\n")
	fmt.Printf("Part 1: %d\n", ans1)
	fmt.Printf("Part 2: %d\n", ans2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
