package main

import (
	"bufio"
	"bytes"
	"fmt"
	"os"
	"unicode"
)

func main() {
	pt1()
	pt2();
}

func pt1() {
	fh, _ := os.Open("input.txt")
	defer fh.Close()
	scanner := bufio.NewScanner(fh)
	var sum int32

	for scanner.Scan() {
		digits := make([]int32, 0)
		for _, c := range scanner.Text() {
			if unicode.IsDigit(c) {
				digits = append(digits, c - 48)
			}
		}

		value := digits[0] * 10 + digits[len(digits)-1]
		sum += value

	}

	fmt.Printf("part 1: %d\n", sum)
}

func pt2() {
	fh, _ := os.Open("input.txt")
	defer fh.Close()

	scanner := bufio.NewScanner(fh)
	var sum int32

	digitWords := [][]byte{
		[]byte("one"),
		[]byte("two"),
		[]byte("three"),
		[]byte("four"),
		[]byte("five"),
		[]byte("six"),
		[]byte("seven"),
		[]byte("eight"),
		[]byte("nine"),
	}

	digits := make([]int32, 16)

	for scanner.Scan() {
		// replace words with digit counterparts
		b := scanner.Bytes()

		digits = digits[:0]

		for len(b) > 0 {
			if b[0] >= 48 && b[0] < 58 {
				digits = append(digits, int32(b[0]) - 48)
			} else {
				for i, w := range digitWords {
					if bytes.HasPrefix(b, w) {
						digits = append(digits, int32(i) + 1)
					}
				}
			}

			b = b[1:]
		}

		value := digits[0] * 10 + digits[len(digits)-1]
		sum += value
	}

	fmt.Printf("part 2: %d\n", sum)
}
