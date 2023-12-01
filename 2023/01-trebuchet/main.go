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
	pt2()
}

func pt1() {
	fh, _ := os.Open("input.txt")
	defer fh.Close()
	var sum int32
	var value int32
	digits := make([]int32, 0, 16)

	scanner := bufio.NewScanner(fh)
	for scanner.Scan() {
		digits = digits[:0]
		for _, c := range scanner.Text() {
			if unicode.IsDigit(c) {
				digits = append(digits, c - 48)
			}
		}

		value = digits[0] * 10 + digits[len(digits)-1]
		sum += value
	}

	fmt.Printf("part 1: %d\n", sum)
}

func pt2() {
	fh, _ := os.Open("input.txt")
	defer fh.Close()

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

	var sum int32
	var value int32
	digits := make([]int32, 0, 16)

	scanner := bufio.NewScanner(fh)
	for scanner.Scan() {
		b := scanner.Bytes()

		// TODO: We could just store first and last digit vs. all digits on that line
		// clear digits array from previous line
		digits = digits[:0]


		for len(b) > 0 {
			// check for 0 - 9 characters. 48 is unicode point for 0.
			if b[0] >= 48 && b[0] < 58 {
				digits = append(digits, int32(b[0]) - 48)
			} else {
				// check for "one" - "nine"
				for i, w := range digitWords {
					if bytes.HasPrefix(b, w) {
						digits = append(digits, int32(i)+1)
						// TODO: We may be able to skip ahead here
						// But not by entire length of words, as sometimes digits share characters with their previous one
					}
				}

			}

			b = b[1:]
		}

		value = digits[0] * 10 + digits[len(digits)-1]
		sum += value
	}

	fmt.Printf("part 2: %d\n", sum)
}
