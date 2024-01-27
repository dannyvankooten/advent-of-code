package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
	"time"
)

const (
	HIGH_CARD = iota
	ONE_PAIR
	TWO_PAIR
	THREE_OF_A_KLIND
	FULL_HOUSE
	FOUR_OF_A_KIND
	FIVE_OF_A_KIND
)

func parse(filename string) []Hand {
	input, _ := os.ReadFile(filename)
	hands := make([]Hand, 0, 64)

	for _, line := range strings.Split(string(input), "\n") {
		if line == "" {
			continue
		}
		pos := strings.Index(line, " ")
		cards := make([]rune, 0, 5)
		countMap := make(map[rune]int)

		for _, c := range line[:pos] {
			cards = append(cards, c)
			countMap[c]++
		}

		b, err := strconv.Atoi(line[pos+1:])
		if err != nil {
			panic("error parsing int")
		}

		counts := make([]int, 0, 5)
		for _, c := range countMap {
			counts = append(counts, c)
		}
		sort.Slice(counts, func(i, j int) bool {
			return counts[i] > counts[j]
		})
		hands = append(hands, Hand{
			Cards:  cards,
			Bid:    b,
			Counts: counts,
		})
	}

	return hands
}

type Hand struct {
	Cards  []rune
	Bid    int
	Counts []int
}

func (h *Hand) Type() int {
	if h.Counts[0] >= 5 {
		return FIVE_OF_A_KIND
	} else if h.Counts[0] >= 4 {
		return FOUR_OF_A_KIND
	} else if h.Counts[0] >= 3 && h.Counts[1] >= 2 {
		return FULL_HOUSE
	} else if h.Counts[0] >= 3 {
		return THREE_OF_A_KLIND
	} else if h.Counts[0] >= 2 && h.Counts[1] >= 2 {
		return TWO_PAIR
	} else if h.Counts[0] >= 2 {
		return ONE_PAIR
	}

	return HIGH_CARD
}

func solve(hands []Hand, cardValues map[rune]int) int {
	sort.Slice(hands, func(i, j int) bool {
		diff := hands[i].Type() - hands[j].Type()

		if diff == 0 {
			for c := 0; c < len(hands[i].Cards); c++ {
				if hands[i].Cards[c] == hands[j].Cards[c] {
					continue
				}

				return cardValues[hands[i].Cards[c]] < cardValues[hands[j].Cards[c]]
			}
		}

		return diff < 0
	})

	winnings := 0
	for rank, h := range hands {
		winnings += (rank + 1) * h.Bid
	}
	return winnings
}

func (h *Hand) distributeJokers() {
	countMap := make(map[rune]int, 5)
	for _, c := range h.Cards {
		countMap[c]++
	}

	// in case we only counted a single card type (incl. jokers)
	if len(countMap) == 1 {
		h.Counts = []int{5}
		return
	}

	counts := make([]int, 0, 5)
	for card, count := range countMap {
		if card == 'J' {
			continue
		}

		counts = append(counts, count)
	}
	sort.Slice(counts, func(i, j int) bool {
		return counts[i] > counts[j]
	})
	nJokers := countMap['J']
	diff := max(0, 3-counts[0])
	counts[0] += nJokers
	nJokers -= diff
	if len(counts) > 1 && nJokers > 0 {
		counts[1] += nJokers
	}

	h.Counts = counts
}

func main() {
	timeStart := time.Now()
	hands := parse("input.txt")

	cardValues := map[rune]int{
		'2': 1,
		'3': 2,
		'4': 3,
		'5': 4,
		'6': 5,
		'7': 6,
		'8': 7,
		'9': 8,
		'T': 9,
		'J': 10,
		'H': 11,
		'Q': 12,
		'K': 13,
		'A': 14,
	}
	a1 := solve(hands, cardValues)

	cardValues['J'] = 0
	for i := range hands {
		hands[i].distributeJokers()
	}
	a2 := solve(hands, cardValues)

	fmt.Print("--- Day 7: Camel Cards ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)
	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
