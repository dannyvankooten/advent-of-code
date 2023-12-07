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
	_ = iota
	_
	CARD_2
	CARD_3
	CARD_4
	CARD_5
	CARD_6
	CARD_7
	CARD_8
	CARD_9
	CARD_T
	CARD_J
	CARD_Q
	CARD_K
	CARD_A
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
	charToCard := map[rune]int{
		'2': CARD_2,
		'3': CARD_3,
		'4': CARD_4,
		'5': CARD_5,
		'6': CARD_6,
		'7': CARD_7,
		'8': CARD_8,
		'9': CARD_9,
		'T': CARD_T,
		'J': CARD_J,
		'K': CARD_K,
		'Q': CARD_Q,
		'A': CARD_A,
	}

	for _, line := range strings.Split(string(input), "\n") {
		if line == "" {
			continue
		}
		pos := strings.Index(line, " ")
		cards := make([]int, 0, 8)
		var h int
		for _, c := range line[:pos] {
			if _, ok := charToCard[c]; !ok {
				panic("unexpected input")
			}
			h = charToCard[c]
			cards = append(cards, h)
		}

		b, err := strconv.Atoi(line[pos+1:])
		if err != nil {
			panic("error parsing int")
		}
		hands = append(hands, Hand{
			Cards: cards,
			Bid:   b,
		})
	}

	return hands
}

type Hand struct {
	Cards []int
	Bid   int
}

func (h *Hand) Type() int {
	counts := make([]int, 15)
	for _, c := range h.Cards {
		counts[c]++
	}

	threes := 0
	pairs := 0
	for _, c := range counts {
		if c >= 5 {
			return FIVE_OF_A_KIND
		}

		if c >= 4 {
			return FOUR_OF_A_KIND
		}

		if c >= 3 {
			threes++
			continue
		}

		if c >= 2 {
			pairs++
			continue
		}
	}

	if pairs == 1 && threes == 1 {
		return FULL_HOUSE
	}

	if threes == 1 {
		return THREE_OF_A_KLIND
	}

	if pairs == 2 {
		return TWO_PAIR
	}

	if pairs == 1 {
		return ONE_PAIR
	}

	return HIGH_CARD
}

type CardCount struct {
	Card  int
	Count int
}

func (h *Hand) Type2() int {
	counts := make([]CardCount, 15)
	for _, c := range h.Cards {
		counts[c].Card = c
		counts[c].Count++
	}

	jokers := counts[CARD_J].Count

	// sort counts by highest first
	// because we want to use jokers for highest possible combo
	sort.Slice(counts, func(i, j int) bool {
		return counts[i].Count >= counts[j].Count
	})

	threes := 0
	pairs := 0
	for _, c := range counts {
		if c.Card == CARD_J {
			continue
		}

		count := c.Count + jokers
		if count >= 5 {
			return FIVE_OF_A_KIND
		}

		if count >= 4 {
			return FOUR_OF_A_KIND
		}

		if count >= 3 {
			jokers -= 3 - c.Count
			threes++
			continue
		}

		if count >= 2 {
			jokers -= 2 - c.Count
			pairs++
			continue
		}
	}

	if pairs >= 1 && threes >= 1 {
		return FULL_HOUSE
	}

	if threes >= 1 {
		return THREE_OF_A_KLIND
	}

	if pairs >= 2 {
		return TWO_PAIR
	}

	if pairs >= 1 {
		return ONE_PAIR
	}

	return HIGH_CARD
}

func pt1(hands []Hand) int {
	sort.Slice(hands, func(i, j int) bool {
		diff := hands[i].Type() - hands[j].Type()

		if diff == 0 {
			for c := 0; c < len(hands[i].Cards); c++ {
				if hands[i].Cards[c] != hands[j].Cards[c] {
					return hands[i].Cards[c] < hands[j].Cards[c]
				}
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

func pt2(hands []Hand) int {
	sort.Slice(hands, func(i, j int) bool {
		diff := hands[i].Type2() - hands[j].Type2()

		if diff == 0 {
			for c := 0; c < len(hands[i].Cards); c++ {
				if hands[i].Cards[c] == hands[j].Cards[c] {
					continue
				}

				a := hands[i].Cards[c]
				b := hands[j].Cards[c]
				if a == CARD_J {
					a = 1
				}
				if b == CARD_J {
					b = 1
				}
				return a < b
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

func main() {
	timeStart := time.Now()
	hands := parse("input.txt")

	a1 := pt1(hands)
	a2 := pt2(hands)

	fmt.Print("---- Day 6: Wait For It ---\n")
	fmt.Printf("Part 1: %d\n", a1)
	fmt.Printf("Part 2: %d\n", a2)

	fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
